//
// Created by root on 18-1-18.
//

#ifndef FR2017_DATAMANAGE_H
#define FR2017_DATAMANAGE_H


#include "featureDescription.h"




class DataManage {

public:
    DataManage(){};
    ~DataManage(){};
private:
    struct BosphourusFileList
    {
        std::list<std::list<string>> filename;
        std::string rootdict;
    } bfl;
public:
    array<vector<string>,105> bpdatabase_read(string rootdict);
    void writemyfeatureVec(vector<Myfeature> fts,string outpath);
    void CombineVector();
};


#endif //FR2017_DATAMANAGE_H
