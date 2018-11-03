//
// Created by cuizhou on 18-7-8.
//

#include <iostream>
#include "../src/myfUtils/FileOperator.h"

#include "opencv2/opencv.hpp"

using namespace std;
int main(){

    char* basePath = "/home/cuizhou/AAAA/ContainerProject/识别样例/aftertrans/part";//detection
    std::vector<std::string> filenames = myf::readFileList(basePath);

    for(auto filnm:filenames){
        string filepath = basePath;
        filepath+="/"+filnm;

        cv::Mat srcImage = cv::imread(filepath);
        cv::Mat GrayImg;
        cv::cvtColor(srcImage,GrayImg,CV_BGR2GRAY);

        cv::Mat Bw;
        cv::threshold(GrayImg, Bw, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);//灰度图像二值化
//        cv::threshold(GrayImg, Bw, 130, 255, CV_THRESH_BINARY);//灰度图像二值化


//        cv::imshow("Bw",Bw);
//        cv::waitKey(0);
        cv::imwrite("/home/cuizhou/AAAA/ContainerProject/识别样例/traditional/"+filnm,Bw);
    }

    return 0;
}


