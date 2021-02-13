//
// Created by david on 1/6/19.
//

#include "Timer.h"


namespace jarvis {


    double Timer::get_difTime(Timer::timeUnits units, std::string label) {

        auto itMapStarts = _mapStarts.find(label);
        auto itMapEnds   = _mapEnds.find(label);

        if (itMapStarts == _mapStarts.end()) {
            std::cout << "[ERROR]: There is not starting element with that label \n";
            return -1;
        }

        if (itMapEnds == _mapEnds.end()) {
            std::cout << "[ERROR]: There is not ending element with that label \n";
            return -1;
        }

        double timeDuration = -1;

        switch (units) {
            case seconds:
                timeDuration = std::chrono::duration<double>
                        (itMapEnds->second  - itMapStarts->second).count();
                break;
            case milliseconds:
                timeDuration = std::chrono::duration<double, std::milli>
                        (itMapEnds->second  - itMapStarts->second).count();
                break;
            default:
                std::cout << "[ERROR]: None of the options for time units matched \n";
        }

        return timeDuration;

    }
}