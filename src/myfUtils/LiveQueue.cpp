//
// Created by cuizhou on 18-7-27.
//

#include "LiveQueue.h"
//#include "../global_variables.h"
LiveQueue::LiveQueue() {

}
LiveQueue::LiveQueue(const int length) {
    _length_queue=length;
    _queue = new float[length];
    _idx   = 0 ;
}

LiveQueue::~LiveQueue(){
    delete []_queue;
}

void LiveQueue::update(float ratio){
    if(_idx<_length_queue){
        _queue[_idx]=ratio;
    }else{
        moveForward(_queue);
        _queue[_length_queue - 1] = ratio;
    }
    ++_idx;

}

void LiveQueue::gc(){
    _idx  = 0;
    for(int i = 0;i <_length_queue;i++)
    {
        _queue[i]=0;
    }
}
void LiveQueue::moveForward(float *frames) {
    for(int i = 1 ; i <_length_queue;i++)
    {
        frames[i-1]=frames[i];
    }
}


//----------------------------------------------------------------------------------------------------------------------

LiveQueue2::LiveQueue2() {

}
LiveQueue2::LiveQueue2(const int length,const int count_max_thresh) {
    _length_queue=length;
    _idx   = 0 ;
    _count_max_thresh = count_max_thresh;
}

LiveQueue2::~LiveQueue2(){

}

void LiveQueue2::update(const float ratio){
    if(_idx<_length_queue){
        _queue.push_back(ratio);
    }else{
        moveForward(_queue);
        _queue[_length_queue - 1] = ratio;
    }
    ++_idx;
}


int LiveQueue2::getLiveTime(){
    return _idx;
}

void LiveQueue2::gc(){
    _idx  = 0;
    for(int i = 0;i <_length_queue;i++)
    {
        _queue[i]=0;
    }
}
void LiveQueue2::moveForward(std::vector<float>& frames) {
    for(int i = 1 ; i <_length_queue;i++)
    {
        frames[i-1]=frames[i];
    }
}


int LiveQueue2::getStateBase(const float ratio){
    update(ratio);
    if(_idx<_length_queue){
        return -1;
    }
    int count = 0;
    for(int ind=0;ind<_length_queue;ind++){
        if(_queue[ind]==1.0){
            count++;
        }
    }
    if(count>_count_max_thresh)return 1;
    else return 0;
}



