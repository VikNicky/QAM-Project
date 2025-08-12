#include "AWGNChannel.h"

AWGNChannel::AWGNChannel(double snr_db, double noise_power, unsigned int seed)
    : generator(seed), distribution(0.0, 1.0) {

    if (!isnan(snr_db) && !isnan(noise_power)) {
        throw invalid_argument("Enter SNR or Noise Power! Not Together!");
    }

    if (isnan(snr_db) && isnan(noise_power)) {
        throw invalid_argument("Enter SNR or Noise Power!");
    }

    this->snr_db = snr_db;
    this->noise_power = noise_power;
}

vector<complex<float>> AWGNChannel::addNoise(const vector<complex<float>>& mod_data) {
    // If SNR
    if (!isnan(snr_db)) {

        float signal_power = 0.0;
        for (const auto& symbol : mod_data) {
            signal_power += norm(symbol);
        }
        signal_power /= mod_data.size();
        float snr_linear = pow(10.0, snr_db / 10.0);
        noise_power = signal_power / snr_linear;
    }

    vector<complex<float>> noisy_data;
    noisy_data.reserve(mod_data.size());

    // Standard deviation
    float stddev = sqrt(noise_power / 2.0);


    for (const auto& symbol : mod_data) {
        float real_noise = distribution(generator) * stddev;
        float imag_noise = distribution(generator) * stddev;
        noisy_data.emplace_back(
            symbol.real() + real_noise,
            symbol.imag() + imag_noise
        );
    }

    return noisy_data;
}

float AWGNChannel::getNoisePower() const {
    return noise_power;
}