//
// Created by david on 4/27/19.
//

#ifndef JARVISLIBRARY_JARVIS_H
#define JARVISLIBRARY_JARVIS_H


#include "Log.h"
#include "Random.h"
#include "Results.h"
#include "TableCreator.h"
#include "Timer.h"


namespace jarvis {

    class Jarvis {

    public:
        jarvis::Timer timer;
        std::vector<std::string> tableNames;
        std::map<std::string, std::vector<jarvis::Results>> dictResults;
        jarvis::TableCreator table;
        jarvis::Random randNum;
        jarvis::Log logFile;

    };

}



#endif //JARVISLIBRARY_JARVIS_H
