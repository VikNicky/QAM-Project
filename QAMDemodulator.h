#pragma once
#include <vector>
#include <complex>
#include <stdexcept>

using namespace std;

class QAMDemodulator {
private:
    unsigned int constellation_order;
    unsigned int bps;
    float norm_factor;
    vector<complex<float>> constellation_map;

    void createConstellationMap();
    vector<bool> symbolToBits(int symInd) const;
    bool is_power_of_4(unsigned int n);

public:
    QAMDemodulator(unsigned int order = 4);
    vector<unsigned char> demodulate(const complex<float>& symbol) const;
    vector<unsigned char> demodulateBlock(const vector<complex<float>>& symbols) const;
};