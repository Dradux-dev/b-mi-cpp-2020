#include <iostream>
#include <random>

#include "compare-runtime.h"
#include "ti1.h"
#include "ti2.h"
#include "ti3.h"

template <typename TVec>
struct Test : public CompareRuntime::Test {
    public:
        using Dot = std::function<float(const TVec&, const TVec&)>;

        explicit inline Test(Dot f)
            : e2(rd()),
              dist(-10000, 10000),
              dot(f)
        {}

        virtual void prepare() {
            a = TVec(
                static_cast<float>(dist(e2)),
                static_cast<float>(dist(e2)),
                static_cast<float>(dist(e2))
            );

            b = TVec(
                static_cast<float>(dist(e2)),
                static_cast<float>(dist(e2)),
                static_cast<float>(dist(e2))
            );
        }

        virtual inline void execute() {
            result = dot(a, b);
        }

    private:
        std::random_device rd;
        std::mt19937 e2;
        std::uniform_real_distribution<> dist;

        Dot dot;

        TVec a;
        TVec b;

        float result;
};

int main(int, char**) {
    CompareRuntime app({
        std::make_shared<CompareRuntime::Test>(new Test<ti1::Vec3f>(
           [](const ti1::Vec3f& a, const ti1::Vec3f& b) -> float {
               return ti1::dot(a, b);
           }
        )),
        std::make_shared<CompareRuntime::Test>(new Test<ti2::Vec3f>(
           [](const ti2::Vec3f& a, const ti2::Vec3f& b) -> float {
               return static_cast<float>(ti2::dot(a, b));
           }
        )),
        std::make_shared<CompareRuntime::Test>(new Test<ti3::Vec3f>(
           [](const ti3::Vec3f& a, const ti3::Vec3f& b) -> float {
               return static_cast<float>(ti3::dot(a, b));
           }
        ))
    });

    std::cout << "Configuring application" << std::endl;
    app.configure(25, 100, 100);

    std::cout << "Running tests" << std::endl;
    app.run();

	return 0;
}

