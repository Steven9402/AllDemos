//
// Created by cuizhou on 18-4-5.
//

#ifndef CUIZHOUDETECTLANE_MORPH_H
#define CUIZHOUDETECTLANE_MORPH_H

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
namespace myf {
    using namespace cv;

    void erosion(const Mat &src, Mat &erosion_dst, int erosion_size, int erosion_type);

    void Dilation(const Mat &src, Mat &dilation_dst, int dilation_size, int dilation_type);


}
#endif //CUIZHOUDETECTLANE_MORPH_H
