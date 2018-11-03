//
// Created by cuizhou on 18-3-25.
//

#include "ParameterHandler.h"
ParameterHandler::ParameterHandler(){

}
ParameterHandler::ParameterHandler(char* ini_path){
    _ini_path = ini_path;
}

void ParameterHandler::initParams(){

    parameterReader *para_reader = parameterReader::GetInstance(_ini_path); //"../../res/parameters.ini"

    video_in = para_reader->get_data("video_in");//.c_str();
    video_out = para_reader->get_data("video_out");

    // Config file settings
    line_reject_degrees = atoi(para_reader->get_data("line_reject_degrees").c_str());
    canny_min_thresh = atoi(para_reader->get_data("canny_min_thresh").c_str());
    canny_max_thresh = atoi(para_reader->get_data("canny_max_thresh").c_str());
    hough_thresh = atoi(para_reader->get_data("hough_thresh").c_str());
    hough_min_length = atoi(para_reader->get_data("hough_min_length").c_str());
    hough_max_gap = atoi(para_reader->get_data("hough_max_gap").c_str());
    scan_step = atoi(para_reader->get_data("scan_step").c_str());
    bw_thresh = atoi(para_reader->get_data("bw_thresh").c_str());
    borderx = atoi(para_reader->get_data("borderx").c_str());
    max_response_dist = atoi(para_reader->get_data("max_response_dist").c_str());
    k_vary_factor = atof(para_reader->get_data("k_vary_factor").c_str());
    b_vary_factor = atoi(para_reader->get_data("b_vary_factor").c_str());
    max_lost_frames = atoi(para_reader->get_data("max_lost_frames").c_str());
    departure_b_thresh = atof(para_reader->get_data("departure_b_thresh").c_str());

    pyramid_ratio = atof(para_reader->get_data("pyramid_ratio").c_str());
}
