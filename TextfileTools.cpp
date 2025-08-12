#include "TextfileTools.h"
#include <fstream>
#include <iomanip>
#include <iostream>

void saveBitVector(const vector<unsigned char>& vec, const string& filename) {
    ofstream outFile(filename);
    if (!outFile.is_open()) {
        cerr << "Can't open file!" << endl;
        return;
    }
    for (size_t i = 0; i < vec.size(); ++i) {
        outFile << static_cast<int>(vec[i]);

        if (i != vec.size() - 1) {
            outFile << ", ";
        }
    }
}

void saveComplexVector(const vector<complex<float>>& vec, const string& filename) {
    ofstream outFile(filename);
    if (!outFile.is_open()) {
        cerr << "Can't open file!" << endl;
        return;
    }

    outFile << fixed << setprecision(6);

    for (const auto& num : vec) {

        outFile << "(" << num.real() << ", " << num.imag() << "i)" << endl;
    }

    outFile.close();
}

void save_results_to_files(const vector<BERResult>& results) {

    ofstream snr_file("snr_values_64.txt");
    ofstream ber_file("ber_values_64.txt");
    snr_file << fixed << setprecision(10);
    ber_file << fixed << setprecision(10);


    if (!snr_file.is_open() || !ber_file.is_open()) {
        cerr << "Error opening output files!" << endl;
        return;
    }

    for (const auto& res : results) {
        snr_file << res.snr << endl;
        ber_file << res.ber << endl;
    }

    snr_file.close();
    ber_file.close();
}