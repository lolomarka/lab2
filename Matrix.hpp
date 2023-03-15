#include <omp.h>
#include <vector>
#include <iostream>

using namespace std;

template <
    typename T,
    typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
class Matrix
{
private:
    size_t mRows = 0;
    size_t mCols = 0;
    vector<T> mData;

    bool validateIndexes(size_t row, size_t col) const
    {
        if (row < 0 || row >= mRows)
            return false;
        if (col < 0 || col >= mCols)
            return false;
        return true;
    }

public:
    size_t nRows() const { return mRows; }
    size_t nCols() const { return mCols; }

    Matrix()
    {
        mRows = mCols = 0;
    }

    Matrix(size_t dimension)
    {
        Matrix(dimension, dimension);
    }

    Matrix(size_t rows, size_t cols)
        : mRows(rows),
          mCols(cols),
          mData(rows * cols)
    {
        mRows = rows;
        mCols = cols;
    }

    Matrix(size_t rows, size_t cols, const vector<T> &data)
        : mRows(rows),
          mCols(cols)
    {
        if (data.size() != rows * cols)
            throw invalid_argument("Размерность матрицы не совпадает с инициализирующим вектором.");
        mData = data;
    }

    T &operator()(size_t i, size_t j)
    {
        validateIndexes(i, j);
        return mData[i * mCols + j];
    }

    void Print()
    {
        for (size_t i = 0; i < mCols; i++)
        {
            for (size_t j = 0; j < mRows; j++)
            {
                cout << operator()(i,j) << "\t";
            }
            cout << endl;
        }
    }

    T operator()(size_t i, size_t j) const
    {
        validateIndexes(i, j);
        return mData[i * mCols + j];
    }

    template <size_t N>
    Matrix(size_t rows, size_t cols, T (&data)[N])
    {
        if (N != rows * cols)
            throw invalid_argument("Размерность матрицы не совпадает с инициализирующим вектором.");
        vector<T> v(data, data + N);
        Matrix(rows, cols, v);
    }

    Matrix &operator*=(Matrix &other)
    {
        if (mCols != other.mRows)
            throw invalid_argument(
                "Cannot multiply these matrices: L " + to_string(mRows) + "x" +
                to_string(mCols) + ", R " + to_string(other.mRows) + "x" + to_string(other.mCols));

        Matrix result(mRows, other.mCols);

        for (size_t i = 0; i < result.mRows; i++)
        {
            for (size_t k = 0; k < result.mCols; k++)
            {
                for (size_t j = 0; j < mCols; j++)
                    result(i, k) += operator()(i, j) * other(j, k);
            }
        }

        mRows = result.mRows;
        mCols = result.mCols;
        mData = result.mData;
        return *this;
    }

    Matrix MultiplyParallel(Matrix &other)
    {
        if (mCols != other.mRows)
            throw invalid_argument(
                "Cannot multiply these matrices: L " + to_string(mRows) + "x" +
                to_string(mCols) + ", R " + to_string(other.mRows) + "x" + to_string(other.mCols));

        Matrix result(mRows, other.mCols);

        #pragma omp parallel for shared(&this, other, result) private(i,j,k)
        for (size_t i = 0; i < result.mRows; i++)
        {
            for (size_t k = 0; k < result.mCols; k++)
            {
                for (size_t j = 0; j < mCols; j++)
                    result(i, k) += operator()(i, j) * other(k, j);
            }
        }
        return result;
    }
};