//
// Created by cuizhou on 18-9-14.
//

#include "Geometry.h"

namespace myf {

//    CvPoint2D32f sub(CvPoint2D32f b, CvPoint2D32f a) { return cvPoint2D32f(b.x - a.x, b.y - a.y); }
//
//    CvPoint2D32f mul(CvPoint2D32f b, CvPoint2D32f a) { return cvPoint2D32f(b.x * a.x, b.y * a.y); }
//
//    CvPoint2D32f add(CvPoint2D32f b, CvPoint2D32f a) { return cvPoint2D32f(b.x + a.x, b.y + a.y); }
//
//    CvPoint2D32f mul(CvPoint2D32f b, float t) { return cvPoint2D32f(b.x * t, b.y * t); }
//
//    float dot(CvPoint2D32f a, CvPoint2D32f b) { return (b.x * a.x + b.y * a.y); }
//
//    float dist(CvPoint2D32f v) { return sqrtf(v.x * v.x + v.y * v.y); }
//
//    CvPoint2D32f point_on_segment(CvPoint2D32f line0, CvPoint2D32f line1, CvPoint2D32f pt) {
//        CvPoint2D32f v = sub(pt, line0);
//        CvPoint2D32f dir = sub(line1, line0);
//        float len = dist(dir);
//        float inv = 1.0f / (len + 1e-6f);
//        dir.x *= inv;
//        dir.y *= inv;
//
//        float t = dot(dir, v);
//        if (t >= len) return line1;
//        else if (t <= 0) return line0;
//
//        return add(line0, mul(dir, t));
//    }
//
//    float dist2line(CvPoint2D32f line0, CvPoint2D32f line1, CvPoint2D32f pt) {
//        return dist(sub(point_on_segment(line0, line1, pt), pt));
//    }

    float lengthLine(const vector<float> &line){
        return sqrtf(pow(line[2] - line[0],2) + pow(line[3]-line[1],2));
    }

    vector<float> ptsub(const vector<float> &pt1, const vector<float> &pt2) {
        return {pt1[0] - pt2[0], pt1[1] - pt2[1]};
    }

    float vecmul(const vector<float> &pt1, const vector<float> &pt2) {
        float result = pt1[0] * pt2[1] - pt1[1] * pt2[0];
        return fabs(result);
    }

    //line: {x1,y1,x2,y2}
    //pt: 直线外一点 {x,y}
    float dist2line(const vector<float> &line, const vector<float>& pt) {
        //直线上单位向量
        vector<float> vec_dir = ptsub({line[0], line[1]}, {line[2], line[3]});
        float len = sqrtf(vec_dir[0] * vec_dir[0] + vec_dir[1] * vec_dir[1]);
        float inv = 1.0f / (len + 1e-6f);
        vec_dir[0] *= inv;
        vec_dir[1] *= inv;

        //点到直线上一端点的向量
        vector<float> vec_pt = ptsub({line[0], line[1]}, pt);

        //点向量和单位向量叉积
        float dist = vecmul(vec_dir, vec_pt);
        return dist;
    }

}