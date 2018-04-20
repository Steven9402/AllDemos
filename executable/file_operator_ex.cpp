//
// Created by cuizhou on 18-3-19.
//
#include <iostream>
#include "myfUtils/FileOperator.h"


using namespace std;
int main(){

    char* basePath = "/home/cuizhou/myGitRepositories/dlibExamples/data/facepool";//detection
    std::vector<std::string> folders;
    std::vector<std::string> files;

    myf::walk(basePath, folders, files);

    cout<<"hello world!"<<endl;
}
