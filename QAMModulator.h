#pragma once
#include <vector>
#include <complex>
#include <stdexcept>

using namespace std;

class QAMModulator {
private:
    unsigned int constellation_order;  // modulation order 
    unsigned int bps;      // bits per symbol
    float norm_factor;        // norm factor
    vector<complex<float>> constellation_map;  // constellation

    // checking n is power of 4 for square constellation
    bool is_power_of_4(unsigned int n);

    // getting constellation
    void createConstellationMap();

public:

    QAMModulator(unsigned int order = 4);

    // using modulation
    vector<complex<float>> modulate(vector<unsigned char>& bits);

    const vector<complex<float>>& getConstellation() const;
    


};