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

    bool expandRoi(int expandwidth,int expandheight,const cv::Rect& roi,cv::Rect& expandedROI,int matwidth,int matheight);

    //旋转整张mat
    cv::Mat rotate(const cv::Mat& srcImage,double degree);
    //旋转mat内的rect部分
    cv::Mat rotate(const cv::Mat& srcImage,const cv::Rect&,double degree);
    /*
     * 旋转之后保留黑边 ,rgb图
     */
    cv::Mat rotateAdvanced(const cv::Mat& srcImage,double degree);

    //线性拉升 均值->120
    cv::Mat linearLighten(const cv::Mat& srcImage);

    /*
     * 旋转点。此函数与 上面的rotate配合使用。
     * x,y  ： 是点在原图像中坐标
     * theta： 是逆时针旋转角度
     * s0   ： 是原图像中心点
     * s1   ： 是旋转后图像中心点
     */
    cv::Point rotatePoint(float x,float y,float theta,cv::Point s0,cv::Point s1);
}

#endif //MSER_MYMATOPERATION_H
