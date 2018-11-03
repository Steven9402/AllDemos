//
// Created by cuizhou on 18-9-14.
//
#include <myfUtils/FileOperator.h>
#include "myfUtils/RectangleDetector.h"

#include <iostream>
#include <chrono>
#include "glog/logging.h"

using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;

#define TEST01 // 基于直线检测的矩形检测
#ifdef TEST01
int main(){
    google::InitGoogleLogging("test");
    google::InstallFailureSignalHandler();

    RectangleDetector rectangle_detector;

    VideoCapture capture("/home/cuizhou/AAAA/AndroidFACE/data/0/real.mp4");
    capture.set(CV_CAP_PROP_FPS,6000);

    Mat frame;
    while(capture.read(frame)) {
        if (frame.empty()) continue;

//        vector<pairs> pairs_vertical = rectangle_detector.detectVerticalLines(frame);
//
//        for(int i=0;i<pairs_vertical.size();i++){
//            cv::Mat show_mat=frame.clone();
//
//            line(show_mat,Point(pairs_vertical[i].line_a[0],pairs_vertical[i].line_a[1]),
//                 Point(pairs_vertical[i].line_a[2],pairs_vertical[i].line_a[3]),Scalar(255-i*50%255,i*50%255,0),i+5,8,0);
//            line(show_mat,Point(pairs_vertical[i].line_b[0],pairs_vertical[i].line_b[1]),
//                 Point(pairs_vertical[i].line_b[2],pairs_vertical[i].line_b[3]),Scalar(255-i*50%255,i*50%255,0),i+5,8,0);
//            resize(show_mat,show_mat,Size(0,0),0.5,0.5);
//            imshow("show_mat",show_mat);
//            waitKey(0);
//        }

//        vector<pairs> pairs_parallel = rectangle_detector.detectParallelLines(frame);
//
//        for(int i=0;i<pairs_parallel.size();i++){
//            cv::Mat show_mat=frame.clone();
//
//            line(show_mat,Point(pairs_parallel[i].line_a[0],pairs_parallel[i].line_a[1]),
//                 Point(pairs_parallel[i].line_a[2],pairs_parallel[i].line_a[3]),Scalar(255-i*50%255,i*50%255,0),i+5,8,0);
//            line(show_mat,Point(pairs_parallel[i].line_b[0],pairs_parallel[i].line_b[1]),
//                 Point(pairs_parallel[i].line_b[2],pairs_parallel[i].line_b[3]),Scalar(255-i*50%255,i*50%255,0),i+5,8,0);
//            resize(show_mat,show_mat,Size(0,0),0.5,0.5);
//            imshow("show_mat",show_mat);
//            waitKey(0);
//        }

        high_resolution_clock::time_point beginTime = high_resolution_clock::now();
        vector<Rect> my_rects = rectangle_detector.detect(frame);
        high_resolution_clock::time_point endTime = high_resolution_clock::now();
        milliseconds timeInterval = std::chrono::duration_cast<milliseconds>(endTime - beginTime);
        std::cout << timeInterval.count() << "ms\n";
        for(int i=0;i<my_rects.size();i++){
            rectangle(frame,my_rects[i],Scalar(255,255,0),1,8,0);
        }
        resize(frame,frame,Size(0,0),0.5,0.5);
        imshow("frame",frame);
        waitKey(26);

    }

}
#endif

//#define TEST02 //图像ｄｅｍｏ
#ifdef TEST02

int main(){

    RectangleDetector2 rectangle_detector;

    std::string dir = "../../data/test_rect";
    std::vector<string> filepath = myf::readFileList(dir.c_str());


    for(string filenm:filepath){

        filenm=dir+"/"+filenm;
        Mat srcImage = imread(filenm);

        if( srcImage.empty() )
        {
            cout << "Couldn't load " << filenm << endl;
            continue;
        }


        high_resolution_clock::time_point beginTime = high_resolution_clock::now();
        vector<vector<Point> > squares;
        rectangle_detector.FindSquaresSingleChannel(srcImage, squares);

        high_resolution_clock::time_point endTime = high_resolution_clock::now();
        milliseconds timeInterval = std::chrono::duration_cast<milliseconds>(endTime - beginTime);
        std::cout << timeInterval.count() << "ms\n";

        rectangle_detector.DrawSquares(srcImage, squares);
//        rectangle_detector.DrawSquares(srcImage, colorSquares);

        int c = waitKey();
        if( (char)c == 27 )
            break;

    }

}
#endif

//#define TEST03
#ifdef TEST03
int main(){
    RectangleDetector2 rectangle_detector;

    VideoCapture capture("/home/cuizhou/AAAA/AndroidFACE/data/9/fake.mp4");
    capture.set(CV_CAP_PROP_FPS,6000);

    Mat frame;
    while(capture.read(frame)) {
        if (frame.empty()) continue;

        high_resolution_clock::time_point beginTime = high_resolution_clock::now();

//        vector<vector<Point> > squares;
//        vector<vector<Point> > colorSquares[3];
//        rectangle_detector.FindSquaresMultiChannel(frame, squares, colorSquares);

        vector<vector<Point> > squares;
        rectangle_detector.FindSquaresSingleChannel(frame, squares);

        high_resolution_clock::time_point endTime = high_resolution_clock::now();
        milliseconds timeInterval = std::chrono::duration_cast<milliseconds>(endTime - beginTime);
        std::cout << timeInterval.count() << "ms\n";

        rectangle_detector.DrawSquares(frame, squares);
//        rectangle_detector.DrawSquares(frame, colorSquares);

        int c = waitKey();
        if( (char)c == 27 )
            break;

    }

}
#endif



