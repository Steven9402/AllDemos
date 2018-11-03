//
// Created by cuizhou on 18-5-17.
//
#include "../src/myfUtils/FileOperator.h"
using namespace std;
int main(){
    /**
     * 根据一2层文件夹目录，创建相同的2层文件夹目录，并删除第二级内容少于阈值的文集夹
     * 设置： 输入原人脸库路劲(原图) input_root_path
     * 设置: 输出人脸库路径 outpu_root_path
     * 设置: 保留文件夹阈值 thresh
     */
    char* input_root_path = "../../data/test_ori";
    std::string outpu_root_path = "../../data/test_dst";
    int thresh=3;

    std::vector<string> folders;
    std::vector<string> files;

    myf::walk(input_root_path,folders,files);
    myf::createSubFoldersDemo(outpu_root_path,folders);



//    //删除内容少于3的文件夹
//    std::vector<string> folders2;
//    std::vector<string> files2;
//    myf::walk(outpu_root_path.c_str(),folders2,files2);
//
//    for(string folder:folders2) {
//        string sub_input_root = outpu_root_path;
//        sub_input_root += "/" + folder;
//        vector<string> filenames = myf::readFileList(sub_input_root.c_str());
//        if(filenames.size()<thresh){
//            myf::delete_dir(sub_input_root.c_str());
//        }
//    }
}