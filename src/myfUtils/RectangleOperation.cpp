//
// Created by cuizhou on 18-2-25.
//

#include <iostream>
#include "RectangleOperation.h"
namespace  myf {

    double calcIouRatio(const Rect &rect1, const Rect &rect2) {

        //''' Rect = [x1, y1, x2, y2] '''
        int x1 = rect1.x;
        int y1 = rect1.y;
        int width1 = rect1.width;
        int height1 = rect1.height;

        int x2 = rect2.x;
        int y2 = rect2.y;
        int width2 = rect2.width;
        int height2 = rect2.height;

        int endx = max(x1 + width1, x2 + width2);
        int startx = min(x1, x2);
        int width = width1 + width2 - (endx - startx);

        int endy = max(y1 + height1, y2 + height2);
        int starty = min(y1, y2);
        int height = height1 + height2 - (endy - starty);

        double ratio = 0;
        if (width <= 0 || height <= 0) {
            ratio = 0;
        } else {
            double area = width * height;
            double area1 = width1 * height1;
            double area2 = width2 * height2;
            ratio = area * 1.0 / (area1 + area2 - area);
        }
        return ratio;
    }

    double calcOverlapRatio(const Rect &rect1, const Rect &rect2) {

        //''' Rect = [x1, y1, x2, y2] '''
        int x1 = rect1.x;
        int y1 = rect1.y;
        int width1 = rect1.width;
        int height1 = rect1.height;

        int x2 = rect2.x;
        int y2 = rect2.y;
        int width2 = rect2.width;
        int height2 = rect2.height;

        int endx = max(x1 + width1, x2 + width2);
        int startx = min(x1, x2);
        int width = width1 + width2 - (endx - startx);

        int endy = max(y1 + height1, y2 + height2);
        int starty = min(y1, y2);
        int height = height1 + height2 - (endy - starty);

        double ratio = 0;
        if (width <= 0 || height <= 0) {
            ratio = 0;
        } else {
            double area = width * height;
            double area1 = width1 * height1;
            double area2 = width2 * height2;
            ratio = (double) area / min(area1, area2);
        }
        return ratio;
    }

    cv::Rect getSafeROI(int x1,int y1,int x2,int y2,int rows,int cols){
        if(rows>0&&cols>0&&x2-x1>0&&y2-y1>0
           &&x1>=0&&x2>=0){
            if(x1>cols-1)x1=cols-1;
            if(y1>rows-1)y1=rows-1;

            if(x2>cols-1)x2=cols-1;
            if(y2>rows-1)y2=rows-1;

            cv::Rect result(cv::Point(x1,y1),cv::Point(x2,y2));
            return result;
        }else{
            cv::Rect tmp;
            return tmp;
        }
    }


    bool comparison_area(Rect a,Rect b){
        //>是降序
        return a.area()>b.area();
    }

    void nms(vector<Rect>& rects,float thresh){
        sort(rects.begin(),rects.end(),comparison_area);

        if(rects.size()>1){
            int ind_i=0;
            int ind_j=1;

            while(ind_i<rects.size()-1){
                while(ind_j<rects.size()){
                    float iou_ratio=myf::calcIouRatio(rects[ind_i],rects[ind_j]);
                    if(iou_ratio>thresh){
                        rects.erase(rects.begin()+ind_j);
                        ind_j--;
                    }
                    ind_j++;
                }
                ind_i++;
            }

        }
    }
}