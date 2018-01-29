//
// Created by jdq on 17-12-26.
//

#pragma once
#ifndef UNIT_H
#define UNIT_H

#include <string>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>
#include <boost/filesystem.hpp>
#include <random>
#define _USE_MATH_DEFINES
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
typedef OpenMesh::TriMesh_ArrayKernelT<>  MyTriMesh;
#ifdef _DEBUG
#pragma comment(lib, "OpenMeshCored.lib")
#pragma comment(lib, "OpenMeshToolsd.lib")
#else
#pragma comment(lib, "OpenMeshCore.lib")
#pragma comment(lib, "OpenMeshTools.lib")
#endif // _DEBUG

using namespace std;
using namespace cv;
using namespace OpenMesh;
namespace fs=boost::filesystem;

struct Myfeature{
    vector<double> feature;
    unsigned int landmarks_id;
    unsigned int face_id;
};

struct GaborRandPara{
    int lamda;
    double win;
    double theta;
};

struct mylabelData{
    vector<double> input;
    unsigned int label;
} ;

#include "BosphorusDBdriver.h"
#include "featureDescription.h"
#include "DataManage.h"



#endif // !UNIT_H


