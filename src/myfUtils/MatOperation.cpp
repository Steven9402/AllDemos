//
// Created by cuizhou on 18-2-24.
//

#include "MatOperation.h"
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

    bool expandRoi(int expandwidth, int expandheight,const cv::Rect &roi, cv::Rect &expandedROI, int matwidth, int matheight) {
        if (matwidth < 1 || matheight < 1)return false;

        int x = roi.x - expandwidth;
        int y = roi.y - expandheight;
        int x2 = roi.br().x + expandwidth;
        int y2 = roi.br().y + expandheight;

        x=x>0?x:0;
        x=x<matwidth-1?x:matwidth-1;
        x2=x2>0?x2:0;
        x2=x2<matwidth-1?x2:matwidth-1;
        y=y>0?y:0;
        y=y<matheight?y:matheight-1;
        y2=y2>0?y2:0;
        y2=y2<matheight?y2:matheight-1;

        if(x2>x&&y2>y){
            expandedROI = Rect(Point(x, y), Point(x2, y2));
            return true;
        }else{
            return false;
        }

    }

    cv::Mat rotate(const cv::Mat& srcImage,double degree){
        cv::Mat rotateMat;
        cv::Mat result;
        cv::Point center(srcImage.cols/2+0.5,srcImage.rows/2+0.5);
        rotateMat = cv::getRotationMatrix2D(center,degree,1);
        cv::warpAffine(srcImage,result,rotateMat,srcImage.size());
        return result;
    }

    cv::Mat rotate(const cv::Mat& srcImage,const cv::Rect& rect,double degree){
        //对角线长度 创建空白mat
        float hlf_exp=sqrt(pow(rect.width/2,2)+pow(rect.height/2,2))+1;
        float center_x = rect.x+rect.width/2;
        float center_y = rect.y+rect.height/2;

        cv::Rect exp_rect;
        exp_rect.x=center_x-hlf_exp;
        exp_rect.y=center_y-hlf_exp;
        exp_rect.width=2*hlf_exp;
        exp_rect.height=2*hlf_exp;

        cv::Mat empty_mat = cv::Mat::zeros(exp_rect.height,exp_rect.width,srcImage.type());//创建empty_mat

        //计算 emptymat 中的兴趣区域
        cv::Rect interested_rect(rect.x-exp_rect.x,rect.y-exp_rect.y,rect.width,rect.height);

        //将原图拷贝到拓展的矩形中
        int x_shift=0,y_shift=0;
        cv::Rect ori_img_roi;

        if(exp_rect.x<0){
            x_shift=-exp_rect.x;
            ori_img_roi.x=0;
        }else{
            ori_img_roi.x=exp_rect.x;
        }

        if(exp_rect.y<0){
            y_shift=-exp_rect.y;
            ori_img_roi.y=0;
        }else{
            ori_img_roi.y=exp_rect.y;
        }

        if(exp_rect.br().x>srcImage.cols-1){
            ori_img_roi.width=srcImage.cols-1-ori_img_roi.x;
        }else{
            ori_img_roi.width=exp_rect.br().x-ori_img_roi.x;
        }

        if(exp_rect.br().y>srcImage.rows-1){
            ori_img_roi.height=srcImage.rows-1-ori_img_roi.y;
        }else{
            ori_img_roi.height=exp_rect.br().y-ori_img_roi.y;
        }


        //将原图中的expanded roi区域拷贝到 emptymat
        cv::Rect tmp_rect(x_shift,y_shift,ori_img_roi.width,ori_img_roi.height);
        srcImage(ori_img_roi).copyTo(empty_mat(tmp_rect));

        cv::Point center(empty_mat.cols / 2, empty_mat.rows / 2);
        cv::Mat rotateMat = cv::getRotationMatrix2D(center, degree, 1);

        cv::Mat resulttmp;
        cv::warpAffine(empty_mat, resulttmp, rotateMat, empty_mat.size());

        cv::Mat result;
        resulttmp(interested_rect).copyTo(result);
        return result;

    }

    /*
     * 旋转之后保留黑边 ,rgb图
     */
    cv::Mat rotateAdvanced(const cv::Mat& srcImage,double degree) {
        float end_to_end = sqrt(pow(srcImage.cols,2)+pow(srcImage.rows,2));
        float theta = atan((float)srcImage.cols/srcImage.rows)* 180/3.14159;

        float target_half_height=end_to_end/2*cos((theta-abs(degree))*3.14159/180);
        float target_half_width=end_to_end/2*cos((90-theta-abs(degree))*3.14159/180);

        int exp_x=target_half_width-srcImage.cols/2;
        int exp_y=target_half_height-srcImage.rows/2;

        if(exp_x<0)exp_x=0;
        if(exp_y<0)exp_y=0;

        cv::Mat srcImageExp;
        cv::copyMakeBorder(srcImage,srcImageExp,exp_y,exp_y,exp_x,exp_x,BORDER_CONSTANT,cv::Scalar(0,0,0));

        cv::Mat srcImageRotate=rotate(srcImageExp,degree);
        return srcImageRotate;
    }

    cv::Mat linearLighten(const cv::Mat& srcImage){

        cv::Mat result(srcImage.rows,srcImage.cols,CV_8UC1);
        if(srcImage.channels()!=1){
            return result;
        }

        cv::Mat dst=srcImage.clone();
        GaussianBlur(srcImage,dst,Size(9,9),0,0);

        int avg=0;
        for(int row=0;row<srcImage.rows;row++){
            for(int col=0;col<srcImage.cols;col++){
                int value = dst.at<uchar>(row,col);
                avg+=value;
            }
        }
        avg=avg/srcImage.rows/srcImage.cols;

        float ratio=(float)120/avg;

        if(ratio>0){

            for(int row=0;row<srcImage.rows;row++){
                for(int col=0;col<srcImage.cols;col++){
                    int value = srcImage.at<uchar>(row,col);
                    int new_value=ratio*value;
                    if(new_value>255)new_value=255;

                    result.at<uchar>(row,col)=new_value;
                }
            }
        }

        return result;
    }

    cv::Point rotatePoint(float x,float y,float theta,cv::Point s0,cv::Point s1){
        float x0=x-s0.x;
        float y0=y-s0.y;

        float d_theta=theta*3.14159/180;
        float xx=x0*cos(d_theta)+y0*sin(d_theta)+s1.x;
        float yy=-x0*sin(d_theta)+y0*cos(d_theta)+s1.y;
        return cv::Point(xx,yy);
    }


}
