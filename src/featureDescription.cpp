//
// Created by jdq on 17-12-26.
//


#include "featureDescription.h"

FeatureDescriptor::FeatureDescriptor(BosphorusDBdriver bp)
{
    this->bp=bp;
    this->depth = bp.getdepthmap();
    this->mesh = bp.getMesh();
    this->label3dpts=bp.getBosphouruspoint3d();
    //this->Mat3D = bp.get3dpointMat();
}

FeatureDescriptor::~FeatureDescriptor()
{
}
/* The parameters of the Gabor wavelet are scale, orientation, windows width;
 * To get the reasonable results wo limit the scale, orientation and  windows width
 * kernel_size = int(win*scale*kkernel)*2+1;
 * lamda=scale
 *
*/
Mat FeatureDescriptor::depth2normal(Mat& img)
{
    int cols=img.cols;
    int rows=img.rows;
    //imshow("4",img);
    img=img*cols/2;
    img.convertTo(img,CV_64FC1);
    //imshow("3",img);
    //normalize(img,img,0,1,32);

    Mat dst=cv::Mat::zeros(rows, cols, CV_64FC3);
    int a[]={-1,-1, -1,0, -1,1, 0,1, 1,1, 1,0, 1,-1, 0,-1,-1,-1};
    for (int i=1; i<rows; i++) {
        for (int j = 1; j < cols; j++) {
            cv::Vec3d sumv = cv::Vec3d(0, 0, 0);
            for (int k = 0; k < 8; k++) {
                cv::Vec3d t=cv::Vec3d(a[2 * k], a[2 * k + 1],
                                        img.at<double>(i + a[2 * k], j + a[2 * k + 1])-img.at<double>(i, j))
                        .cross(cv::Vec3d(-a[2 * k+2], -a[2 * k + 3],
                                           img.at<double>(i + a[2 * k+2], j + a[2 * k + 3])-img.at<double>(i, j)));
           //     cout << "t:" << t << endl;
                normalize(t,t);
                sumv+=t;

            }
            normalize(sumv,sumv);
            sumv=sumv/2+cv::Vec3d(0.5,0.5,0.5);
            dst.at<cv::Vec3d>(i,j)=sumv;
        }
    }
//    vector<Mat> channels;
//    split(dst,channels);
//    Mat c=cv::Mat::zeros(rows, cols, CV_64FC1);
//    normalize(channels[0],channels[0],-1,1,32);
//    normalize(channels[1],channels[1],-1,1,32);
//    normalize(channels[2],channels[2],-1,1,32);
//    for (int i=0;i<rows;i++)
//    {
//        for (int j=0;j<cols;j++)
//            cout<<c.at<double>(i,j)<<" ";
//        cout<<endl; }
//
//    imshow("0",channels[0]);
//    imshow("1",channels[1]);
//    imshow("2",channels[2]);
//
//
// imshow("1",dst);
    return dst;
}

 void FeatureDescriptor::GaborFeature(int scale, int th, int win, vector<Mat> &features)
{

    const double kkernel=3;
    const double winmax=0.5, winmin=0.1;
    double gam=1;
    int rows = depth.rows*scale;
    int cols = depth.cols*(th + 1);
    cv::Mat maxth = cv::Mat::zeros(depth.rows, depth.cols, CV_64FC1);
    cv::Mat dst = cv::Mat::zeros(rows, cols, CV_64FC1);
    cout<<"start Gabor feature"<<endl;
    for (int i=0;i<scale;i++)
    {
        double lamda=scale;
        for (int j=0; j<win; j++)
        {
            double sigma=((winmax-winmin)/win*j+winmin)*lamda;
            int kernel_size = int(sigma * kkernel) * 2 + 1;
            cv::Size size(kernel_size*2+1, kernel_size*2+1);
            for (int k=0; k<th; k++) {
                double theta = k * 1.0 / th * CV_PI / 2;
                cv::Mat kernel = cv::getGaborKernel(size, sigma, theta, lamda, gam, 0);
                cv::Mat dest;
                cv::filter2D(depth, dest, CV_64F, kernel);
                normalize(dest, dest, -1, 1, 32);
                //Mat dst2=depth2normal(dest);
                //cv::imwrite("../data/output/out_"+to_string(i)+"_"+to_string(j)+"_"+to_string(k)+".png", dest*255);
                features.push_back(dest);
                dest.copyTo(dst(cv::Rect(j*depth.cols, i*depth.rows, depth.cols, depth.rows)));
                for (int row = 0; row < depth.rows; row++)
                    for (int col = 0; col < depth.cols; col++)
                        if (maxth.at<double>(row, col) < dest.at<double>(row, col)) maxth.at<double>(row, col) = dest.at<double>(row, col);
            }
        }
    }
}


void FeatureDescriptor::GaborFeatureRandom(Mat& features)
{
    std::random_device rd;
    const double kkernel=3;
    const double winmax=0.5, winmin=0.1;
    double gam=1;
    int lamda=rd()%30;
    double win=(rd()%40+10)*1.0/100;
    double sigma=win*lamda;
    double theta=rd() % 180 * 1.0 / 180 * CV_PI / 2;
    int kernel_size = int(sigma * kkernel) * 2 + 1;
    cv::Size size(kernel_size*2+1, kernel_size*2+1);
    cv::Mat kernel = cv::getGaborKernel(size, sigma, theta, lamda, gam, 0);
    cv::Mat dest;
    cv::filter2D(depth, dest, CV_64F, kernel);
    normalize(dest, dest, -1, 1, 32);
    features=dest;
}


vector<GaborRandPara> FeatureDescriptor::GenerateRandom(int number)
{
    vector<GaborRandPara> grps;
    for (int i=0; i<number;i++){
        std::random_device rd;
        int lamda=rd()%10+1;
        double win=(rd()%40+10)*1.0/100;
        double theta=rd() % 60 * 1.0 / 60 * CV_PI;
        GaborRandPara grp{lamda,win,theta};
        grps.push_back(grp);
    }
    return grps;
}

void FeatureDescriptor::GaborFeature(vector<GaborRandPara> grps, vector<Mat> &features)
{

    const double kkernel=3;
    const double winmax=0.5, winmin=0.1;
    double gam=1;
    for (auto &grp:grps) {
        //cout<<grp.lamda<<" "<<grp.win<<" "<<grp.theta<<endl;
        int lamda=grp.lamda;
        double win=grp.win;
        double theta=grp.theta;
        double sigma = win * lamda;
        int kernel_size = int(sigma * kkernel) * 2 + 1;
        cv::Size size(kernel_size * 2 + 1, kernel_size * 2 + 1);
        cv::Mat kernel = cv::getGaborKernel(size, sigma, theta, lamda, gam, 0);
        cv::Mat dest;
        cv::filter2D(depth, dest, CV_64F, kernel);
        normalize(dest, dest, -1, 1, 32);
        features.push_back(dest);
    }
}

void FeatureDescriptor::ldBaseFT(
        vector<Mat> &features,
        vector<vector<double>> &featureDescriptors,
        std::vector<Bosphouruspoint2d> & landmarks,
        vector<unsigned int> &labels
) {
    const int roiSize = 8;
    for (auto &feature : features) {
        Mat img = depth2normal(feature);
        for (int j = 0; j < landmarks.size(); j++) {
            vector<double> featureDescriptor;
            cv::Point2d center = landmarks[j].pts2d;
            if (center.x - roiSize / 2 > 0 && center.x + roiSize / 2 < img.rows && center.y - roiSize / 2 > 0 &&
                    center.y + roiSize / 2 < img.cols)
            {
                Mat roi = img(cv::Rect(int(center.y - roiSize / 2), int(center.x - roiSize / 2), roiSize, roiSize));

                for (int i1 = 0; i1 < roi.rows; i1++)
                    for (int j1 = 0; j1 < roi.cols; j1++) {
                        featureDescriptor.push_back(roi.at<cv::Vec3d>(i1, j1)[0]);
                        featureDescriptor.push_back(roi.at<cv::Vec3d>(i1, j1)[1]);
                        featureDescriptor.push_back(roi.at<cv::Vec3d>(i1, j1)[2]);
                    }
                featureDescriptors.push_back(featureDescriptor);
                labels.push_back(landmarks[j].labels2d);
            }
        }
    }
}

vector<Mat> FeatureDescriptor::msehLBPfeature(int r,int number)
{
    // getRing
    int height = depth.rows;
    int width = depth.cols;
    cv::Mat dst;

    for (int scale=1; scale<=r; scale++) {
        dst = cv::Mat::zeros(height, width, CV_64FC1);
        for (int j = scale; j < width - scale; j++) {
            for (int i = scale; i < height - scale; i++) {
                int *tmp;
                tmp = new int[number];
                double binarycode = 0;
                for (int k = 0; k < number; k++) {
                    double theta = k * 1.0 / number * CV_PI;
                    int tpoint = depth.at<unsigned short>(int(cos(theta)*scale + i), int(sin(theta)*scale + j));
                    int center = depth.at<unsigned short>(i, j);
                    if (tpoint > center) tmp[k] = 1; else tmp[k] = 0;
                    binarycode = (binarycode + tmp[k]) / 2;
                }
                cout<<binarycode<<" ";
                dst.at<double>(i, j) = binarycode;
            }
        }
        imshow("d",dst);
        imwrite("../data/output/lbp/out_"+to_string(scale)+"_"+to_string(number)+".png",dst*255);
    }

    return vector<Mat>();
}

vector<Mat> FeatureDescriptor::SIFTfeature()
{
    Ptr<xfeatures2d::SIFT>feature = xfeatures2d::SIFT::create();
    vector<KeyPoint>keypoints;
    Mat depth2, temp;
    temp = depth / 255;
    temp.convertTo(depth2, CV_8U);
    feature->detect(depth2, keypoints);
    //int descritor;
    //feature->compute(depth2, keypoints, descritor);
    Mat output;
    drawKeypoints(depth2, keypoints, output);
    cv::imshow("output", output);
    cv::imwrite("output.png", output);

    return vector<Mat>();
}

vector<Myfeature> FeatureDescriptor::GaborFeaturedescriptor(vector<Mat>& filter_imgs){

    vector<Myfeature> dst;
    vector<vector<double>> featureDescriptors;
    vector<unsigned int> labels;
    vector<Bosphouruspoint2d> tp;
    tp=bp.find3dlmOnDepth();
    //cout<<"build director..."<<endl;
    ldBaseFT(filter_imgs,featureDescriptors,tp,labels);
    //cout<<"build director..."<<endl;
    for(int i=0; i<featureDescriptors.size();i++)
    {

        //cout<<"size: "<<featureDescriptors[i].size()<<endl;
        Myfeature feature{featureDescriptors[i],labels[i],bp.getfaceid()};
        dst.push_back(feature);
    }
    //cout<<"build director..."+to_string(dst.size())<<endl;
    return dst;
}




