//
// Created by cuizhou on 18-4-20.
//

#include <iostream>
#include "parameterHandler/ParameterHandler.h"

using namespace std;
int main(){

    ParameterHandler parameter_handler("../../res/parameters.ini");
    parameter_handler.initParams();

    std::string video_in = parameter_handler.video_in;
    const char* video_in_=video_in.c_str();

    cout<<"video_in_ = "<< video_in_ <<endl;
}