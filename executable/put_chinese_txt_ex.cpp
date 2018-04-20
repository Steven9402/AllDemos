//
// Created by cuizhou on 18-4-20.
//

#include "putChineseText/OpencvText.h"
#include <memory>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

int main(){
    const char* font_path = "../../res/putChineseText/STKAITI.TTF";
    std::shared_ptr<Cv310Text> put_text_ptr = std::make_shared<Cv310Text>(font_path,30);

    cv::Mat srcImage = cv::imread("../../data/timg2.jpg");

    put_text_ptr->putText(srcImage,"中文",cv::Point(20,20),cv::Scalar(255,5,46));
    cv::imshow("ss",srcImage);
    cv::waitKey(0);
}