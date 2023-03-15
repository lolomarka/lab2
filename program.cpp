#include <stdio.h>
#include <chrono>
#include "Matrix.hpp"
using namespace std;

chrono::milliseconds seq()
{
    Matrix<size_t> m1 = Matrix<size_t>(1000,1000);
    Matrix<size_t> m2 = Matrix<size_t>(1000,1000);

    auto start = chrono::steady_clock::now();
    m1 *= m2;
    auto end = chrono::steady_clock::now();
    auto time = chrono::duration_cast<chrono::milliseconds>(end - start);
    m1.Print();
    return time;
}

chrono::milliseconds par()
{
    Matrix<size_t> m1 = Matrix<size_t>(1000,1000);
    Matrix<size_t> m2 = Matrix<size_t>(1000,1000);

    auto start = chrono::steady_clock::now();
    m1.MultiplyParallel(m2);
    auto end = chrono::steady_clock::now();
    auto time = chrono::duration_cast<chrono::milliseconds>(end - start);
    m1.Print();
    return time;
}

int main(int argc, const char** argv)
{
    auto a1 = seq();

    auto a2 = par();
    cout << "Время умножения (без Parallel): " << a1.count() << " мс\n";
    cout << "Время умножения (с Parallel)  : " << a2.count() << " мс\n";

    return 0;
}




