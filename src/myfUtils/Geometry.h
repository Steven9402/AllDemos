//
// Created by cuizhou on 18-9-14.
//

#ifndef ALLDEMOS_GEOMETRY_H
#define ALLDEMOS_GEOMETRY_H
#include <vector>
#include <opencv2/opencv.hpp>

namespace myf {
    using namespace std;
//    CvPoint2D32f sub(CvPoint2D32f b, CvPoint2D32f a);
//
//    CvPoint2D32f add(CvPoint2D32f b, CvPoint2D32f a);
//
//    CvPoint2D32f mul(CvPoint2D32f b, CvPoint2D32f a);
//
//    CvPoint2D32f mul(CvPoint2D32f b, float t);
//
//    float dot(CvPoint2D32f a, CvPoint2D32f b);
//
////向量长度
//    float dist(CvPoint2D32f v);
//
//    CvPoint2D32f point_on_segment(CvPoint2D32f line0, CvPoint2D32f line1, CvPoint2D32f pt);
//
//    float dist2line(CvPoint2D32f line0, CvPoint2D32f line1, CvPoint2D32f pt);

    /**
     * 求直线长度
     * @param line[0-3]: x1,y1,x2,y2
     * @return
     */
    float lengthLine(const vector<float> &line);

    /**
     * 点相减求向量
     * @param pt1
     * @param pt2
     * @return
     */
    vector<float> ptsub(const vector<float> &pt1, const vector<float> &pt2);

    /**
     * 求两向量叉积
     * @param pt1
     * @param pt2
     * @return
     */
    float vecmul(const vector<float> &pt1, const vector<float> &pt2);


    /**
     * 求点到直线的距离
     * @param line:{x1,y1,x2,y2}
     * @param pt:直线外一点 {x,y}
     * @return
     */
    float dist2line(const vector<float> &line, const vector<float>& pt);
}
#endif //ALLDEMOS_GEOMETRY_H
