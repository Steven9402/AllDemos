//
// Created by cuizhou on 18-8-1.
//
#include <iostream>
#include <string>

#include "boost/filesystem.hpp"
#include "myfUtils/FileOperator.h"

using namespace std;
namespace bfs = boost::filesystem;

int main(int argc, char* argv[])
{
    /**
     * 划分训练/测试集
     */
    char* input_root_path = "/media/cuizhou/E861-628E/kaggle/new_train"; //step1: 设置训练&测试集路径
    std::string output_root_path_train = "/media/cuizhou/E861-628E/kaggle/new_train_01";//step2: 设置训练集
    std::string output_root_path_test = "/media/cuizhou/E861-628E/kaggle/new_val_01";//step3: 设置测试集

    float train_ratio=0.8; //step4: 设置train val 比例。8:2

    std::vector<string> folders;
    std::vector<string> files;
    myf::walk(input_root_path,folders,files);

    myf::createSubFoldersDemo(output_root_path_train,folders);
    myf::createSubFoldersDemo(output_root_path_test,folders);

    for(string folder:folders){
        string sub_input_root = input_root_path;
        sub_input_root+="/"+folder;
        vector<string> filenames = myf::readFileList(sub_input_root.c_str());

        if(filenames.size()<2)continue;// caution

        int test_count=(1-train_ratio)*filenames.size();

        for(int i=0;i<filenames.size();i++){
            string filename = filenames[i];
            string image_input_path = sub_input_root+"/"+filename;//图片输入路径
            if(i<filenames.size()-test_count){
                //作为train
                string image_output_path=output_root_path_train+"/"+folder+"/"+filename;//图片输出路径
                bfs::copy_file(image_input_path,image_output_path,bfs::copy_option::overwrite_if_exists);
            }else{
                //最为test
                string image_output_path=output_root_path_test+"/"+folder+"/"+filename;//图片输出路径
                bfs::copy_file(image_input_path,image_output_path,bfs::copy_option::overwrite_if_exists);
            }
        }
    }

    return 0;

}
