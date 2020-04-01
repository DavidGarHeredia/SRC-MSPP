//
// Created by david on 1/6/19.
//

#ifndef JARVISLIBRARY_RANDOM_H
#define JARVISLIBRARY_RANDOM_H


#include <random>
#include <iostream>

namespace jarvis {

    class Random {

    public:
        enum randVar { RVUnifD, RVUnifC, RVBin, RVPois, RVExp, RVNorm };

    private:

        double   _value;
        randVar  _option = RVUnifC;

        // parameters for the uniform
        double _lb = 0.0, _ub = 1.0;

        // parameters for the normal distribution
        double _mu = 0.0, _sigma = 1.0;

        // parameters for binomial distribution
        double _p = 0.5;
        int    _n = 1;

        // parameters for poisson or exponential distribution
        double _lambda = 1.0;

    public:

        Random() {}

        Random(randVar _option) : _option(_option) {}

        virtual ~Random() {}

        double get_value() const {
            return _value;
        }

        void set_value(double _value) {
            Random::_value = _value;
        }

        int get_option() const {
            return _option;
        }

        void set_option(randVar _option) {
            Random::_option = _option;
        }

        double get_lb() const {
            return _lb;
        }

        void set_lb(double _lb) {
            Random::_lb = _lb;
        }

        double get_ub() const {
            return _ub;
        }

        void set_ub(double _ub) {
            Random::_ub = _ub;
        }

        double get_mu() const {
            return _mu;
        }

        void set_mu(double _mu) {
            Random::_mu = _mu;
        }

        double get_sigma() const {
            return _sigma;
        }

        void set_sigma(double _sigma) {
            Random::_sigma = _sigma;
        }

        double get_p() const {
            return _p;
        }

        void set_p(double _p) {
            Random::_p = _p;
        }

        int get_n() const {
            return _n;
        }

        void set_n(int _n) {
            Random::_n = _n;
        }

        double get_lambda() const {
            return _lambda;
        }

        void set_lambda(double _lambda) {
            Random::_lambda = _lambda;
        }


        // Methods

        double generate_random(std::default_random_engine &generator);
        void d_unif(std::default_random_engine &generator);
        void r_unif(std::default_random_engine &generator);
        void r_bin(std::default_random_engine &generator);
        void r_pois(std::default_random_engine &generator);
        void r_exp(std::default_random_engine &generator);
        void r_norm(std::default_random_engine &generator);

    };


}



#endif //JARVISLIBRARY_RANDOM_H
