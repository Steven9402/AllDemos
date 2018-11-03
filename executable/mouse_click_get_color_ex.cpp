//
// Created by cuizhou on 18-3-26.
//
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "mouseUtils/MouseGetPoint.h"

using namespace std;
using namespace cv;

int main(int argc, char* argv[])
{

    Mat frame = imread("../../data/probimgs3/155.jpg");
    getColor(frame, 1.0 ,1);

}
