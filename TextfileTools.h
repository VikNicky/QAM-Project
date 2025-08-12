#pragma once
#include "BER.h"
#include <vector>
#include <complex>
#include <string>


using namespace std;

void saveBitVector(const vector<unsigned char>& vec, const string& filename);
void saveComplexVector(const vector<complex<float>>& vec, const string& filename);
void save_results_to_files(const vector<BERResult>& results);