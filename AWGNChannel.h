#pragma once
#include <vector>
#include <complex>
#include <random>
#include <limits>

using namespace std;

class AWGNChannel {
private:
    double snr_db;
    double noise_power;
    default_random_engine generator;
    normal_distribution<double> distribution;

public:
    AWGNChannel(double snr_db = numeric_limits<double>::quiet_NaN(),
        double noise_power = numeric_limits<double>::quiet_NaN(),
        unsigned int seed = random_device{}());

    vector<complex<float>> addNoise(const vector<complex<float>>& mod_data);
    float getNoisePower() const;
};