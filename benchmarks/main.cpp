#include <chrono>
#include <iostream>

#include <math/Matrix.h>
#include <math/math.h>


class Timer
{
    std::chrono::time_point<std::chrono::system_clock> _start;
    char character;

public:
    Timer(char _c)
        : character(_c) { }
    void start() { _start = std::chrono::system_clock::now(); }
    void stop(size_t i = 1, size_t j = 1) {
        auto end                                      = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = end - _start;
        std::cout << "finished computation with " << std::to_string(i) << " elements and " << std::to_string(j) << " "
                  << character << " operations. elapsed time: " << elapsed_seconds.count() << "s\n";
    }
};

int main() {
    Timer multi_timer('*');
    Timer add_timer('+');
    int I = 3;
    int J = 5000000;

    Matrix<double> A = Matrix<double>::Random(I, I, 1, -1000.0, 1000.0);
    Matrix<double> B = Matrix<double>::Random(I, I, 1, -1000.0, 1000.0);
    mat3<double> A3(
    Random::Get(),
    Random::Get(),
    Random::Get(),
    Random::Get(),
    Random::Get(),
    Random::Get(),
    Random::Get(),
    Random::Get(),
    Random::Get());
    mat3<double> B3(
    Random::Get(),
    Random::Get(),
    Random::Get(),
    Random::Get(),
    Random::Get(),
    Random::Get(),
    Random::Get(),
    Random::Get(),
    Random::Get());

    std::cout << "Matrix<double>:" << std::endl;
    multi_timer.start();
    for(size_t j = 0; j < J; ++j) { A + B; }
    multi_timer.stop(A.rows() * A.columns(), J);

    multi_timer.start();
    for(size_t j = 0; j < J; ++j) { A.HadamardMulti(B); }
    multi_timer.stop(A.rows() * A.columns(), J);

    add_timer.start();
    for(size_t j = 0; j < J; ++j) { A* B; }
    add_timer.stop(A.rows() * A.columns(), J);

    std::cout << "mat3<double>:" << std::endl;
    multi_timer.start();
    for(size_t j = 0; j < J; ++j) { A3* B3; }
    multi_timer.stop(3 * 3, J);

    add_timer.start();
    for(size_t j = 0; j < J; ++j) { A3 + B3; }
    add_timer.stop(3 * 3, J);


    return 0;
}