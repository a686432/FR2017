//
// Created by jdq on 17-12-26.
//

#ifndef FEATUREDESCRIPTION_H
#define FEATUREDESCRIPTION_H


#include "unit.h"
#include "BosphorusDBdriver.h"








class FeatureDescriptor
{



public:
    FeatureDescriptor(BosphorusDBdriver bp);
    ~FeatureDescriptor();


private:
    BosphorusDBdriver bp;
    MyTriMesh mesh;
    Mat depth;
    Mat Mat3D;
    vector<Bosphouruspoint3d> label3dpts;

public:
    void GaborFeature(vector<GaborRandPara> grps, vector<Mat> &features);

    void ldBaseFT(vector<Mat> &features, vector<vector<double>> &featureDescription, std::vector<Bosphouruspoint2d> & landmarks,std::vector<unsigned int>& labels);
    vector<Mat> SIFTfeature();
    void GaborFeature(int scale, int th, int win, vector<Mat> &features);
    vector<Mat> msehLBPfeature(int scale, int number);

    Mat depth2normal(Mat &mat);

    vector<Myfeature> GaborFeaturedescriptor(vector<Mat>& filter_imgs);
    void GaborFeatureRandom(Mat& features);

    static vector<GaborRandPara> GenerateRandom(int number);



};




#endif //FEATUREDESCRIPTION_H
