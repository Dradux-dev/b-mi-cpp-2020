#include "compare-runtime.h"

#include <fstream>
#include <iomanip>
#include <iostream>

void CompareRuntime::configure(size_t maxStep, size_t maxValue, size_t iterations) {
    this->maxStep = maxStep;
    this->maxValue = maxValue;
    this->iterations = iterations;
}

CompareRuntime::nanoseconds CompareRuntime::test(Test& t, size_t value) {
    nanoseconds total;

    for (size_t iteration = 0; iteration < iterations; ++iteration) {
        for (size_t n = 0; n < value; ++n) {
            t.prepare();

            auto start = std::chrono::high_resolution_clock::now();
            t.execute();
            auto end = std::chrono::high_resolution_clock::now();

            total += std::chrono::duration_cast<nanoseconds>(end - start);
        }
    }

    return total / iterations;
}

void CompareRuntime::saveDat() {
    std::fstream dat(name + ".dat", std::ios::out);
    if ( !dat.is_open() ) {
        std::cerr << "Can not save \"" << name << ".dat\"" << std::endl;
        return;
    }

    for (const std::vector<double>& row : results) {
        for (size_t column = 0; column < tests.size() + 1; ++column) {
            if ( column == 0 ) {
                dat << static_cast<std::int64_t>(row[column]);
            }
            else {
                dat << "\t" << std::setprecision(9) << std::fixed << row[column];
            }
        }
    }

    dat.close();
}

void CompareRuntime::saveGnuplot(int maxExponent) {

}

void CompareRuntime::saveResult() {
    // Fix unit
    int maxExponent = 0;

    saveDat();
    saveGnuplot(maxExponent);
}

void CompareRuntime::run() {
    std::vector<double> row;

    for (size_t value = 0; value < maxValue; value += std::min(std::min((value * 10) - value, maxStep), maxValue - value)) {
        row.push_back(value);

        for (std::shared_ptr<Test> t : tests) {
            if ( t ) {
                nanoseconds duration = test(*t, value);
                row.push_back(static_cast<double>(duration.count()));
            }
            else {
                row.push_back(0);
            }
        }

        results.push_back(row);
    }

    saveResult();
}
