//
// Created by cuizhou on 18-9-14.
//

#ifndef ALLDEMOS_LINEDETECTOR_H
#define ALLDEMOS_LINEDETECTOR_H

#include "opencv2/opencv.hpp"
//#include "opencv2/imgproc.hpp"
#include <vector>

using namespace std;
using namespace cv;
class LineDetector{
public:
//    LineDetector();
    virtual vector<Vec4f> detect(const Mat&frame)=0;

    void drawLines(cv::Mat&frame,vector<Vec4f>& lines_lsd);
};

class LSDLineDetector:public LineDetector {
public:
//    LSDLineDetector();
    vector<Vec4f> detect(const cv::Mat& frame);
    void drawLines(cv::Mat&frame,vector<Vec4f>& lines_lsd);
private:
    Ptr<LineSegmentDetector> _lsd_detector_ptr = createLineSegmentDetector(LSD_REFINE_ADV,0.8, 0.6,2.0, 22.5,0,0.7,1024);

};

class HoughLineDetector:public LineDetector{
public:
    vector<Vec4f> detect(const cv::Mat& frame);
private:
};

class LineDetectorFactory{
public:
    LineDetector* createLineDetector(int flag);
};

#endif //ALLDEMOS_LINEDETECTOR_H
