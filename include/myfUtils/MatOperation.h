//
// Created by cuizhou on 18-2-24.
//

#ifndef MSER_MYMATOPERATION_H
#define MSER_MYMATOPERATION_H

#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;
using namespace std;

namespace myf{
    /**
    * 横向组合成一张mat，rows取决于第一张mat。
    * @param matpool
    * @return
    */
    cv::Mat composeRGBVisMat(const std::vector<Mat>& matpool);

    cv::Mat composeGrayVisMat(std::vector<Mat> matpool);

    bool expandRoi(int expandwidth,const cv::Rect& roi,cv::Rect& expandedROI,int matwidth,int matheight);

    cv::Mat rotate180(const cv::Mat& srcImage);

}

#endif //MSER_MYMATOPERATION_H
