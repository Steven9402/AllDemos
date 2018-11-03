//
// Created by cuizhou on 18-9-14.
//

#include "LineDetector.h"

void LineDetector::drawLines(cv::Mat&frame,vector<Vec4f>& lines)
{

    for (size_t i = 0; i < lines.size(); i++) {
        Vec4f l = lines[i];
        line(frame, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(23, 180, 55), 1, CV_AA);
    }
    imshow("【效果图】", frame);
    waitKey(0);
}
vector<Vec4f> LSDLineDetector::detect(const cv::Mat& frame){
    vector<Vec4f> lines_lsd;
    Mat grayMat;
    cvtColor(frame, grayMat, CV_BGR2GRAY);
    _lsd_detector_ptr->detect(grayMat, lines_lsd);
    return lines_lsd;
}

void LSDLineDetector::drawLines(cv::Mat&frame,vector<Vec4f>& lines_lsd){
    _lsd_detector_ptr->drawSegments(frame,lines_lsd);
    resize(frame,frame,Size(0,0),0.5,0.5);
    imshow("frame",frame);
    waitKey(0);
}


vector<Vec4f> HoughLineDetector::detect(const cv::Mat& frame){

    Mat grayMat;
    cvtColor(frame, grayMat, CV_BGR2GRAY);
    GaussianBlur(grayMat, grayMat, Size(5, 5), 1.5);

    Mat result_mask;
    Canny(grayMat,result_mask, 50, 200);//进行一次canny边缘检测
    //调用一次回调函数，调用一次HoughLinesP函数
    vector<Vec4f> lines_hough;
    HoughLinesP(result_mask, lines_hough, 1, CV_PI / 180, 80,//投票阈值
                50, //直线最短长度
                10);//两直线之间的最大距离

    return lines_hough;
}

LineDetector* LineDetectorFactory::createLineDetector(int flag){
    switch (flag){
        case 1:
        {
            return new LSDLineDetector;
        }
        case 2:
        {
            return new HoughLineDetector;
        }
        default:
            break;

    }
}