#pragma once
#include "Random.h"
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <functional>
#include <initializer_list>
#include <iostream>
#include <memory>
#include <type_traits>


struct MatrixDimension {
    size_t rows;
    size_t columns;
};
/**
 * Represents a Matrix of data with type `T`
 *
 * The class is capable of regular matrix operations, including matrix - vector calculation.
 * It also holds several helper methods to calculate data science specific products or other
 * common operations.
 * @tparam T
 *
 * Element access using **matrix(row, column)** notation
 *
 * Accessed elements can be modified!
 */
template<typename T = double>
class Matrix
{
public:
    explicit Matrix(T val, size_t rowCount, size_t colCount = 1) {
        Resize(rowCount, colCount);
        for(size_t i = 0; i < _rows; i++) {
            for(size_t j = 0; j < _columns; j++) { _data[GetIndex(i, j)] = val; }
        }
    }
    explicit Matrix(T* val, size_t colCount) {
        Resize(1, colCount);
        for(size_t j = 0; j < _columns; j++) { _data[GetIndex(0, j)] = val[j]; }
    }
    Matrix() = default;

    Matrix(std::initializer_list<std::initializer_list<T>> lst) {
        int i = 0, j = 0;
        auto rows = lst.size();
        auto cols = lst.begin()->size();
        Resize(rows, cols);
        for(const auto& l : lst) {
            for(const auto& v : l) {
                _data[GetIndex(i, j)] = v;
                ++j;
            }
            j = 0;
            ++i;
        }
    }
    Matrix(Matrix const& other) {
        this->_rows    = other._rows;
        this->_columns = other._columns;
        this->_data    = std::move(other._data);
    };

    ~Matrix() {
        if(_data != nullptr) { }
    }

    /**
     * Generates a random matrix
     * @param rows number of rows in target matrix
     * @param columns number of columns in target matrix
     * @param minValue
     * @param maxValue
     * @return matrix of dimension `rows`, `columns` initialized with random values from `minValue` to `maxValue`
     */
    static Matrix Random(size_t rows, size_t columns, double minValue = 0.0, double maxValue = 1.0) {
        Matrix<double> matrix(0, rows, columns);
        for(size_t i = 0; i < rows; ++i) {
            for(size_t j = 0; j < columns; ++j) { matrix(i, j) = Random::Get(minValue, maxValue); }
        }
        return matrix;
    }

    /**
     * row getter
     * @return
     */
    [[nodiscard]] inline size_t rows() const { return _rows; }
    /**
     * columns getter
     * @return
     */
    [[nodiscard]] inline size_t columns() const { return _columns; }

    /**
     * Calculates Determinant
     * @return
     */

    [[nodiscard]] inline T Determinant() const {
        if(!HasDet()) return 0;

        if(_rows == 3 && _columns == 3) {
            return (
            _data[GetIndex(0, 0)] * _data[GetIndex(1, 1)] * _data[GetIndex(2, 2)]
            + _data[GetIndex(0, 1)] * _data[GetIndex(1, 2)] * _data[GetIndex(2, 0)]
            + _data[GetIndex(0, 2)] * _data[GetIndex(1, 0)] * _data[GetIndex(2, 1)]
            - _data[GetIndex(0, 2)] * _data[GetIndex(1, 1)] * _data[GetIndex(2, 0)]
            - _data[GetIndex(0, 1)] * _data[GetIndex(1, 0)] * _data[GetIndex(2, 2)]
            - _data[GetIndex(0, 0)] * _data[GetIndex(1, 2)] * _data[GetIndex(2, 1)]);
        }
        if(_rows == 2 && _columns == 2) {
            return _data[GetIndex(0, 0)] * _data[GetIndex(1, 1)] - _data[GetIndex(0, 1)] * _data[GetIndex(1, 0)];
        }

        Matrix<T> submat(0.0, _rows - 1, _columns - 1);
        T d = 0;
        {
            for(size_t c = 0; c < _columns; c++) {
                size_t subi = 0; //sub-matrix's i value
                for(size_t i = 1; i < _rows; i++) {
                    size_t subj = 0;
                    for(size_t j = 0; j < _columns; j++) {
                        if(j == c) continue;
                        submat._data[submat.GetIndex(subi, subj)] = _data[GetIndex(i, j)];
                        subj++;
                    }
                    subi++;
                }
                d = d + (std::pow(-1, c) * _data[GetIndex(0, c)] * submat.Determinant());
            }
        }
        return d;
    }

    /**
     * Creates transposed matrix of `this`
     * @return
     */
    [[nodiscard]] constexpr Matrix<T> Transpose() const {
        Matrix<T> res(0, columns(), rows());
        for(size_t i = 0; i < columns(); ++i) {
            for(size_t j = 0; j < rows(); ++j) { res._data[res.GetIndex(i, j)] = _data[GetIndex(j, i)]; }
        }
        return res;
    }

    /**
     * Horizontal matrix concatenation
     * @param other Matrix with same number of rows, dimension n1, m2
     * @return concatenated matrix of [this, other] with dimension n1, m1 + m2
     */
    Matrix<T> HorizontalConcat(const Matrix<T>& other) {
        assert(this->rows() == other.rows());
        auto result = new Matrix<T>(0, this->rows(), this->columns() + other.columns());
        for(size_t i = 0; i < rows(); ++i) {
            for(size_t j = 0; j < columns() + other.columns(); ++j) {
                (*result)(i, j) = j < columns() ? _data[GetIndex(i, j)] : other(i, j - columns());
            }
        }
        return *result;
    }

    /** OPERATORS **/

    // Comparison

    bool operator==(const Matrix<T>& rhs) const {
        // Just need to check element-wise
        // Dimensions handled by implementation.
        this->assertSize(rhs);
        return elementWiseCompare(rhs);
    }
    bool operator!=(const Matrix<T>& rhs) const {
        return !(rhs == *this); // NOLINT
    }

    /**
     * Helper to check for equal dimensions
     * @param other
     */
    void assertSize(const Matrix<T>& other) const { assert(_columns == other.columns() && _rows && other.rows()); }

    /**
     * Element-wise comparison
     * @param rhs
     * @return
     */
    [[nodiscard]] bool elementWiseCompare(const Matrix<T>& rhs) const {
        assertSize(rhs);
        for(size_t i = 0; i < rows(); i++) {
            for(size_t j = 0; j < columns(); j++) {
                if(_data[GetIndex(i, j)] != rhs(i, j)) { return false; }
            }
        }
        return true;
    }


    // Assignment
    /**
     * careful, actually overrides different sized matrices, just like other languages (python, matlab)
     * @param other
     * @return
     */
    Matrix& operator=(const Matrix& other) {
        if(this != &other) {
            if((this == NULL) || (_rows != other.rows() || _columns != other.columns())) {
                Resize(other.rows(), other.columns());
            }
            for(size_t i = 0; i < _rows; i++) {
                for(size_t j = 0; j < _columns; j++) { _data[GetIndex(i, j)] = other(i, j); }
            }
        }
        return *this;
    }

    // Math

    /**
     * Simple Matrix scalar multiplication
     * @param lhs
     * @param rhs
     * @return scaled matrix
     */
    friend Matrix<T> operator*(Matrix<T> lhs, const T& rhs) {
        auto result = new Matrix<T>(0.0, lhs.rows(), lhs.columns());
        for(size_t i = 0; i < lhs.rows(); i++) {
            for(size_t j = 0; j < lhs.columns(); j++) { (*result)(i, j) = lhs(i, j) * rhs; }
        }
        return *result;
    }
    /**
     * Simple Matrix scalar multiplication
     * @param lambda
     * @param A
     * @return scaled matrix lambda * A = B with B(i, j) = lambda * A(i, j)
     */
    friend Matrix<T> operator*(T lambda, const Matrix<T>& A) {
        auto result = new Matrix<T>(0.0, A.rows(), A.columns());
        for(size_t i = 0; i < A.rows(); i++) {
            for(size_t j = 0; j < A.columns(); j++) { (*result)(i, j) = A(i, j) * lambda; }
        }
        return *result;
    }
    Matrix<T>& operator*=(const T& rhs) { return *this * rhs; }

    /**
     * Regular Matrix-Matrix multiplication
     * Calculates LHS * RHS
     * @param lhs
     * @param rhs
     * @return Rows x C result matrix
     */
    friend Matrix<T>& operator*(const Matrix<T>& lhs, const Matrix<T>& rhs) {
        assert(lhs.columns() == rhs.rows());
        auto result = new Matrix<T>(0.0, lhs.rows(), rhs.columns());
        for(size_t i = 0; i < lhs.rows(); i++) {
            for(size_t j = 0; j < rhs.columns(); j++) {
                for(size_t k = 0; k < rhs.rows(); k++) { (*result)(i, j) += (T)(lhs(i, k) * rhs(k, j)); }
            }
        }
        return *result;
    }

    /**
     * Hadamard Multiplication
     * Z[i][j] = A[i][j] * B[i][j]
     * @param other
     * @return
     */
    Matrix& HadamardMulti(const Matrix& other) {
        for(size_t i = 0; i < rows(); i++) {
            for(size_t j = 0; j < columns(); j++) { _data[GetIndex(i, j)] *= other(i, j); }
        }
        return *this;
    }

    /**
     * A form of matrix multiplication
     * For explicit reference please consult https://en.wikipedia.org/wiki/Kronecker_product
     * @param other right hand side with same dimension
     * @return resulting matrix with same dimension
     */
    Matrix<T>& KroneckerMulti(const Matrix<T>& other) {
        auto result = new Matrix<T>(0, rows() * other.rows(), columns() * other.columns());
        for(size_t m = 0; m < rows(); m++) {
            for(size_t n = 0; n < columns(); n++) {
                for(size_t p = 0; p < other.rows(); p++) {
                    for(size_t q = 0; q < other.columns(); q++) {
                        (*result)(m * other.rows() + p, n * other.columns() + q) = _data[GetIndex(m, n)] * other(p, q);
                    }
                }
            }
        }
        return *result;
    }

    double sumElements() {
        T result = T(0.0);
        for(size_t i = 0; i < rows(); i++) {
            for(size_t j = 0; j < columns(); j++) { result += _data[GetIndex(i, j)]; }
        }
        return result;
    }

    friend Matrix<T> operator+(Matrix<T> lhs, const Matrix<T>& rhs) {
        lhs.assertSize(rhs);
        auto result = new Matrix<T>(0, lhs.rows(), lhs.columns());
        for(size_t i = 0; i < lhs.rows(); i++) {
            for(size_t j = 0; j < lhs.columns(); j++) { (*result)(i, j) = lhs(i, j) + rhs(i, j); }
        }
        return *result;
    }
    Matrix<T>& operator+=(const Matrix<T>& rhs) {
        (*this) = (*this) + rhs;
        return *this;
    }
    friend Matrix<T> operator-(Matrix<T> lhs, const Matrix<T>& rhs) {
        lhs.assertSize(rhs);
        auto result = new Matrix<T>(0, lhs.rows(), lhs.columns());
        for(size_t i = 0; i < lhs.rows(); i++) {
            for(size_t j = 0; j < lhs.columns(); j++) { (*result)(i, j) = lhs(i, j) - rhs(i, j); }
        }
        return *result;
    }
    Matrix<T>& operator-=(const Matrix<T>& rhs) {
        this = (*this) - rhs;
        return *this;
    }

    // Access

    // Element access
    T& operator()(const size_t& row, const size_t& column) { return _data[GetIndex(row, column)]; }
    T& operator()(const size_t& row, const size_t& column) const { return _data[GetIndex(row, column)]; }

    // row vector access
    Matrix<T> operator()(const size_t& row) { return Matrix<T>(&_data[row * _columns], _columns); }
    Matrix<T> operator()(const size_t& row) const { return Matrix<T>(&_data[row * _columns], _columns); }

    void SetRow(const size_t& index, const Matrix<T>& other) {
        bool isInColumns = other.columns() > other.rows();
        auto colCount    = isInColumns ? other.columns() : other.rows();
        assert(colCount == _columns);
        for(int i = 0; i < colCount; ++i) {
            _data[GetIndex(index, i)] = other(isInColumns ? 0 : i, isInColumns ? i : 0);
        }
    }

    friend std::ostream& operator<<(std::ostream& ostr, const Matrix& m) {
        ostr.precision(17);
        ostr << "[\n";
        for(size_t row = 0; row < m.rows(); row++) {
            ostr << '\t';
            for(size_t col = 0; col < m.columns(); col++) {
                ostr << m._data[m.GetIndex(row, col)];
                if(col < m.columns() - 1) ostr << ", ";
            }
            ostr << "\n";
        }
        ostr << "]\n";
        return ostr;
    }

    /**
     * Resizes a matrix
     * @param rows target number of rows
     * @param cols target number of columns
     */
    void Resize(size_t rows, size_t cols) {
        _rows    = rows;
        _columns = cols;
        _data    = new T[rows * cols];
    }

    /**
     *
     * @param row \in [0, rows() - 1]
     * @param col \in [0, columns() - 1]
     * @return col + row * columns()
     */
    [[nodiscard]] inline int GetIndex(const size_t& row, const size_t& col) const {
        assert(row < _rows && col < _columns);
        return col + row * _columns;
    }

private:
    /**
     * Helper to test if Matrix can have a determinant
     * @return
     */
    [[nodiscard]] bool HasDet() const { return columns() > 1 && rows() > 1; }


    size_t _rows    = 0;
    size_t _columns = 0;
    T* _data;
};

/**
 * Helper utilities
 */

/**
 * Element wise multiplication
 * @tparam T value type
 * @param lhs left hand side with dimension n1, m1
 * @param rhs right hand side with dimension n1, m1
 * @return product of element wise multiplication of lhs * rhs with dimension n1, m1
 */
template<typename T>
Matrix<T>& HadamardMulti(const Matrix<T>& lhs, const Matrix<T>& rhs) {
    lhs.assertSize(rhs);
    auto result = new Matrix<T>(0, lhs.rows(), lhs.columns());
    for(size_t i = 0; i < result->rows(); i++) {
        for(size_t j = 0; j < result->columns(); j++) { (*result)(i, j) = lhs(i, j) * rhs(i, j); }
    }
    return *result;
}

/**
 * A form of matrix multiplication
 * For explicit reference please consult https://en.wikipedia.org/wiki/Kronecker_product
 * @tparam T value type
 * @param lhs left hand side with dimension n1, m1
 * @param rhs right hand side with dimension n2, m2
 * @return resulting matrix with dimension n1 * n2, m1 * m2
 */
template<typename T>
Matrix<T>& KroneckerMulti(const Matrix<T>& lhs, const Matrix<T>& rhs) {
    auto result = new Matrix<T>(0, lhs.rows() * rhs.rows(), lhs.columns() * rhs.columns());
    for(size_t m = 0; m < lhs.rows(); m++) {
        for(size_t n = 0; n < lhs.columns(); n++) {
            for(size_t p = 0; p < rhs.rows(); p++) {
                for(size_t q = 0; q < rhs.columns(); q++) {
                    (*result)(m * rhs.rows() + p, n * rhs.columns() + q) = lhs(m, n) * rhs(p, q);
                }
            }
        }
    }
    return *result;
}

/**
 * Horizontal concatenation of 2 matrices of same Row size
 * @tparam T value type
 * @param lhs left hand side with dimension n1, m1
 * @param rhs right hand side with dimension n1, m2
 * @return concatenated matrix [lhs, rhs] of dimension n1, m1 + m2
 */
template<typename T>
Matrix<T> HorizontalConcat(const Matrix<T>& lhs, const Matrix<T>& rhs) {
    assert(lhs.rows() == rhs.rows());
    auto result = new Matrix<T>(0.0, lhs.rows(), lhs.columns() + rhs.columns());
    for(size_t i = 0; i < lhs.rows(); ++i) {
        for(size_t j = 0; j < lhs.columns() + rhs.columns(); ++j) {
            (*result)(i, j) = j < lhs.columns() ? lhs(i, j) : rhs(i, j - lhs.columns());
        }
    }
    return *result;
}

/**
 * Counts correlations between `A` and `B`
 * @tparam T value type of `A` and `B`
 * @param A matrix to compare
 * @param B matrix to compare with
 * @return number of element wise equal elements
 */
template<typename T>
size_t& Corr(const Matrix<T>& A, const Matrix<T>& B) {
    A.assertSize(B);
    auto count = new size_t(0);
    for(size_t i = 0; i < A.rows(); i++) {
        for(size_t j = 0; j < A.columns(); j++) { (*count) += (A(i, j) == B(i, j)); }
    }
    return *count;
}

/**
 * Converts array of elements of type `T` into matrix of given `size`
 * @tparam T value type
 * @param value pointer to array of elements of type `T`
 * @param size dimension of target matrix
 * @return
 */
template<typename T>
Matrix<T>& from_vptr(const T* value, MatrixDimension size) {
    auto out = new Matrix<T>(0, size.rows, size.columns);
    for(size_t i = 0; i < size.rows; i++) {
        for(size_t j = 0; j < size.columns; ++j) { (*out)(i, j) = value[i * size.columns + j]; }
    }
    return *out;
}

/**
 * Search index of value with maximum value
 * **Caution!** This value does represent the index in a ongoing array.
 * Example:
 *      A = {{1,1}, {1,2}}
 *      argmax(A) == 3
 *      A(argmax(A)//A.colums(), argmax(A)%A.columns()) == 2
 *
 * @tparam T
 * @param mat
 * @return
 */
template<typename T>
size_t argmax(const Matrix<T>& mat) {
    T maxVal        = std::numeric_limits<T>::min();
    size_t maxIndex = -1;
    for(size_t i = 0; i < mat.rows(); i++) {
        for(size_t j = 0; j < mat.columns(); j++) {
            if(mat(i, j) > maxVal) {
                maxVal   = mat(i, j);
                maxIndex = i * mat.columns() + j;
            }
        }
    }
    return maxIndex;
}

/**
 * Search index of value with lowest value
 * **Caution!** This value does represent the index in a ongoing array.
 * Example:
 *      A = {{3,3}, {3,2}}
 *      argmin(A) == 3
 *      A(argmin(A)//A.colums(), argmin(A)%A.columns()) == 2
 *
 * @tparam T matrix value type
 * @param mat element to search in
 * @return index of minimal value
 */
template<typename T>
size_t argmin(const Matrix<T>& mat) {
    T maxVal        = std::numeric_limits<T>::max();
    size_t maxIndex = -1;
    for(size_t i = 0; i < mat.rows(); i++) {
        for(size_t j = 0; j < mat.columns(); j++) {
            if(mat(i, j) < maxVal) {
                maxVal   = mat(i, j);
                maxIndex = i * mat.columns() + j;
            }
        }
    }
    return maxIndex;
}

/**
 * Returns elements chosen from `valIfTrue` or `valIfFalse` depending on `condition`
 * @tparam T value type
 * @param condition lambda/function to test a condition on every element of `in`
 * @param in input values
 * @param valIfTrue value vector with values in case `in` meets `condition`
 * @param valIfFalse value vector with values in case `in` does not meet `condition`
 * @return
 */
template<typename T>
Matrix<T> where(
const std::function<bool(T)>& condition, const Matrix<T>& in, const Matrix<T>& valIfTrue, const Matrix<T>& valIfFalse) {
    assert(valIfTrue.columns() == valIfFalse.columns() && valIfTrue.rows() == valIfFalse.rows());
    bool refVector = true;
    if((valIfTrue.columns() == valIfTrue.rows()) == 1) { refVector = false; }
    auto out = refVector ? valIfTrue : in;

    for(size_t i = 0; i < in.rows(); i++) {
        for(size_t j = 0; j < in.columns(); j++) {
            if(refVector) {
                if(!condition(in(i, j))) out(i, j) = valIfFalse(i, j);
            } else {
                out(i, j) = condition(in(i, j)) ? valIfTrue(0, 0) : valIfFalse(0, 0);
            }
        }
    }
    return out;
}

/**
 * Converts two input matrices into a vector of
 * row-wise pairs of `a` and `b`
 *
 * Example:
 *  a = {{1,1,1}, {2,2,2}, {3,3,3}}
 *  b = {{0}, {1}, {2}}
 *
 *  zip(a,b) == {({1,1,1}, {0}), ({2,2,2}, {1}), ({3,3,3}, {2})}
 * @tparam T value type
 * @param a
 * @param b
 * @return
 */
template<typename T>
std::vector<std::pair<Matrix<T>, Matrix<T>>> zip(const Matrix<T>& a, const Matrix<T>& b) {
    std::vector<std::pair<Matrix<T>, Matrix<T>>> out(a.rows());
    for(size_t i = 0; i < a.rows(); i++) {
        Matrix<T> subA, subB;
        subA.Resize(1, a.columns());
        subB.Resize(1, b.columns());
        for(size_t j = 0; j < a.columns(); j++) { subA(0, j) = a(i, j); }
        for(size_t j = 0; j < b.columns(); j++) { subB(0, j) = b(i, j); }

        out[i] = { subA, subB };
    }
    return out;
}