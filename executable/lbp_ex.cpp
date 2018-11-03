//
// Created by cuizhou on 18-7-31.
//

#include "../src/myfUtils/Lbp.h"

int main(){
    cv::Mat srcImage=cv::imread("../../data/timg2.jpg",CV_LOAD_IMAGE_GRAYSCALE);
    cv::Mat lbp_result=cv::Mat::zeros(srcImage.rows,srcImage.cols,CV_8UC1);
    myf::elbp(srcImage,lbp_result,1,8);
    cv::imshow("lbpresult",lbp_result);
    cv::waitKey(0);

}