#pragma once
#include <vector>

using namespace std;

struct BERResult {
    double snr;
    double ber;
    size_t total_bits;
    size_t error_bits;
    size_t iterations;
};

vector<BERResult> calculate_ber(
    unsigned int modulation_order,
    const vector<double>& snr,
    size_t max_errors, // Error stop
    size_t max_bits, // Error stop
    unsigned int bpb // Bits in iteration
);

