#include "QAMDemodulator.h"
#include <cmath>

QAMDemodulator::QAMDemodulator(unsigned int order) : constellation_order(order) {

    if (!is_power_of_4(order) || order < 4) {
        throw invalid_argument("Order must be a power of 4!");
    }

    bps = static_cast<unsigned int>(log2(order));
    norm_factor = sqrt(3.0f / 2.0f / (order - 1));



    createConstellationMap();
}

void QAMDemodulator::createConstellationMap() {
    const int side_size = static_cast<int>(sqrt(constellation_order));
    constellation_map.clear();
    constellation_map.reserve(constellation_order);

    vector<float> xPoints, yPoints;
    for (int i = -(side_size - 1); i <= (side_size - 1); i += 2) {
        xPoints.push_back(i);
    }
    for (int i = (side_size - 1); i >= -(side_size - 1); i -= 2) {
        yPoints.push_back(i);
    }

    for (auto y : yPoints) {
        for (auto x : xPoints) {
            constellation_map.emplace_back(x * norm_factor, y * norm_factor);
        }
    }
}

vector<unsigned char> QAMDemodulator::demodulate(const complex<float>& symbol) const {

    vector<unsigned char> bits(bps);
    const int sqrt_order = static_cast<int>(sqrt(constellation_order));
    float num_re = round((symbol.real() / norm_factor + (sqrt_order - 1)) / 2.0f);
    float num_im = round((symbol.imag() / norm_factor + (sqrt_order - 1)) / 2.0f);

    if (num_re < 0)
        num_re = 0;
    else if
        (num_re > sqrt_order - 1)
        num_re = sqrt_order - 1;

    if (num_im < 0)
        num_im = 0;
    else if
        (num_im > sqrt_order - 1)
        num_im = sqrt_order - 1;


    unsigned int index = sqrt_order - num_im - 1 + sqrt_order * num_re;
    for (int i = 0; i < bps; ++i) {
        bits[i] = (index >> (bps - 1 - i)) & 1;
    }
    return bits;
}

vector<unsigned char> QAMDemodulator::demodulateBlock(const vector<complex<float>>& symbols) const {
    vector<unsigned char> result;
    result.reserve(symbols.size() * bps);

    for (const auto& symbol : symbols) {
        auto bits = demodulate(symbol);
        result.insert(result.end(), bits.begin(), bits.end());
    }

    return result;
}

vector<bool> QAMDemodulator::symbolToBits(int symInd) const {
    vector<bool> bits(bps);

    for (int i = 0; i < bps; ++i) {
        bits[bps - 1 - i] = (symInd >> i) & 1;
    }
    return bits;
}

bool QAMDemodulator::is_power_of_4(unsigned int n) {
    return (n != 0) && ((n & (n - 1)) == 0) && ((n & 0x5555'5555) != 0);
}