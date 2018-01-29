#include "unit.h"

string test_fold = "../data/";
string input_fold = test_fold + "input/";
string output_fold = test_fold + "output/";
std::string Database_root="/home/jdq/database/BosphorusDB";
#define TEST2



void inputtest()
{

      BosphorusDBdriver bdriver{input_fold+"1"};
      Mat depthmap = bdriver.getdepthmap();
      imshow("depthMap", depthmap);
      waitKey(0);



      //// Test the local feature discription
      /*vector<Bosphouruspoint2d> lm3D=bdriver.find3dlmOnDepth();
      for (auto &i : lm3D) {
          cout<< i.pts2d<<endl;
          cout<< i.pts2d.x<<" "<< i.pts2d.y<<endl;
          depthmap.at<unsigned short>(int(i.pts2d.x),int(i.pts2d.y))=0;
      }
      imshow("depthMap", depthmap);
      waitKey(0);
      for (auto &i : lm3D) {
          Mat B = depthmap(Rect(int(i.pts2d.y), int(i.pts2d.x), 5, 5));
          B = 0;
      }
      imshow("depthMap", depthmap);
      waitKey(0);*/



      ////Test the feature discroptor


      FeatureDescriptor fdescription{ bdriver };
      vector<cv::Mat> filtered_imgs;
      vector<GaborRandPara> grps=FeatureDescriptor::GenerateRandom(5);
      fdescription.GaborFeature(grps,filtered_imgs);
      for (auto d:filtered_imgs) {
          Mat a=fdescription.depth2normal(d);
          imshow("aa",a);
          waitKey(0);
      }
      vector<Myfeature> Gaborfeature;
      Gaborfeature=fdescription.GaborFeaturedescriptor(filtered_imgs);
      waitKey(0);


      // te
//    //Test the mesh
//    MyTriMesh depthRecMesh = bdriver.getMesh();
//    if (!OpenMesh::IO::write_mesh(depthRecMesh, (output_fold+"1.obj").c_str()))
//        std::cerr << "Cannot write mesh to file: " << "1.obj" << std::endl;
//    else
//        std::cout << "successfully save mesh" << std::endl;
}
void featuretest()
{

    ////generate the feature
//    std::random_device rd;
//    int lamda=rd()%30;
//    double win=(rd()%40+10)*1.0/100;
//    double theta=rd() % 180 * 1.0 / 180 * CV_PI / 2;
//    DataManage datamanage;
//    array<vector<std::string>,105> bpfiles=datamanage.bpdatabase_read(Database_root);
//    vector<GaborRandPara> grps=FeatureDescriptor::GenerateRandom(5);
//    for (int i=0; i<bpfiles.size();i++)
//        for (int j=0; j<bpfiles[i].size();j++) {
//            cout << bpfiles[i][j] << endl;
//            BosphorusDBdriver bdriver{bpfiles[i][j]};
//            bdriver.setfaceid(i);
//            FeatureDescriptor fdescription{ bdriver };
//            vector<cv::Mat> filtered_imgs;
//            fdescription.GaborFeature(grps,filtered_imgs);
//            cout<<"Get feature description..."<<endl;
//            vector<Myfeature> Gaborfeature;
//            Gaborfeature=fdescription.GaborFeaturedescriptor(filtered_imgs);
//            string outputpath="../data/output/feature/";
//            if (!fs::exists(outputpath))
//                fs::create_directory(outputpath);
//            cout<<"writing the " << outputpath+to_string(i)+"_"+to_string(j) <<endl;
//            datamanage.writemyfeatureVec(Gaborfeature,outputpath+to_string(i)+"_"+to_string(j)+".txt");
//        }
//      cv::waitKey();
        ////reorganize the feature
        DataManage datamanage;
        datamanage.CombineVector();


}


void test_main()
{
#ifdef TEST1
    inputtest();
#else TEST2
    featuretest();
#endif
}

int main()
{
    test_main();
    return 0;
}
