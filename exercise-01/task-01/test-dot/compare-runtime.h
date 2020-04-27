#pragma once

#include <chrono>
#include <cstdint>
#include <functional>
#include <initializer_list>
#include <memory>
#include <string>
#include <vector>

class CompareRuntime {
    public:
        using size_t = std::size_t;
        using duration = double;
        using nanoseconds = std::chrono::nanoseconds;

        class Test {
            public:
                virtual ~Test() = default;

                virtual void prepare() = 0;
                virtual void execute() = 0;
        };

        inline CompareRuntime(const std::initializer_list<std::shared_ptr<Test>>& l)
            : tests(l)
        {}

        void configure(size_t maxStep, size_t maxValue, size_t iterations = 100);
        nanoseconds test(Test& t, size_t value);
        void saveDat();
        void saveGnuplot(int maxExponent);
        void saveResult();
        void run();

    protected:
        std::vector<std::vector<double>> results;
        std::vector<std::shared_ptr<Test>> tests;
        size_t maxValue;
        size_t maxStep;
        size_t iterations;
        std::string name;
};
