//
// Created by jdq on 17-12-26.
//

#include "FeatureSelection.h"

FeatureSelection::FeatureSelection(FeatureDescriptor fd)
{
//this->features = fd.get;
}

FeatureSelection::~FeatureSelection()
{
}

void FeatureSelection::LDA(vector<int> &a)
{
   shark::LDA();

}

ClassificationDataset FeatureSelection::vec2labelData(vector<mylabelData>& a) {
   std::size_t dimensions = a[0].input.size();
   ClassificationDataset dataset;
   const int maximumBatchSize = 256;
   std::vector<std::size_t> batchSizes = shark::detail::optimalBatchSizes(a.size(), maximumBatchSize);
   cout<<"batchSizes: "<< batchSizes[1] <<endl;
   dataset = ClassificationDataset(batchSizes.size());

   std::size_t currentIndex = 0;
   for (std::size_t b = 0; b != batchSizes.size(); ++b) {
      RealMatrix& inputs = dataset.batch(b).input;
      UIntVector& labels = dataset.batch(b).label;
      inputs.resize(batchSizes[b], dimensions);
      labels.resize(batchSizes[b]);
      for (std::size_t i = 0; i != batchSizes[b]; ++i, currentIndex++) {
         for (std::size_t j = 0; j != dimensions; ++j) {
            inputs(i, j) = a[currentIndex].input[j];
            labels[i] = a[currentIndex].label;
         }
      }

   }
   RealMatrix& inputs = dataset.batch(0).input;
   cout<<"batchSizes: "<< inputs(1,1) <<endl;
   cout<<"batchSizes: "<< a[0].input[0] <<endl;
   return dataset;
}

vector<mylabelData> FeatureSelection::loadData(const string &inputpath="../data/input/feature/combine/1.txt")
{
   ifstream in(inputpath, ios::in);
   char tmp;
   int count,dim;
   std::string buff;
   in>>tmp>>count>>dim;
   vector<mylabelData> Mydata;
   cout<<tmp<<"Number of data: "<<count<<" dimension of data: "<<dim<<endl;
   getline(in,buff);
   for (int i=0; i<count; i++) {
      vector<double> vec;
      getline(in,buff);
      istringstream ss(buff);
      cout<<buff<<endl;
      for (int j = 0; j < dim; j++) {
         double a;
         ss >> a;
         vec.push_back(a);
      }
      unsigned int label;
      ss>>label;
      mylabelData mydata{vec,label};
      Mydata.push_back(mydata);
   }
   in.close();

   return Mydata;
}