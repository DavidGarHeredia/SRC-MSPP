//
// Created by david on 1/6/19.
//


#include "Random.h"

namespace jarvis {

    double Random::generate_random(std::default_random_engine &generator) {

        switch (_option) {
            case RVUnifD:
                d_unif(generator);
                break;
            case RVUnifC:
                r_unif(generator);
                break;
            case RVBin:
                r_bin(generator);
                break;
            case RVPois:
                r_pois(generator);
                break;
            case RVExp:
                r_exp(generator);
                break;
            case RVNorm:
                r_norm(generator);
                break;
            default:
                std::cout << "[ERROR]: None of the options match."
                          << " No random number is generated."
                          << '\n';
        }
        return _value;
    }

    void Random::r_unif(std::default_random_engine &generator) {
        std::uniform_real_distribution<double> distribution(_lb, _ub);
        _value = distribution(generator);
    }

    void Random::d_unif(std::default_random_engine &generator) {

        std::uniform_int_distribution<int> distribution(static_cast<int>(_lb),
                                                        static_cast<int>(_ub));
        _value = distribution(generator);
    }

    void Random::r_bin(std::default_random_engine &generator) {
        std::binomial_distribution<int> distribution(_n, _p);;
        _value = distribution(generator);
    }

    void Random::r_pois(std::default_random_engine &generator) {
        std::poisson_distribution<int> distribution(_lambda);
        _value = distribution(generator);
    }

    void Random::r_exp(std::default_random_engine &generator) {
        std::exponential_distribution<double> distribution(_lambda);
        _value = distribution(generator);
    }

    void Random::r_norm(std::default_random_engine &generator) {
        std::normal_distribution<double> distribution(_mu, _sigma);
        _value = distribution(generator);
    }

}
