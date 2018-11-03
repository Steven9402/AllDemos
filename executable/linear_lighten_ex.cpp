//
// Created by cuizhou on 18-5-21.
//
#include <opencv2/opencv.hpp>
#include "../src/myfUtils/MatOperation.h"
#include "../src/myfUtils/FileOperator.h"
using namespace std;
using namespace cv;

int main(){

    const char* path = "../../data/18";
    vector<string> lists=myf::readFileList(path);
    for(string filenm:lists){
        string filepath=path;
        filepath=filepath+"/"+filenm;

        cv::Mat srcImage=imread(filepath,CV_LOAD_IMAGE_GRAYSCALE);

        if(srcImage.cols>600){
            int new_y=600.0/srcImage.cols*srcImage.rows;
            resize(srcImage,srcImage,Size(600,new_y));
        }

        Mat lightened_mat=myf::linearLighten(srcImage);

        imshow("srcImage",srcImage);

        imshow("lightened_mat",lightened_mat);
        waitKey(0);
    }

}
