//
// Created by cuizhou on 18-9-14.
//
#include "myfUtils/LineDetector.h"

int main(){
    VideoCapture capture("/home/cuizhou/AAAA/AndroidFACE/data/9/real.mp4");
    //capture.set(CV_CAP_PROP_FPS,70);

    LineDetectorFactory line_detector_factory;
    // 1 for lsd
    // 2 for hough
    LineDetector* line_detector = line_detector_factory.createLineDetector(2);
    Mat frame;
    while(capture.read(frame)) {
        if (frame.empty()) continue;
        vector<Vec4f> lines = line_detector->detect(frame);
        line_detector->drawLines(frame,lines);
    }
}
