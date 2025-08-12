#include "QAMModulator.h"
#include <cmath>

QAMModulator::QAMModulator(unsigned int order) : constellation_order(order) {
    if (!is_power_of_4(order) || order < 4) {
        throw invalid_argument("Order must be a power of 4!");
    }
    bps = static_cast<unsigned int>(log2(order));
    norm_factor = sqrt(3.0f / 2.0f / (order - 1));
    createConstellationMap();
}

bool QAMModulator::is_power_of_4(unsigned int n) {
    return (n != 0) && ((n & (n - 1)) == 0) && ((n & 0x5555'5555) != 0);
}

void QAMModulator::createConstellationMap() {
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

    for (auto x : xPoints) {
        for (auto y : yPoints) {
            constellation_map.emplace_back(x * norm_factor, y * norm_factor);
        }
    }
}

vector<complex<float>> QAMModulator::modulate(vector<unsigned char>& bits) {
    // checking message size + filling in with zeros
    const size_t remainder = bits.size() % bps;
    if (remainder != 0) {
        const size_t paddingLength = bps - remainder;
        bits.insert(bits.end(), paddingLength, 0);
    }

    size_t num_symbols = bits.size() / bps;
    vector<complex<float>> symbols(num_symbols);

    for (size_t i = 0; i < num_symbols; ++i) {
        unsigned int index = 0;

        for (unsigned int j = 0; j < bps; ++j) {
            index = (index << 1) | bits[i * bps + j];
        }

        symbols[i] = constellation_map[index];
    }

    return symbols;
}

const vector<complex<float>>& QAMModulator::getConstellation() const {
    return constellation_map;
}