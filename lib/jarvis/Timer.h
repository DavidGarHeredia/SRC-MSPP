//
// Created by david on 1/6/19.
//

#ifndef JARVISLIBRARY_TIMER_H
#define JARVISLIBRARY_TIMER_H

#include <chrono>
#include <map>
#include <iostream>


namespace jarvis {


    class Timer {

    private:

        std::map<std::string, std::chrono::steady_clock::time_point> _mapStarts, _mapEnds;

//        std::chrono::steady_clock::now()

    public:

        enum  timeUnits {seconds, milliseconds};

        void start_time(std::string label) {
            _mapStarts[label] = std::chrono::steady_clock::now();
        }

        void end_time(std::string label) {
            _mapEnds[label] = std::chrono::steady_clock::now();
        }

        double get_difTime(timeUnits units, std::string label);



    };

}

#endif //JARVISLIBRARY_TIMER_H
