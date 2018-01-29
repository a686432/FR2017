//
// Created by root on 18-1-18.
//

#include "DataManage.h"

array<vector<string>,105> DataManage::bpdatabase_read(string Database_root){
    //for (auto& p : fs::directory_iterator(Database_root));

    array<vector<string>,105> bpfiles;
    if (fs::exists(Database_root)) {
        fs::directory_iterator item_begin(Database_root);
        fs::directory_iterator item_end;
        for (; item_begin != item_end; ++item_begin) {
            if (fs::is_directory(*item_begin)) {
                fs::directory_iterator item_begin2(item_begin->path());
                fs::directory_iterator item_end2;
                int faceid=std::stoi(item_begin->path().native().substr(item_begin->path().native().find_last_of('/')+3));
                vector<string> temp;
                for (; item_begin2 != item_end2; ++item_begin2) {
                    if (item_begin2->path().native().substr(item_begin2->path().native().find_last_of('.'))==".bnt")
                        temp.push_back(item_begin2->path().native().substr(0,item_begin2->path().native().find_last_of('.')));
                }
                bpfiles[faceid]=temp;
            }
        }
    }
    return bpfiles;
}

void DataManage::writemyfeatureVec(vector<Myfeature> fts, string outpath) {
    ofstream out(outpath, ios::out);
    if (out.is_open())
    {

        out << "# "<<fts.size()<<endl;
        for (auto &ft : fts) {
            out << "* " << ft.face_id << " " << ft.landmarks_id << " " << ft.feature.size() << endl;
            for (double j : ft.feature) {
                out<< j <<" ";
            }
            out <<endl;
        }
        out.close();
    }
    else
        cout<<"a"<<endl;
}

void DataManage::CombineVector() {
    string outputpath = "../data/output/feature/combine/";
    string inputpath = "../data/input/feature/";
    if (!fs::exists(outputpath))
        fs::create_directory(outputpath);
    array<ofstream, 22> outs;
    vector<string> filelists;
    if (fs::exists(outputpath)) {
        fs::directory_iterator item_begin(inputpath);
        fs::directory_iterator item_end;
        for (; item_begin != item_end; ++item_begin) {
            filelists.push_back(item_begin->path().native());
        }
    }
    cout<<"The file number: "<<filelists.size()<<endl;
    for (auto &filepath:filelists) {
        ifstream in(filepath, ios::in);
        if (in.is_open()) {
            char a;
            int number;
            in >> a >> number;
            cout << "a: " << a << "number: " << number << endl;
            for (int i = 0; i < number; i++) {
                char b;
                unsigned int face_id, count, lmid;
                vector<double> features;
                in >> b >> face_id >> lmid >> count;
                cout << "b: " << b << "face_id: " << face_id << "count: " << count << endl;
                for (int k = 0; k < count; k++) {
                    double temp;
                    in >> temp;
                    features.push_back(temp);
                }
                mylabelData mydata{features,face_id};
                string outfile = outputpath + to_string(lmid) + ".txt";
                ofstream out(outfile, ios::out | ios::app);
                if (out.is_open()) {
                    out<<"# "<<mydata.label<<" "<<mydata.input.size()<<endl;
                    for (auto feature:mydata.input)
                        out << feature << " ";
                    out << endl;
                }
                out.close();
            }
        }
        else
            cout<<"can't open input file!"<<endl;

    }
}


//    for (int i=0;i<22;i++)
//        outs[i].close();
//
