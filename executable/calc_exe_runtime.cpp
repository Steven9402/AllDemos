//
// Created by cuizhou on 18-9-17.
//


#include <iostream>
#include <chrono>

using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;

int main()
{
    high_resolution_clock::time_point beginTime = high_resolution_clock::now();

    //run

    high_resolution_clock::time_point endTime = high_resolution_clock::now();
    milliseconds timeInterval = std::chrono::duration_cast<milliseconds>(endTime - beginTime);
    std::cout << timeInterval.count() << "ms\n";
}
