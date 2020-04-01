//
// Created by david on 1/6/19.
//

#include "TableCreator.h"


namespace jarvis {

    void TableCreator::export_table(std::vector<Results> results) {

        time_t now = time(0);
        tm *ltm = localtime(&now);
        std::string date  = std::to_string(1900 + ltm->tm_year) + "/" +
                            std::to_string(1 + ltm->tm_mon) + "/" +
                            std::to_string(ltm->tm_mday);
        std::string time2 = std::to_string(ltm->tm_hour) + "h:" +
                            std::to_string(ltm->tm_min)  + "m:" +
                            std::to_string(ltm->tm_sec)  + "s";

        std::fstream fs;
        fs.open (_tableName, std::fstream::out | std::fstream::app);

        // Headers of the table
        fs << "Date" << _separator << "Time" << _separator << "InstanceName" << _separator;
        for (int i = 0; i < results[0].get_sizeResults(); i++) {
            fs << results[0].get_colName(i);
            if (i != results[0].get_sizeResults() - 1) { fs << _separator; }
        }

        // Results
        for (int i = 0; i < results.size(); i++) {
            fs << std::endl;
            fs << date  << _separator;
            fs << time2 << _separator;
            fs << results[i].get_instanceName() << _separator;
            for (int j = 0; j < results[i].get_sizeResults(); j++) {
                if (results[i].is_numeric(j)) {
                    fs << results[i].get_value_numeric(j);
                } else {
                    fs << results[i].get_value_string(j);
                }
                if (j != results[i].get_sizeResults() - 1) { fs << _separator; }
            }
        }

        fs << std::endl;
        fs.close();
    }
}
