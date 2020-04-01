//
// Created by david on 1/18/19.
//

#include "Log.h"


namespace jarvis {


    void Log::write_log(const std::string msg, Log::logInfo level) {

        if (level <= _logstatus) {

            std::string msg1 = "";

            if (level == Error) {
                msg1 = "[ERROR]: ";
            } else if (level == Warning) {
                msg1 = "[WARNING]: ";
            } else if (level == Info) {
                msg1 = "[INFO]: ";
            }

            std::fstream fs;
            fs.open (_logfilename, std::fstream::out | std::fstream::app);
            fs << msg1 << msg << '\n';
            fs.close();
        }

    }
}
