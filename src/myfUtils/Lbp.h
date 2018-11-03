//
// Created by cuizhou on 18-7-31.
//

#ifndef ALLDEMOS_LBP_H
#define ALLDEMOS_LBP_H

#include <opencv2/opencv.hpp>

#include <iostream>
#include <fstream>
#include <sstream>

using namespace cv;
using namespace std;

namespace myf {
//elbp是圆形算子LBP函数
    void elbp(Mat &src, Mat &dst, int radius, int neighbors);

//elbp1是通常LBP图
    void elbp1(Mat &src, Mat &dst);
}
#endif //ALLDEMOS_LBP_H
