//
// Created by cuizhou on 18-2-24.
//

#include "myfUtils/MatOperation.h"
namespace  myf {

    cv::Mat composeRGBVisMat(const std::vector<Mat>& matpool){
        cv::Mat result;
        if(matpool.size()>0&&!matpool[0].empty()){
            int margin=5;
            //按照第一个mat的height来resize
            int new_height = matpool[0].rows;
            std::vector<cv::Mat> newpool;
            for(int ind=0;ind<matpool.size();ind++){
                Mat tmpmat = matpool[ind];
                int new_width = (int)((double)new_height/tmpmat.rows)*tmpmat.cols;
                cv::resize(tmpmat,tmpmat,cv::Size(new_width,new_height),0,0,CV_INTER_LINEAR);
                newpool.push_back(tmpmat);
            }

            int composeheight = 0;
            int composewidth = 0-margin;
            for(int i=0;i<newpool.size();i++){
                if(composeheight<newpool[i].rows)composeheight=newpool[i].rows;
                composewidth+=newpool[i].cols+margin;
            }

            result=cv::Mat::zeros(composeheight,composewidth,CV_8UC3);
            int startx=0;
            for(int i=0;i<newpool.size();i++) {
                cv::Rect roi(startx,0,newpool[i].cols,newpool[i].rows);
                newpool[i].copyTo(result(roi));
                startx+=margin+newpool[i].cols;
            }
        }
        return result;
    }

    cv::Mat composeGrayVisMat(std::vector<Mat> matpool) {
        int cols = 20;
        int rows = matpool.size() / 20 + 1;

        int size = 75;

        Mat vismat = Mat::zeros(size * rows, size * cols, CV_8UC1);
        for (int ind = 0; ind < matpool.size(); ind++) {
            int row = ind / cols;
            int col = ind % cols;
            int x = col * size;
            int y = row * size;
            Rect pasteroi(x, y, size - 5, size - 5);

            Mat pastemat;
            resize(matpool[ind], pastemat, Size(size - 5, size - 5), 0, 0);
            pastemat.copyTo(vismat(pasteroi));
        }

        return vismat;
    }

    bool expandRoi(int expandwidth, const cv::Rect &roi, cv::Rect &expandedROI, int matwidth, int matheight) {
        if (matwidth < 1 || matheight < 1)return false;

        int x = roi.x - expandwidth;
        int y = roi.y - expandwidth;
        int x2 = roi.br().x + expandwidth;
        int y2 = roi.br().y + expandwidth;

        if (x < 0)x = 0;
        if (y < 0)y = 0;
        if (x2 > matwidth - 1)x2 = matwidth - 1;
        if (y2 > matheight - 1)y2 = matheight - 1;
        expandedROI = Rect(Point(x, y), Point(x2, y2));

        return true;
    }

    cv::Mat rotate180(const cv::Mat& srcImage){
        cv::Mat rotateMat;
        cv::Mat result;
        cv::Point center(srcImage.cols/2,srcImage.rows/2);
        rotateMat = cv::getRotationMatrix2D(center,180,1);
        cv::warpAffine(srcImage,result,rotateMat,srcImage.size());
        return result;
    }

}