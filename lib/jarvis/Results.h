//
// Created by david on 1/6/19.
//

#ifndef JARVISLIBRARY_RESULTS_H
#define JARVISLIBRARY_RESULTS_H

#include <string>
#include <vector>

struct ResultStruct {
    std::string colName, label;
    double value;
    bool numeric;
};

namespace jarvis {

    class Results {

    private:
        ResultStruct _r1;
        std::string _instanceName = "noNameGiven";
        std::vector<ResultStruct> _results;

    public:

        Results() {}

        explicit Results(const std::string &_instanceName) : _instanceName(_instanceName) {}

        virtual ~Results() {}

        const std::string &get_instanceName() const {
            return _instanceName;
        }

        void set_instanceName(const std::string &_instanceName) {
            Results::_instanceName = _instanceName;
        }

        void add_result_numeric(const std::string &colName, const double &value) {
            _r1.colName = colName;
            _r1.value   = value;
            _r1.numeric = true;
            _results.push_back(_r1);
        }

        void add_result_string(const std::string &colName, const std::string &value) {
            _r1.colName = colName;
            _r1.label   = value;
            _r1.numeric = false;
            _results.push_back(_r1);
        }

        const std::string &get_colName(int pos) const {
            return _results[pos].colName;
        }

        const std::string &get_lastColName() const {
            return _results.back().colName;
        }

        double get_value_numeric(int pos) const {
            return _results[pos].value;
        }

        std::string get_value_string(int pos) const {
            return _results[pos].label;
        }

        bool is_numeric(int pos) const {
            return _results[pos].numeric;
        }

        int get_sizeResults() const {
            return static_cast<int>(_results.size());
        }


    };

}

#endif //JARVISLIBRARY_RESULTS_H
