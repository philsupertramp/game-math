#pragma once
#include "../Random.h"
#include <cmath>
#include <cstdlib>
#include <initializer_list>
#include <iostream>
#include <memory>
#include <type_traits>


struct MatrixDimension {
    size_t rows;
    size_t columns;
};
/**
 *
 * @tparam T
 */
template<typename T = double>
class MatrixDS
{
public:
    explicit MatrixDS(T val, size_t rowCount, size_t colCount = 1) {
        Resize(rowCount, colCount);
        for(size_t i = 0; i < rows(); i++) {
            for(size_t j = 0; j < columns(); j++) { _data[i][j] = val; }
        }
    }
    MatrixDS()
        : MatrixDS(0, 1, 1) { }
    MatrixDS(std::initializer_list<std::initializer_list<T>> lst) {
        int i = 0, j = 0;
        auto rows = lst.size();
        auto cols = lst.begin()->size();
        Resize(rows, cols);
        for(const auto& l : lst) {
            for(const auto& v : l) {
                _data[i][j] = v;
                ++j;
            }
            j = 0;
            ++i;
        }
    }
    MatrixDS(MatrixDS const& other) {
        this->_rows    = other._rows;
        this->_columns = other._columns;
        this->_data    = std::move(other._data);
    };

    ~MatrixDS() {
        if(_data != nullptr) { }
    }

    static MatrixDS Random(size_t rows, size_t columns, double minWeight = 0.0, double maxWeight = 1.0) {
        MatrixDS<double> matrix(0, rows, columns);
        for(size_t i = 0; i < rows; ++i) {
            for(size_t j = 0; j < columns; ++j) { matrix[i][j] = Random::Get(minWeight, maxWeight); }
        }
        return matrix;
    }

    [[nodiscard]] inline size_t rows() const { return _rows; }
    [[nodiscard]] inline size_t columns() const { return _columns; }

    /**
     * Calculates Determinant
     * @return
     */

    [[nodiscard]] inline T Determinant() const {
        if(!HasDet()) return 0;

        if(rows() == 3 && columns() == 3) {
            return (
            _data[0][0] * _data[1][1] * _data[2][2] + _data[0][1] * _data[1][2] * _data[2][0]
            + _data[0][2] * _data[1][0] * _data[2][1] - _data[0][2] * _data[1][1] * _data[2][0]
            - _data[0][1] * _data[1][0] * _data[2][2] - _data[0][0] * _data[1][2] * _data[2][1]);
        }
        if(rows() == 2 && columns() == 2) { return _data[0][0] * _data[1][1] - _data[0][1] * _data[1][0]; }

        MatrixDS<T> submat(0.0, rows() - 1, columns() - 1);
        T d = 0;
        {
            for(size_t c = 0; c < columns(); c++) {
                size_t subi = 0; //submatrix's i value
                for(size_t i = 1; i < rows(); i++) {
                    size_t subj = 0;
                    for(size_t j = 0; j < columns(); j++) {
                        if(j == c) continue;
                        submat[subi][subj] = _data[i][j];
                        subj++;
                    }
                    subi++;
                }
                d = d + (std::pow(-1, c) * _data[0][c] * submat.Determinant());
            }
        }
        return d;
    }


    [[nodiscard]] constexpr MatrixDS<T> Transpose() const {
        MatrixDS<T> res(0, columns(), rows());
        for(size_t i = 0; i < columns(); ++i) {
            for(size_t j = 0; j < rows(); ++j) { res[i][j] = this->_data[j][i]; }
        }
        return res;
    }

    MatrixDS<T> HorizontalConcat(const MatrixDS<T>& other) {
        assert(this->rows() == other.rows());
        auto result = new MatrixDS<T>(0, this->rows(), this->columns() + other.columns());
        for(size_t i = 0; i < rows(); ++i) {
            for(size_t j = 0; j < columns() + other.columns(); ++j) {
                (*result)[i][j] = j < columns() ? _data[i][j] : other[i][j - columns()];
            }
        }
        return *result;
    }

    /** OPERATORS **/

    // Comparison

    bool operator==(const MatrixDS<T>& rhs) const {
        // Just need to check element-wise
        // Dimensions handled by implementation.
        this->assertSize(rhs);
        return elementWiseCompare(rhs);
    }
    bool operator!=(const MatrixDS<T>& rhs) const {
        return !(rhs == *this); // NOLINT
    }

    void assertSize(const MatrixDS<T>& other) const {
        assert(this->columns() == other.columns() && this->rows() && other.rows());
    }

    /**
     * Element-wise comparison
     * @param rhs
     * @return
     */
    [[nodiscard]] bool elementWiseCompare(const MatrixDS<T>& rhs) const {
        assertSize(rhs);
        for(size_t i = 0; i < rows(); i++) {
            for(size_t j = 0; j < columns(); j++) {
                if(_data[i][j] != rhs[i][j]) { return false; }
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
    MatrixDS& operator=(const MatrixDS& other) {
        if(this != &other) {
            if(rows() != other.rows() || columns() != other.columns()) { Resize(other.rows(), other.columns()); }
            for(size_t i = 0; i < rows(); i++) {
                for(size_t j = 0; j < columns(); j++) { _data[i][j] = other[i][j]; }
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
    friend MatrixDS<T> operator*(MatrixDS<T> lhs, const T& rhs) {
        auto result = new MatrixDS<T>(0.0, lhs.rows(), lhs.columns());
        for(size_t i = 0; i < lhs.rows(); i++) {
            for(size_t j = 0; j < lhs.columns(); j++) { (*result)[i][j] = lhs[i][j] * rhs; }
        }
        return *result;
    }
    MatrixDS<T>& operator*=(const T& rhs) { return *this * rhs; }

    /**
     * Regular Matrix-Matrix multiplication
     * Calculates LHS * RHS
     * @param lhs
     * @param rhs
     * @return Rows x C result matrix
     */
    friend MatrixDS<T>& operator*(const MatrixDS<T>& lhs, const MatrixDS<T>& rhs) {
        assert(lhs.columns() == rhs.rows());
        auto result = new MatrixDS<T>(0.0, lhs.rows(), rhs.columns());
        for(size_t i = 0; i < lhs.rows(); i++) {
            for(size_t j = 0; j < rhs.columns(); j++) {
                for(size_t k = 0; k < rhs.rows(); k++) { (*result)[i][j] += (T)(lhs[i][k] * rhs[k][j]); }
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
    MatrixDS& HadamardMulti(const MatrixDS& other) {
        for(size_t i = 0; i < rows(); i++) {
            for(size_t j = 0; j < columns(); j++) { _data[i][j] *= other[i][j]; }
        }
        return *this;
    }

    MatrixDS<T>& KroneckerMulti(const MatrixDS<T>& other) {
        auto result = new MatrixDS<T>(0, rows() * other.rows(), columns() * other.columns());
        for(size_t m = 0; m < rows(); m++) {
            for(size_t n = 0; n < columns(); n++) {
                for(size_t p = 0; p < other.rows(); p++) {
                    for(size_t q = 0; q < other.columns(); q++) {
                        (*result)[m * other.rows() + p][n * other.columns() + q] = _data[m][n] * other[p][q];
                    }
                }
            }
        }
        return *result;
    }

    double& sumElements() {
        auto result = new T(0.0);
        for(size_t i = 0; i < rows(); i++) {
            for(size_t j = 0; j < columns(); j++) { (*result) += _data[i][j]; }
        }
        return *result;
    }

    friend MatrixDS<T> operator+(MatrixDS<T> lhs, const MatrixDS<T>& rhs) {
        lhs.assertSize(rhs);
        auto result = new MatrixDS<T>(0, lhs.rows(), lhs.columns());
        for(size_t i = 0; i < lhs.rows(); i++) {
            for(size_t j = 0; j < lhs.columns(); j++) { (*result)[i][j] = lhs[i][j] + rhs[i][j]; }
        }
        return *result;
    }
    MatrixDS<T>& operator+=(const MatrixDS<T>& rhs) {
        (*this) = (*this) + rhs;
        return *this;
    }
    friend MatrixDS<T> operator-(MatrixDS<T> lhs, const MatrixDS<T>& rhs) {
        lhs.assertSize(rhs);
        auto result = new MatrixDS<T>(0, lhs.rows(), lhs.columns());
        for(size_t i = 0; i < lhs.rows(); i++) {
            for(size_t j = 0; j < lhs.columns(); j++) { (*result)[i][j] = lhs[i][j] - rhs[i][j]; }
        }
        return *result;
    }
    MatrixDS<T>& operator-=(const MatrixDS<T>& rhs) {
        this = (*this) - rhs;
        return *this;
    }

    // Access
    T* operator[](size_t index) { return (_data[index]); }
    const T* operator[](size_t index) const { return (_data[index]); }

    friend std::ostream& operator<<(std::ostream& ostr, MatrixDS const& m) {
        ostr << "[\n";
        for(size_t row = 0; row < m.rows(); row++) {
            ostr << '\t';
            for(size_t col = 0; col < m.columns(); col++) {
                ostr << m._data[row][col];
                if(col < m.columns() - 1) ostr << ", ";
            }
            ostr << "\n";
        }
        ostr << "]\n";
        return ostr;
    }

    void Resize(size_t rows, size_t cols) {
        _rows    = rows;
        _columns = cols;
        _data    = new T*[rows];
        for(size_t i = 0; i < rows; i++) { _data[i] = new T[cols]; }
    }

private:
    [[nodiscard]] bool HasDet() const { return columns() > 1 && rows() > 1; }

    size_t _rows    = 0;
    size_t _columns = 0;
    T** _data;
};

/**
 * Helper utilities
 */

/**
 * Element wise multiplication
 * @tparam T
 * @param lhs
 * @param rhs
 * @return
 */
template<typename T>
MatrixDS<T>& HadamardMulti(const MatrixDS<T>& lhs, const MatrixDS<T>& rhs) {
    lhs.assertSize(rhs);
    auto result = new MatrixDS<T>(0, lhs.rows(), lhs.columns());
    for(size_t i = 0; i < result->rows(); i++) {
        for(size_t j = 0; j < result->columns(); j++) { (*result)[i][j] = lhs[i][j] * rhs[i][j]; }
    }
    return *result;
}

/**
 *
 * @tparam T
 * @param lhs
 * @param rhs
 * @return
 */
template<typename T>
MatrixDS<T>& KroneckerMulti(const MatrixDS<T>& lhs, const MatrixDS<T>& rhs) {
    auto result = new MatrixDS<T>(0, lhs.rows() * rhs.rows(), lhs.columns() * rhs.columns());
    for(size_t m = 0; m < lhs.rows(); m++) {
        for(size_t n = 0; n < lhs.columns(); n++) {
            for(size_t p = 0; p < rhs.rows(); p++) {
                for(size_t q = 0; q < rhs.columns(); q++) {
                    (*result)[m * rhs.rows() + p][n * rhs.columns() + q] = lhs[m][n] * rhs[p][q];
                }
            }
        }
    }
    return *result;
}

/**
 * Horizontal concatenation of 2 matrices of same Row size
 * @tparam T
 * @param lhs
 * @param rhs
 * @return
 */
template<typename T>
MatrixDS<T> HorizontalConcat(const MatrixDS<T>& lhs, const MatrixDS<T>& rhs) {
    assert(lhs.rows() == rhs.rows());
    auto result = new MatrixDS<T>(0.0, lhs.rows(), lhs.columns() + rhs.columns());
    for(size_t i = 0; i < lhs.rows(); ++i) {
        for(size_t j = 0; j < lhs.columns() + rhs.columns(); ++j) {
            (*result)[i][j] = j < lhs.columns() ? lhs[i][j] : rhs[i][j - lhs.columns()];
        }
    }
    return *result;
}

/**
 *
 * @tparam T
 * @param A
 * @param B
 * @return
 */
template<typename T>
size_t& Corr(const MatrixDS<T>& A, const MatrixDS<T>& B) {
    A.assertSize(B);
    auto count = new size_t(0);
    for(size_t i = 0; i < A.rows(); i++) {
        for(size_t j = 0; j < A.columns(); j++) { (*count) += (A[i][j] == B[i][j]); }
    }
    return *count;
}

template<typename T>
MatrixDS<T> from_vptr(T* value, MatrixDimension size){
    auto out = new MatrixDS<T>(size.rows, size.columns);
    if(size.rows > 1 && size.columns > 1) {
        for(size_t i = 0; i < size.rows; i++) {
            for(size_t j = 0; j < size.columns; ++j) { (*out)[i][j] = value[i][j]; }
        }
    } else {
        if(size.rows > 1 && size.columns > 1) {
            for(size_t i = 0; i < size.rows; i++) {
                for(size_t j = 0; j < size.columns; ++j) { (*out)[i][j] = value[i+j]; }
            }
        }
    }
    return &out;
}

template <typename T>
size_t argmax(MatrixDS<T> mat){

}