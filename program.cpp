#include <stdio.h>
#include <omp.h>
#include <time.h>
#include "Matrix.hpp"
using namespace std;


int main(int argc, const char** argv)
{
    Matrix<size_t> m1 = Matrix<size_t>(3,6);
    Matrix<size_t> m2 = Matrix<size_t>(6,3);

    m1 *= m2;
    
    return 0;
}



