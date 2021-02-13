//
// Created by david on 1/18/19.
//

#ifndef JARVISLIBRARY_LOG_H
#define JARVISLIBRARY_LOG_H


#include <string>
#include <fstream>
#include <ctime>

namespace jarvis {

    class Log {

    public:
        enum logInfo {Nothing, EndOutput, Error, Warning, Info};

    private:
        std::string _logfilename = "noNameGiven.txt";
        logInfo _logstatus = Info;

    public:

        Log() {}

        Log(const std::string &_logfilename, logInfo _logstatus) : _logfilename(_logfilename), _logstatus(_logstatus) {
            time_t now = time(0);
            tm *ltm = localtime(&now);
            std::string date  = "\n\n" +
                                std::to_string(1900 + ltm->tm_year) + "/" +
                                std::to_string(1 + ltm->tm_mon) + "/" +
                                std::to_string(ltm->tm_mday) + "    " +
                                std::to_string(ltm->tm_hour) + "h:" +
                                std::to_string(ltm->tm_min) + "m:" +
                                std::to_string(ltm->tm_sec) + "s\n\n";
            write_log(date, Error);

        }

        virtual ~Log() {

            write_log("\n\n", EndOutput);
            write_log("=====================================================", EndOutput);
            write_log("=====================================================", EndOutput);
            write_log("=====================================================", EndOutput);
            write_log("\n\n\n", EndOutput);

        }

        const std::string &get_logfilename() const {
            return _logfilename;
        }

        void set_logfilename(const std::string &_logfilename) {
            Log::_logfilename = _logfilename;
            time_t now = time(0);
            tm *ltm = localtime(&now);
            std::string date  = std::to_string(1900 + ltm->tm_year) + "/" +
                                std::to_string(1 + ltm->tm_mon) + "/" +
                                std::to_string(ltm->tm_mday) + "    " +
                                std::to_string(ltm->tm_hour) + "h:" +
                                std::to_string(ltm->tm_min) + "m:" +
                                std::to_string(ltm->tm_sec) + "s\n\n";
            write_log(date, EndOutput);
        }

        logInfo get_logstatus() const {
            return _logstatus;
        }

        void set_logstatus(logInfo _logstatus) {
            Log::_logstatus = _logstatus;
        }

        void write_log(const std::string msg, logInfo level);

    };

}





#endif //JARVISLIBRARY_LOG_H
