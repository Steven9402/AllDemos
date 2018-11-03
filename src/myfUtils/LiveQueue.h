//
// Created by cuizhou on 18-7-27.
//

#ifndef ALLDEMOS_LIVEQUEUE_H
#define ALLDEMOS_LIVEQUEUE_H


class LiveQueue {
public:
    LiveQueue(const int length);
    LiveQueue();
    ~LiveQueue();

    void update(float ratio);

    void gc();

private:
    void moveForward(float*);

protected:
    int _length_queue;
    int _idx=0;
    float* _queue;
};


#include <vector>
class LiveQueue2 {
public:
    LiveQueue2(const int length,const int count_max_thresh);
    LiveQueue2();
    ~LiveQueue2();

    int getStateBase(const float ratio);

    void gc();

    int getLiveTime();

protected:
    void update(const float ratio);

private:
    void moveForward(std::vector<float>& frame);

protected:
    int _length_queue;
    int _count_max_thresh;
    int _idx=0;
    std::vector<float> _queue;
};


#endif //ALLDEMOS_LIVEQUEUE_H
