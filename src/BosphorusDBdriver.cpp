//
// Created by jdq on 17-12-26.
//



#include "unit.h"
#include "BosphorusDBdriver.h"

/*
    BosphorusDBdriver is the constructor. It read the 3d coordinate(.but), the 2D landmark(.lm2) and 3D landmark(.lm3)
*/
BosphorusDBdriver::BosphorusDBdriver(string filepath)
{
    long size;
    std::string filename_data = filepath +  ".bnt";
    std::string filename_lm2 = filepath +  ".lm2";
    std::string filename_lm3 = filepath + ".lm3";
    cout << filename_data <<endl;
    ifstream in(filename_data, ios::in | ios::binary | ios::ate);
    if (!in.is_open())
    {
        cout << "Error opening file" << endl;
        cout << filename_data;
        exit(1);
    }
    uint16_t len;
    double k;
    size = in.tellg();
    in.seekg(0, ios::beg);
    in.read((char*)&nrows, sizeof(uint16_t));
    in.read((char*)&ncols, sizeof(uint16_t));
    in.read((char*)&zmin, sizeof(double));
    in.read((char*)&len, sizeof(uint16_t));
    imfile = new char[len + 1];
    in.read((char*)imfile, sizeof(char)*len);
    in.read((char*)&len2, sizeof(uint32_t));
    data = new double*[len2 / 5];
    for (int i = 0; i < len2 / 5; i++)
        data[i] = new double[5];
    for (int j = 0; j < 5; j++)
    {
        for (int i = 0; i < len2 / 5; i++)
        {
            in.read((char*)&k, sizeof(double));
            data[i][j] = k;
        }
    }
    in.close();

    // read 2d landmark
    std::string buff;
    in.open(filename_lm2, ios::in);
    if (!in.is_open())
    {
        cout << "Error opening file";
        exit(1);
    }
    getline(in, buff);
    cout << buff << endl;
    getline(in, buff);
    in >> landmark2dn;
    //cout<<"landmark2dn:"<<landmark2dn<<endl;
    getline(in, buff);
    getline(in, buff);
    getline(in, buff);
    for (int i = 0; i < landmark2dn; i++)
    {
        getline(in, buff);
        labels2d.push_back(buff);
    }
    getline(in, buff);
    getline(in, buff);
    for (int i = 0; i < landmark2dn; i++)
    {
        double a, b;
        in >> a >> b;
        Point pt = Point(a, b);
        pts2d.push_back(pt);
    }
    in.close();

    //read landmarks 3d
    in.open(filename_lm3, ios::in);
    if (!in.is_open())
    {
        cout << "Error opening file";
        exit(1);
    }
    getline(in, buff);
    while (buff.empty())
    {
        getline(in, buff);
    }
    in >> landmark3dn;
    //cout<<"landmark3dn:"<<landmark3dn<<endl;
    getline(in, buff);
    for (int i = 0; i < landmark3dn; i++)
    {
        getline(in, buff);
        labels3d.push_back(buff);
        double a, b, c;
        in >> a >> b >> c;
        Point3d pt = Point3d(a, b, c);
        pts3d.push_back(pt);
        getline(in, buff);
    }
    in.close();
    cout << "the complete file is in a buffer" << endl;
}
BosphorusDBdriver::~BosphorusDBdriver()
{
    /*for (int i = 0; i < len2 / 5; i++)
         delete[] data[i];
    delete[] data;
    delete[] imfile;*/
}


bool BosphorusDBdriver::uperTriangleValid(Mat mask, const uint16_t row, const uint16_t col)
{
    int widthImage = mask.cols;
    int heightImage = mask.rows;
    if (row == heightImage - 1 || col == widthImage - 1)
        return false;
    if (mask.at<cv::Vec3d>(row, col) == cv::Vec3d(0, 0, 0) || mask.at<cv::Vec3d>(row + 1, col) == cv::Vec3d(0, 0, 0) || mask.at<cv::Vec3d>(row, col + 1) == cv::Vec3d(0, 0, 0))
        return false;
    else
        return true;
}
bool BosphorusDBdriver::lowerTriangleValid(Mat mask, const uint16_t row, const uint16_t col)
{
    int widthImage = mask.cols;
    int heightImage = mask.rows;
    if (row == heightImage - 1 || col == widthImage - 1)
        return false;
    if (mask.at<cv::Vec3d>(row + 1, col + 1) == cv::Vec3d(0, 0, 0) || mask.at<cv::Vec3d>(row + 1, col) == cv::Vec3d(0, 0, 0) || mask.at<cv::Vec3d>(row, col + 1) == cv::Vec3d(0, 0, 0))
        return false;
    else
        return true;
}

cv::Mat BosphorusDBdriver::getdepthmap()
{
    cv::Mat depthImage = cv::Mat::zeros(nrows, ncols, CV_64FC1);
    cv::Mat depthImagel = cv::Mat::zeros(nrows, ncols, CV_16UC1);
    for (int row = 0; row < nrows; row++)
        for (int col = 0; col < ncols; col++)
        {
            if (data[row*ncols + col][2] == zmin) depthImage.at<double>(nrows*data[row*ncols + col][4], ncols*data[row*ncols + col][3]) = 0;
            else  depthImage.at<double>(nrows*data[row*ncols + col][4], ncols*data[row*ncols + col][3]) = ncols*data[row*ncols + col][2];
        }
    double minp, maxp;
    minMaxIdx(depthImage, &minp, &maxp);
    for (int r = 0; r < depthImage.rows; r++)
        for (int c = 0; c < depthImage.cols; c++)
            if (depthImage.at<double>(r, c) != 0)
                depthImagel.at<unsigned short>(r, c) = (depthImage.at <double>(r, c) - minp) / (maxp - minp) * 65535;
            else depthImagel.at<unsigned short>(r, c) = 0;
    return depthImagel;
}

MyTriMesh BosphorusDBdriver::getMesh()
{
    MyTriMesh mesh;
    int vertexCount = nrows * ncols;
    MyTriMesh::VertexHandle* vhandle = new MyTriMesh::VertexHandle[vertexCount];
    cv::Mat mask = cv::Mat::zeros(nrows, ncols, CV_64FC3);
    for (int idx = 0; idx < vertexCount; idx++)
    {
        vhandle[idx] = mesh.add_vertex(MyTriMesh::Point(data[idx][0], data[idx][1], data[idx][2]));
        if (data[idx][0] == zmin)
            mask.at<cv::Vec3d>(idx / ncols, idx%ncols) = cv::Vec3d(0, 0, 0);
        else
            mask.at<cv::Vec3d>(idx / ncols, idx%ncols) = cv::Vec3d(data[idx][0], data[idx][1], data[idx][2]);
    }

    std::set<int> validVertIdx;
    // Insert topology on the Mesh
    std::vector<MyTriMesh::VertexHandle>  face_vhandles;
    for (int row = 0; row < nrows; row++)
    {
        for (int col = 0; col < ncols; col++)
        {
            if (uperTriangleValid(mask, row, col))
            {
                int index_0 = row*ncols + col;
                int index_2 = row*ncols + col + 1;
                int index_1 = (row + 1)*ncols + col;
                //record valid vertices
                validVertIdx.insert(index_0);
                validVertIdx.insert(index_1);
                validVertIdx.insert(index_2);

                face_vhandles.clear();
                face_vhandles.push_back(vhandle[index_0]);
                face_vhandles.push_back(vhandle[index_1]);
                face_vhandles.push_back(vhandle[index_2]);
                mesh.add_face(face_vhandles);
            }

            if (lowerTriangleValid(mask, row, col))
            {
                int index_0 = (row + 1)*ncols + col;
                int index_2 = row*ncols + col + 1;
                int index_1 = (row + 1)*ncols + col + 1;
                //record valid vertices
                validVertIdx.insert(index_0);
                validVertIdx.insert(index_1);
                validVertIdx.insert(index_2);

                face_vhandles.clear();
                face_vhandles.push_back(vhandle[index_0]);
                face_vhandles.push_back(vhandle[index_1]);
                face_vhandles.push_back(vhandle[index_2]);
                mesh.add_face(face_vhandles);
            }
        }
    }
    if (!mesh.has_vertex_status())
        mesh.request_vertex_status();
    if (!mesh.has_face_status())
        mesh.request_face_status();
    if (!mesh.has_edge_status())
        mesh.request_edge_status();
    //find the invalid pt set
    vector<int> invalidVertIdx;
    invalidVertIdx.reserve(vertexCount - validVertIdx.size());
    std::set<int>::iterator iter = validVertIdx.begin();
    for (int i = 0; i<vertexCount; i++)
    {
        if (i<(*iter))
            invalidVertIdx.push_back(i);
        else
        {
            if (iter != validVertIdx.end())
            {
                iter++;
                if (iter == validVertIdx.end())
                {
                    for (int j = i + 1; j<vertexCount; j++)
                        invalidVertIdx.push_back(j);
                    break;
                }
            }
        }
    }
    for (int i = 0; i<invalidVertIdx.size(); i++)
        mesh.delete_vertex(mesh.vertex_handle(invalidVertIdx[i]), false);

    delete vhandle;
    mesh.garbage_collection();
    //release attribute
    if (mesh.has_vertex_status())
        mesh.release_vertex_status();
    if (mesh.has_face_status())
        mesh.release_vertex_status();
    if (mesh.has_edge_status())
        mesh.release_vertex_status();

    //update face normal
    mesh.request_face_normals();
    mesh.update_face_normals();



    //update vertex normal
    mesh.request_vertex_normals();
    mesh.update_vertex_normals();

    return mesh;
}

vector<Bosphouruspoint2d> BosphorusDBdriver::find3dlmOnDepth()
{
    vector<Bosphouruspoint2d> pts;
    for (int i = 0; i<landmark3dn; i++)
        for (int j = 0; j < nrows*ncols; j++)
            if (pts3d[i] == cv::Point3d(data[j][0], data[j][1], data[j][2]))
            {
                Bosphouruspoint2d tp;
                tp.pts2d=cv::Point2d(j / ncols, j%ncols);
                tp.labels2d=label2point[labels3d[i]];
                //cout<<":"<<label2point[labels3d[i]]<<endl;

               // string s;
               // s="Inner left eyebrow";
               // cout<<s.size()<<endl;
               // if (s==labels3d[1]) cout<<"yes"<<endl;
               // cout<<label2point[s]<<endl;
               // cout<<"S"<<int(labels3d[1][18])<<endl;
               // cout<<":"<<label2point[labels3d[1]]<<endl;
                pts.push_back(tp);
                break;
            }


    return pts;
}
Mat BosphorusDBdriver::get3dpointMat()
{
    cv::Mat depthImage = cv::Mat::zeros(nrows, ncols, CV_64FC3);
    for (int row = 0; row < nrows; row++)
        for (int col = 0; col < ncols; col++)
        {
            if (data[row*ncols + col][2] == zmin) depthImage.at<cv::Vec3d>(nrows*data[row*ncols + col][4], ncols*data[row*ncols + col][3]) = cv::Vec3d(0,0,0);
            else  depthImage.at<cv::Vec3d>(nrows*data[row*ncols + col][4], ncols*data[row*ncols + col][3])
                          = cv::Vec3d(data[row*ncols + col][1],data[row*ncols + col][0],data[row*ncols + col][2]);
        }
    return depthImage;
}

std::vector<Bosphouruspoint2d> BosphorusDBdriver::getBosphouruspoint2d()
{
    vector<Bosphouruspoint2d> dst;
    for (int i=0;i<landmark2dn;i++)
    {
        Bosphouruspoint2d temp;
        temp.pts2d=pts2d[i];
        temp.labels2d=label2point[labels2d[i]];
        dst.push_back(temp);
    }
    return dst;
}

std::vector<Bosphouruspoint3d> BosphorusDBdriver::getBosphouruspoint3d()
{
    vector<Bosphouruspoint3d> dst;
    for (int i=0;i<landmark3dn;i++)
    {
        Bosphouruspoint3d temp;
        temp.pts3d=pts3d[i];
        temp.labels3d=label2point[labels3d[i]];
        dst.push_back(temp);
    }
    return dst;
}

BosphorusDBdriver::BosphorusDBdriver() {}
