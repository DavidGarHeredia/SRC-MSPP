//
// Created by david on 1/6/19.
//

#ifndef JARVISLIBRARY_TABLECREATOR_H
#define JARVISLIBRARY_TABLECREATOR_H

#include "Results.h"
#include <fstream>
#include <ctime>

namespace jarvis {

    class TableCreator {

    private:
        std::string _tableName = "noNameGiven.txt";
        char _separator = ',';

    public:

        TableCreator() {}

        explicit TableCreator(const std::string &_tableName) : _tableName(_tableName) {}

        virtual ~TableCreator() {}

        const std::string &get_tableName() const {
            return _tableName;
        }

        void set_tableName(const std::string &_tableName) {
            TableCreator::_tableName = _tableName;
        }

        char get_separator() const {
            return _separator;
        }

        void set_separator(char _separator) {
            TableCreator::_separator = _separator;
        }

        void export_table(std::vector<Results> results);

    };

}

#endif //JARVISLIBRARY_TABLECREATOR_H
