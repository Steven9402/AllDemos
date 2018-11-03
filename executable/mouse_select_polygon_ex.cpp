//
// Created by cuizhou on 18-3-26.
//
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include "mouseUtils/selectRoiByMouse.h"
#include <iostream>
#include <opencv2/imgproc.hpp>

#define VIDEO
using namespace std;
using namespace cv;

#ifndef VIDEO
int main(int argc, char* argv[])
{
//    string videoPath = "/home/cuizhou/projects/PuFa_Bank_Car_count/CarCount/data/test1_bus.mp4";
    string videoPath = "/home/cuizhou/Documents/ADAS/弈人视频样例/样例1/0000000000000000-171027-093607-095607-000001007520.avi";
//    string videoPath = "/home/cuizhou/Documents/ADAS/弈人视频样例/样例1/0000000000000000-171027-095607-101117-000001007520.avi";
//    string videoPath = "../../data/examples/road-single.avi";
    VideoCapture cap(videoPath);
    Mat frame;
    int crop_count = 0;
    while(cap.read(frame) && crop_count++ < 10) {
        if (frame.empty())
            continue;
    }


    vector<Point> poly;
    Rect boundRect;
    selectPolygonByMouse(frame,1.0,poly,boundRect);
    cout << "Polygon: " << endl;
    for(int i=0; i<poly.size(); ++i)
        cout << "Point(" << poly[i].x << "," << poly[i].y << ")," ;
    cout << endl;
    cout << "BoundRect: " << boundRect << endl;

    vector<Point> polyline;
    drawPolygon(frame, poly);
    selectPolylineByMouse(frame,1.0,polyline);
    cout << "Polyline: " << endl;
    for(int i=0; i<polyline.size(); ++i)
        cout << "Point(" << polyline[i].x << "," << polyline[i].y << ")," ;
    cout << endl;
    imshow("img",frame);
    waitKey(0);

}
#else
int main(int argc, char* argv[])
{

    Mat frame = imread("../../data/22.jpg");

    //new  跟踪停止线属性与位置
    cv::line(frame,cv::Point(1065,999),cv::Point(1983,994),cv::Scalar(255,255,0),5,8);
    //new　停车线
    cv::line(frame,cv::Point(653,1585),cv::Point(2107,1585),cv::Scalar(255,255,0),5,8);

    //跟踪停止线属性与位置
    //31.22
    //             <跟踪停止线属性与位置>1,1165,851,2082,832</跟踪停止线属性与位置>
    //    cv::line(frame,cv::Point(1165,851),cv::Point(2082,832),cv::Scalar(255,255,0),5,8);
    //停车线
    //         <停车线>838,1326,2038,1321</停车线>
    //    cv::line(frame,cv::Point(838,1326),cv::Point(2038,1321),cv::Scalar(255,255,0),5,8);

    //车道
    cv::line(frame,cv::Point(371,2007),cv::Point(843,1324),cv::Scalar(255,0,255),5,8);
    cv::line(frame,cv::Point(843,1324),cv::Point(1423,1324),cv::Scalar(255,0,255),5,8);
    cv::line(frame,cv::Point(1423,1324),cv::Point(1296,1999),cv::Scalar(255,0,255),5,8);

    cv::line(frame,cv::Point(1292,1995),cv::Point(1428,1328),cv::Scalar(255,0,255),5,8);
    cv::line(frame,cv::Point(1428,1328),cv::Point(2031,1333),cv::Scalar(255,0,255),5,8);
    cv::line(frame,cv::Point(2031,1333),cv::Point(2249,2007),cv::Scalar(255,0,255),5,8);

    //行人检测区域
    cv::rectangle(frame,cv::Rect(374,873,1950,419),Scalar(0,255,0),5,8);
    //车辆检测区域
    cv::rectangle(frame,cv::Rect(379,1093,1929,910),Scalar(0,0,255),5,8);

    bool need_resize = false;
    float ratio = 1500.0/frame.cols;
    if(frame.cols>1500){
        need_resize = true;
        int newrows = ratio*frame.rows;
        resize(frame,frame,Size(1500,newrows));
    }

    vector<Point> poly;
    Rect boundRect;
    selectPolygonByMouse(frame,1.0,poly,boundRect);
    cout << "Polygon: " << endl;
    for(int i=0; i<poly.size(); ++i)
        cout << "Point(" << 1.0/ratio*poly[i].x << "," << 1.0/ratio*poly[i].y << ")," ;
    cout << endl;
    cout << "BoundRect: " << (int)(1.0/ratio*boundRect.x)<<" "<<(int)(1.0/ratio*boundRect.y)<<" "<<(int)(1.0/ratio*boundRect.width)<<" "<<(int)(1.0/ratio*boundRect.height) << endl;

    vector<Point> polyline;
    drawPolygon(frame, poly);
    selectPolylineByMouse(frame,1.0,polyline);
    cout << "Polyline: " << endl;
    for(int i=0; i<polyline.size(); ++i)
        cout << "Point(" << 1.0/ratio*polyline[i].x << "," << 1.0/ratio*polyline[i].y << ")," ;
    cout << endl;
    imshow("img",frame);
    waitKey(0);

}
#endif