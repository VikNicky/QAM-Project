#include "BER.h"
#include "QAMModulator.h"
#include "AWGNChannel.h"
#include "QAMDemodulator.h"
#include <random>
#include <iostream>



vector<BERResult> calculate_ber(
    unsigned int modulation_order,
    const vector<double>& snr,
    size_t max_errors,
    size_t max_bits,
    unsigned int bpb
) {
        vector<BERResult> results;
        results.reserve(snr.size());

        // Initialization
        QAMModulator modulatorQAM(modulation_order);
        QAMDemodulator demodulatorQAM(modulation_order);

        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(0, 1);

        for (double snrs : snr) {

            AWGNChannel channelAWGN(snrs);

            size_t total_errors = 0;
            size_t total_bits = 0;
            size_t iterations = 0;

            while (total_errors < max_errors && total_bits < max_bits) {
                // Generating message
                vector<unsigned char> tx_bits;
                for (size_t i = 0; i < bpb; ++i) {
                    tx_bits.push_back(dis(gen));
                }

                // Modulation
                auto symbols = modulatorQAM.modulate(tx_bits);

                // Noise
                auto noisy_symbols = channelAWGN.addNoise(symbols);

                // Demodulation
                auto rx_bits = demodulatorQAM.demodulateBlock(noisy_symbols);

                // Counting errors
                size_t block_errors = 0;
                for (size_t i = 0; i < tx_bits.size(); ++i) {
                    if (tx_bits[i] != rx_bits[i]) {
                        block_errors++;
                    }
                }

                total_errors += block_errors;
                total_bits += tx_bits.size();
                iterations++;
            }

            results.push_back({
                snrs,
                (total_bits > 0) ? static_cast<double>(total_errors) / total_bits : 0.0,
                total_bits,
                total_errors,
                iterations
                });
        }

        return results;
}

