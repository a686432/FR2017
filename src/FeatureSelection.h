//
// Created by jdq on 17-12-26.
//

#pragma once
#ifndef FEATURESELECTION
#define FEATURESELECTION
#include "unit.h"
#include <shark/Algorithms/Trainers/LDA.h>
#include <shark/Data/Csv.h>
#include <shark/ObjectiveFunctions/Loss/ZeroOneLoss.h>
using namespace shark;
class FeatureSelection
{
public:
    FeatureSelection(FeatureDescriptor fd);
    ~FeatureSelection();

private:

public:
    void LDA(vector<int> &a);
    ClassificationDataset FeatureSelection::vec2labelData(vector<mylabelData>& a);


    vector<mylabelData> loadData(const string &inputpath);
};



#endif
