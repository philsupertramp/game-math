#pragma once
#include <cmath>
#include <cstdlib>
#include <initializer_list>
#include <iostream>
#include <type_traits>


template<size_t Rows, size_t Columns = 1, typename T = double>
class MatrixDS
{
    //    static_assert(Columns > 0 && Rows > 0, "Cannot initialize empty matrix.");

public:
    explicit MatrixDS(T val) {
        for(size_t i = 0; i < Rows; i++) {
            for(size_t j = 0; j < Columns; j++) { _data[i][j] = val; }
        }
    }
    MatrixDS()
        : MatrixDS(0) { }
    MatrixDS(std::initializer_list<std::initializer_list<T>> lst) {
        int i = 0, j = 0;
        for(const auto& l : lst) {
            for(const auto& v : l) {
                _data[i][j] = v;
                ++j;
            }
            j = 0;
            ++i;
        }
    }
    MatrixDS(MatrixDS const&) = default;

    ~MatrixDS() {
        if(_data != nullptr) { }
    }

    [[nodiscard]] constexpr inline size_t rows() const { return Rows; }
    [[nodiscard]] constexpr inline size_t columns() const { return Columns; }

    /**
     * Calculates Determinant
     * @return
     */

    [[nodiscard]] inline T Determinant() const {
        if(!HasDet()) return 0;

        MatrixDS<Rows - 1, Columns - 1, T> submat(0);
        T d = 0;
        {
            for(size_t c = 0; c < Columns; c++) {
                size_t subi = 0; //submatrix's i value
                for(size_t i = 1; i < Rows; i++) {
                    size_t subj = 0;
                    for(size_t j = 0; j < Columns; j++) {
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


    [[nodiscard]] constexpr MatrixDS<Columns, Rows, T> Transpose() const {
        MatrixDS<Columns, Rows, T> res;
        for(size_t i = 0; i < Columns; ++i) {
            for(size_t j = 0; j < Rows; ++j) { res[i][j] = this->_data[j][i]; }
        }
        return res;
    }

    template<size_t C>
    MatrixDS<Rows, Columns + C, T> HorizontalConcat(const MatrixDS<Rows, C, T>& other) {
        auto result = new MatrixDS<Rows, Columns + C, T>();
        for(size_t i = 0; i < Rows; ++i) {
            for(size_t j = 0; j < Columns + C; ++j) {
                (*result)[i][j] = j < columns() ? _data[i][j] : other[i][j - columns()];
            }
        }
        return *result;
    }

    /** OPERATORS **/

    // Comparison

    bool operator==(const MatrixDS<Rows, Columns, T>& rhs) const {
        // Just need to check element-wise
        // Dimensions handled by implementation.
        return elementWiseCompare(rhs);
    }
    bool operator!=(const MatrixDS<Rows, Columns, T>& rhs) const {
        return !(rhs == *this); // NOLINT
    }

    /**
     * Element-wise comparison
     * @param rhs
     * @return
     */
    [[nodiscard]] bool elementWiseCompare(const MatrixDS<Rows, Columns, T>& rhs) const {
        for(size_t i = 0; i < Rows; i++) {
            for(size_t j = 0; j < Columns; j++) {
                if(_data[i][j] != rhs[i][j]) { return false; }
            }
        }
        return true;
    }


    // Assignment
    MatrixDS& operator=(const MatrixDS& other) {
        if(this != &other) {
            for(size_t i = 0; i < Rows; i++) {
                for(size_t j = 0; j < Columns; j++) { _data[i][j] = other[i][j]; }
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
    friend MatrixDS<Rows, Columns, T> operator*(MatrixDS<Rows, Columns, T> lhs, const T& rhs) { return lhs *= rhs; }
    MatrixDS<Rows, Columns, T>& operator*=(const T& rhs) {
        for(size_t i = 0; i < Rows; ++i) {
            for(size_t j = 0; j < Columns; ++j) { _data[i][j] *= rhs; }
        }
        return *this;
    }

    /**
     * Regular Matrix-Matrix multiplication
     * Calculates LHS * RHS
     * @tparam R
     * @tparam C
     * @param lhs
     * @param rhs
     * @return Rows x C result matrix
     */
    template<size_t R, size_t C, class = typename std::enable_if_t<Columns == R>>
    friend MatrixDS<Rows, C, T>& operator*(const MatrixDS<Rows, Columns, T>& lhs, const MatrixDS<R, C, T>& rhs) {
        auto result = new MatrixDS<Rows, C, T>(0.0);
        for(size_t i = 0; i < Rows; i++) {
            for(size_t j = 0; j < C; j++) {
                for(size_t k = 0; k < R; k++) { (*result)[i][j] += (T)(lhs[i][k] * rhs[k][j]); }
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
        for(size_t i = 0; i < Rows; i++) {
            for(size_t j = 0; j < Columns; j++) { _data[i][j] *= other[i][j]; }
        }
        return *this;
    }

    template<size_t R, size_t C>
    MatrixDS<Rows * R, Columns * C, T>& KroneckerMulti(const MatrixDS<R, C, T>& other) {
        auto result = new MatrixDS<Rows * R, Columns * C, T>(0);
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
        for(size_t i = 0; i < Rows; i++) {
            for(size_t j = 0; j < Columns; j++) { (*result) += _data[i][j]; }
        }
        return *result;
    }

    friend MatrixDS<Rows, Columns, T> operator+(MatrixDS<Rows, Columns, T> lhs, const MatrixDS<Rows, Columns, T>& rhs) {
        return lhs += rhs;
    }
    MatrixDS<Rows, Columns, T>& operator+=(const MatrixDS<Rows, Columns, T>& rhs) {
        for(size_t i = 0; i < Rows; i++) {
            for(size_t j = 0; j < Columns; j++) { _data[i][j] += rhs[i][j]; }
        }
        return *this;
    }
    friend MatrixDS<Rows, Columns, T> operator-(MatrixDS<Rows, Columns, T> lhs, const MatrixDS<Rows, Columns, T>& rhs) {
        return lhs -= rhs;
    }
    MatrixDS<Rows, Columns, T>& operator-=(const MatrixDS<Rows, Columns, T>& rhs) {
        for(size_t i = 0; i < Rows; i++) {
            for(size_t j = 0; j < Columns; j++) { _data[i][j] -= rhs[i][j]; }
        }
        return *this;
    }

    // Access
    T* operator[](size_t index) { return _data[index]; }
    const T* operator[](size_t index) const { return _data[index]; }

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

private:
    [[nodiscard]] constexpr bool HasDet() const { return Columns > 1 && Rows > 1; }
    T _data[Rows][Columns];
};

template<typename T>
class MatrixDS<1, 1, T>
{
public:
    explicit MatrixDS(T val) { _data = val; }
    MatrixDS() = default;

    [[nodiscard]] constexpr inline size_t rows() const { return 1; }
    [[nodiscard]] constexpr inline size_t columns() const { return 1; }

    T* operator[]([[maybe_unused]] size_t index) { return &_data; }
    const T* operator[]([[maybe_unused]] size_t index) const { return &_data; }

    inline T Determinant() const { return 0; }

private:
    T _data = {};
};

/**
 * Determinant overrides for faster calculation of 2x2 and 3x3 matrices
 */

template<>
double MatrixDS<2, 2, double>::Determinant() const {
    return _data[0][0] * _data[1][1] - _data[0][1] * _data[1][0];
}
template<>
double MatrixDS<3, 3, double>::Determinant() const {
    return (
    _data[0][0] * _data[1][1] * _data[2][2] + _data[0][1] * _data[1][2] * _data[2][0]
    + _data[0][2] * _data[1][0] * _data[2][1] - _data[0][2] * _data[1][1] * _data[2][0]
    - _data[0][1] * _data[1][0] * _data[2][2] - _data[0][0] * _data[1][2] * _data[2][1]);
}

/**
 * Helper utilities
 */
template<size_t R, size_t C, typename T>
MatrixDS<R, C, T>& HadamardMulti(const MatrixDS<R, C, T>& lhs, const MatrixDS<R, C, T>& rhs) {
    auto result = new MatrixDS<R, C, T>(0.0);
    for(size_t i = 0; i < R; i++) {
        for(size_t j = 0; j < C; j++) { (*result)[i][j] = lhs[i][j] * rhs[i][j]; }
    }
    return *result;
}

template<size_t Rows, size_t Columns, typename T, size_t R, size_t C>
MatrixDS<Rows * R, Columns * C, T>&
KroneckerMulti(const MatrixDS<Rows, Columns, T>& lhs, const MatrixDS<R, C, T>& rhs) {
    auto result = new MatrixDS<Rows * R, Columns * C, T>(0);
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

template<size_t Rows, size_t Columns, typename T, size_t C>
MatrixDS<Rows, Columns + C, T>
HorizontalConcat(const MatrixDS<Rows, Columns, T>& lhs, const MatrixDS<Rows, C, T>& rhs) {
    auto result = new MatrixDS<Rows, Columns + C, T>();
    for(size_t i = 0; i < Rows; ++i) {
        for(size_t j = 0; j < Columns + C; ++j) {
            (*result)[i][j] = j < lhs.columns() ? lhs[i][j] : rhs[i][j - lhs.columns()];
        }
    }
    return *result;
}

template<size_t N, size_t M, typename T>
size_t& Corr(const MatrixDS<N, M, T>& A, const MatrixDS<N, M, T>& B) {
    auto count = new size_t(0);
    for(size_t i = 0; i < N; i++) {
        for(size_t j = 0; j < M; j++) { (*count) += (A[i][j] == B[i][j]); }
    }
    return *count;
}