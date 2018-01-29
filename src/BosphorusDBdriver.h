//
// Created by jdq on 17-12-26.
//

#ifndef BOSPHORUSDBDRIVER_H
#define BOSPHORUSDBDRIVER_H

#include "unit.h"


struct Bosphouruspoint2d
{
    unsigned int labels2d;
    cv::Point2d pts2d;
};

struct Bosphouruspoint3d
{
    unsigned int labels3d;
    cv::Point3d pts3d;
};

class BosphorusDBdriver {
public:
    // Use the filename to creat the bosphorusDBdriver class
    BosphorusDBdriver(string filepath);

    BosphorusDBdriver();

    ~BosphorusDBdriver();
private:
    unsigned int faceID;
    double **data;
    double zmin;
    uint16_t nrows;
    uint16_t ncols;
    uint32_t len2;
    char* imfile;
    int landmark2dn;
    int landmark3dn;
    std::vector<std::string> labels2d;
    std::vector<cv::Point2d> pts2d;
    std::vector<std::string> labels3d;
    std::vector<cv::Point3d> pts3d;

private:
    static bool uperTriangleValid(cv::Mat mask, const uint16_t row, const uint16_t col);
    static bool lowerTriangleValid(cv::Mat mask, const uint16_t row, const uint16_t col);
public:
    map<string,unsigned int> label2point{
            {"Outer left eyebrow\r",0},
            {"Middle left eyebrow\r",1},
            {"Inner left eyebrow\r",2},
            {"Inner right eyebrow\r",3},
            {"Middle right eyebrow\r",4},
            {"Outer right eyebrow\r",5},
            {"Outer left eye corner\r",6},
            {"Inner left eye corner\r",7},
            {"Inner right eye corner\r",8},
            {"Outer right eye corner\r",9},
            {"Nose saddle left\r",10},
            {"Nose saddle right\r",11},
            {"Left nose peak\r",12},
            {"Nose tip\r",13},
            {"Right nose peak\r",14},
            {"Left mouth corner\r",15},
            {"Upper lip outer middle\r",16},
            {"Right mouth corner\r",17},
            {"Upper lip inner middle\r",18},
            {"Lower lip inner middle\r",19},
            {"Lower lip outer middle\r",20},
            {"Chin middle\r",21}
    };
    /*
    In the public function, we want to get the  raw data, mesh, depth map, landmarks and cloud point
    depth map: the out put is opencv Mat
    */
    double** getdata() const { return data; }
    int getlandmark3dn() const { return landmark3dn; }
    std::vector<cv::Point3d> getpts3d() const { return pts3d; }

    unsigned int getfaceid() const { return faceID;}
    void setfaceid(int faceid)  { faceID=faceid;}
    cv::Mat getdepthmap();
    MyTriMesh getMesh();
    vector<Bosphouruspoint2d> find3dlmOnDepth();
    Mat get3dpointMat();
    std::vector<Bosphouruspoint2d> getBosphouruspoint2d();
    std::vector<Bosphouruspoint3d> getBosphouruspoint3d();
    //void read_bntfile(const char* filepath, double** &data, double* zmin, uint16_t*  nrows, uint16_t* ncols, char *imfile);
};


#endif //FR2017_BOSPHORUSDBDRIVER_H
