#include "QAMModulator.h"
#include "AWGNChannel.h"
#include "QAMDemodulator.h"
#include "BER.h"
#include "TextfileTools.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <complex>
#include <cmath>
#include <random>
#include <stdexcept>
#include <algorithm>

int main() {
    try {
        ///// Tasks 1-3

        QAMModulator modulator(4);
        AWGNChannel channel(10);
        QAMDemodulator demodulator(4);

        cout << "Task 1-3. Making classes and testing " << endl;

        // generating message
        vector<unsigned char> bits;
        unsigned int numBits = 16;
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(0, 1);

        for (size_t i = 0; i < numBits; ++i) {
            bits.push_back(dis(gen));
        }
        saveBitVector(bits, "bits.txt");

        // modulation
        auto symbols = modulator.modulate(bits);
        saveComplexVector(symbols, "mod_syms.txt");
        saveComplexVector(modulator.getConstellation(), "constellation.txt");

        //noise
        auto noisy_syms = channel.addNoise(symbols);
        saveComplexVector(noisy_syms, "noisy_syms.txt");

        // demodulation
        auto rx_bits = demodulator.demodulateBlock(noisy_syms);
        saveBitVector(rx_bits, "rx_bits.txt");

        cout << "Task Completed! Look into the text files and ReadMe.ipynb" << endl;

        // Task 4
        for (int i = 0; i < 10; ++i) {
            AWGNChannel channel(numeric_limits<double>::quiet_NaN(), 0.1 * i);
            auto noisy_syms = channel.addNoise(symbols);
            auto rx_bits = demodulator.demodulateBlock(noisy_syms);
            size_t error_count = 0;
            for (size_t j = 0; j < rx_bits.size(); ++j) {
                if (abs(rx_bits[j] - bits[j]) > 0) {
                    ++error_count;
                }
            }
            cout << "Task 4. Calling classes for different values of noise variances" << endl;
            cout << "Number of errors for noise dispersion " << 0.1 * i << " is " << error_count << " from " << numBits << endl;
        }
        // Task 5
        cout << "Task 5. Getting BER" << endl;
        vector<double> snrs;
        for (int i = 0; i <= 52; i = i + 2) {
            snrs.push_back(i * 0.5);
        };
        auto result = calculate_ber(64, snrs, 1000, 1e9, 2048);

        for (const auto& res : result) {
            cout << "SNR, dB: " << res.snr
                << ", BER: " << res.ber
                << ", Errors: " << res.error_bits << "/" << res.total_bits
                << ", Iters: " << res.iterations << endl;
        }

        save_results_to_files(result);
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}