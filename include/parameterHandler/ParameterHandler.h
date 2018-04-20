//
// Created by cuizhou on 18-3-25.
//

#ifndef HELLOWORLD_PARAMETERHANDLER_H
#define HELLOWORLD_PARAMETERHANDLER_H

#include "parameterHandler/parameterReader.h"
class ParameterHandler {
public:
    ParameterHandler();
    ParameterHandler(char* ini_path);

    void initParams();

public:

    std::string _ini_path;
    // Config file settings
    std::string video_in;
    std::string video_out;

    int srcImageWidth;
    int srcImageHeight;
    
    float lane_roi_heightratio;

    int line_reject_degrees;
    int canny_min_thresh;
    int canny_max_thresh;
    int hough_thresh;
    int hough_min_length;
    int hough_max_gap;
    int scan_step;
    int bw_thresh;
    int borderx;
    int max_response_dist;
    float k_vary_factor;
    int b_vary_factor;
    int max_lost_frames;
    float departure_b_thresh;

    float pyramid_ratio;

//detect car roi
    float detect_top_diff;
    float detect_down_diff;
    float detect_left_diff;
    float detect_right_diff;

    //跟踪的多边形底部同上，口子稍作收缩
    float track_shrink_ratio;
};


#endif //HELLOWORLD_PARAMETERHANDLER_H
