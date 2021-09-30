#pragma once
#include "Random.h"
#include "vec/vec3.h"
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <initializer_list>
#include <iostream>
#include <memory>
#include <type_traits>


/**
 * holds dimensions of a Matrix
 */
struct MatrixDimension {
    //! number rows
    size_t rows;
    //! number columns
    size_t columns;
    //! number elements
    size_t elemDim = 1;
};
/**
 * Represents a multi dimensional Matrix of data with type `T`
 *
 * The class is capable of regular matrix operations, including matrix - vector calculation.
 * It also holds several helper methods to calculate data science specific products or other
 * common operations.
 * @tparam T
 *
 * Element access using **matrix(row, column, element)** notation
 *
 * Accessed elements can be modified!
 */
template<typename T = double>
class Matrix
{
public:
    /**
     * Default constructor
     * @param val default value for all elements
     * @param rowCount number of rows
     * @param colCount number of columns
     * @param elementDimension number of dimensions per element
     */
    explicit Matrix(T val, size_t rowCount, size_t colCount, size_t elementDimension = 1) {
        Resize(rowCount, colCount, elementDimension);
        for(size_t i = 0; i < _rows; ++i) {
            for(size_t j = 0; j < _columns; ++j) {
                for(size_t c = 0; c < _element_size; ++c) { _data[GetIndex(i, j, c)] = val; }
            }
        }
    }

    /**
     * Vector-assignment, assigns colCount values of type T
     * @param val pointer representation of array
     * @param colCount number of elements within val
     */
    explicit Matrix(T* val, size_t colCount) {
        Resize(1, colCount);
        for(size_t j = 0; j < _columns; j++) { _data[GetIndex(0, j)] = val[j]; }
    }

    /**
     * overwritten default constructor
     */
    Matrix() { }

    /**
     * Constructor using initializer_list's
     * @param lst
     */
    Matrix(const std::initializer_list<std::initializer_list<T>>& lst) {
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

    /**
     * Constructor using multi-dimensional initializer_list's
     * @param lst
     */
    Matrix(const std::initializer_list<std::initializer_list<std::initializer_list<T>>>& lst) {
        int i = 0, j = 0, c = 0;
        auto rows  = lst.size();
        auto cols  = lst.begin()->size();
        auto elems = lst.begin()->begin()->size();
        Resize(rows, cols, elems);
        for(const auto& l : lst) {
            for(const auto& v : l) {
                for(const auto k : v) {
                    _data[GetIndex(i, j, c)] = k;
                    ++c;
                }
                c = 0;
                ++j;
            }
            j = 0;
            ++i;
        }
    }

    /**
     * Deep copy constructor
     * @param other
     */
    Matrix(Matrix const& other) {
        Resize(other._rows, other._columns, other._element_size);
        std::memcpy(this->_data, other._data, other._dataSize * sizeof(T));
        this->needsFree = true;
    };

    /**
     * Default destructor, doesn't do anything
     */
    ~Matrix() { }

    /**
     * Generates a random matrix
     * @param rows number of rows in target matrix
     * @param columns number of columns in target matrix
     * @param element_size
     * @param minValue
     * @param maxValue
     * @return matrix of dimension `rows`, `columns` initialized with random values from `minValue` to `maxValue`
     */
    static Matrix
    Random(size_t rows, size_t columns, size_t element_size = 1, double minValue = 0.0, double maxValue = 1.0) {
        Matrix<T> matrix(0, rows, columns, element_size);
        for(size_t i = 0; i < rows; ++i) {
            for(size_t j = 0; j < columns; ++j) {
                for(size_t elem = 0; elem < element_size; ++elem) {
                    matrix(i, j, elem) = Random::Get(minValue, maxValue);
                }
            }
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
     * elements getter
     * @return
     */
    [[nodiscard]] inline size_t elements() const { return _element_size; }

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
        Matrix<T> res(0, _columns, _rows, _element_size);
        for(size_t i = 0; i < _columns; ++i) {
            for(size_t j = 0; j < _rows; ++j) {
                for(size_t elem = 0; elem < _element_size; ++elem) {
                    res._data[res.GetIndex(i, j, elem)] = _data[GetIndex(j, i, elem)];
                }
            }
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
        auto result = new Matrix<T>(0, this->rows(), this->columns() + other.columns(), other.elements());
        for(size_t i = 0; i < rows(); ++i) {
            for(size_t j = 0; j < columns() + other.columns(); ++j) {
                for(size_t elem = 0; elem < _element_size; ++elem) {
                    (*result)(i, j, elem) = j < columns() ? _data[GetIndex(i, j, elem)] : other(i, j - columns(), elem);
                }
            }
        }
        return *result;
    }

    /** OPERATORS **/

    // Comparison

    /**
     * comparison operator
     * @param rhs
     * @return
     */
    bool operator==(const Matrix<T>& rhs) const {
        // Just need to check element-wise
        // Dimensions handled by implementation.
        this->assertSize(rhs);
        return elementWiseCompare(rhs);
    }
    /**
     * not-equal operator
     * @param rhs
     * @return
     */
    bool operator!=(const Matrix<T>& rhs) const {
        return !(rhs == *this); // NOLINT
    }

    /**
     * Helper to check for equal dimensions
     * @param other
     */
    void assertSize(const Matrix<T>& other) const {
        assert(_columns == other.columns() && _rows == other.rows() && _element_size == other.elements());
    }

    /**
     * Element-wise comparison
     * @param rhs
     * @return
     */
    [[nodiscard]] bool elementWiseCompare(const Matrix<T>& rhs) const {
        assertSize(rhs);
        for(size_t i = 0; i < rows(); i++) {
            for(size_t j = 0; j < columns(); j++) {
                for(size_t elem = 0; elem < _element_size; ++elem) {
                    if(_data[GetIndex(i, j, elem)] != rhs(i, j, elem)) { return false; }
                }
            }
        }
        return true;
    }


    // Assignment
    /**
     * Assignment-operator.
     *
     * careful! actually overrides different sized matrices, just like other languages (python, matlab)
     * @param other
     * @return
     */
    Matrix<T>& operator=(const Matrix<T>& other) {
        if(this != &other) {
            if((this == NULL) || (_rows != other.rows() || _columns != other.columns())) {
                Resize(other.rows(), other.columns(), other.elements());
            }
            for(size_t i = 0; i < _rows; i++) {
                for(size_t j = 0; j < _columns; j++) {
                    for(size_t elem = 0; elem < _element_size; ++elem) {
                        _data[GetIndex(i, j, elem)] = other(i, j, elem);
                    }
                }
            }
        }
        return *this;
    }

    /**
     * Apply given function to Matrix
     * @param fun element-wise function to apply
     * @return fun(this)
     */
    Matrix<T> Apply(const std::function<T(T)>& fun) const {
        auto out = (*this);
        for(size_t i = 0; i < out.rows(); i++) {
            for(size_t j = 0; j < out.columns(); j++) {
                for(size_t elem = 0; elem < out.elements(); ++elem) {
                    out(i, j, elem) = fun(_data[GetIndex(i, j, elem)]);
                }
            }
        }
        return out;
    }

    // Math

    /**
     * Hadamard Multiplication
     * Z[i][j] = A[i][j] * B[i][j]
     * @param other
     * @return
     */
    Matrix& HadamardMulti(const Matrix& other) {
        for(size_t i = 0; i < rows(); i++) {
            for(size_t j = 0; j < columns(); j++) {
                for(size_t elem = 0; elem < _element_size; ++elem) { _data[GetIndex(i, j, elem)] *= other(i, j, elem); }
            }
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
        assert(_element_size == other.elements());
        auto result = new Matrix<T>(0, rows() * other.rows(), columns() * other.columns(), _element_size);
        for(size_t m = 0; m < rows(); m++) {
            for(size_t n = 0; n < columns(); n++) {
                for(size_t p = 0; p < other.rows(); p++) {
                    for(size_t q = 0; q < other.columns(); q++) {
                        for(size_t elem = 0; elem < _element_size; ++elem) {
                            (*result)(m * other.rows() + p, n * other.columns() + q, elem) =
                            _data[GetIndex(m, n, elem)] * other(p, q, elem);
                        }
                    }
                }
            }
        }
        return *result;
    }

    /**
     * Calculates sum of all elements
     * @return element sum
     */
    T sumElements() const {
        T result = T(0.0);
        for(size_t i = 0; i < rows(); i++) {
            for(size_t j = 0; j < columns(); j++) {
                for(size_t elem = 0; elem < _element_size; ++elem) { result += _data[GetIndex(i, j, elem)]; }
            }
        }
        return result;
    }

    /**
     * Matrix-Constant-Multiplication
     * @param rhs
     * @return
     */
    Matrix<T>& operator*=(const T& rhs) {
        (*this) = *this * rhs;
        return *this;
    }

    /**
     * Matrix-Addition
     * @param rhs
     * @return
     */
    Matrix<T>& operator+=(const Matrix<T>& rhs) {
        (*this) = (*this) + rhs;
        return *this;
    }
    /**
     * Matrix-Subtraction
     * @param rhs
     * @return
     */
    Matrix<T>& operator-=(const Matrix<T>& rhs) {
        (*this) = (*this) - rhs;
        return *this;
    }

    // Access

    /**
     * element access
     * @param row row index
     * @param column column index
     * @param elem element index
     * @return value at given address
     */
    T& operator()(const size_t& row, const size_t& column, const size_t& elem = 0) {
        return _data[GetIndex(row, column, elem)];
    }
    /**
     * const element-access
     * @param row row index
     * @param column column index
     * @param elem element index
     * @return const value at given address
     */
    T& operator()(const size_t& row, const size_t& column, const size_t& elem = 0) const {
        return _data[GetIndex(row, column, elem)];
    }

    /**
     * row getter
     *
     * **no in-place editing, creates new object!**
     * use SetRow instead
     * @param row index of row to get
     * @return row elements
     */
    Matrix<T> operator()(const size_t& row) { return GetSlice(row, row, 0, _columns - 1); }
    /**
     * const row-getter
     * @param row index of row
     * @return row values
     */
    Matrix<T> operator()(const size_t& row) const { return GetSlice(row, row, 0, _columns - 1); }

    /**
     * pointer operator
     * @return
     */
    T& operator*() { return _data[0]; }
    /**
     * const pointer operator
     * @return
     */
    T& operator*() const { return _data[0]; }

    /**
     * Column setter
     * @param index column index to set
     * @param other new values
     */
    void SetColumn(const size_t& index, const Matrix<T>& other) {
        bool isInColumns = other.columns() > other.rows();
        auto rowCount    = isInColumns ? other.columns() : other.rows();
        assert(rowCount == _rows);
        assert(_element_size == other.elements());
        for(size_t i = 0; i < rowCount; ++i) {
            for(size_t elem = 0; elem < elements(); ++elem) {
                _data[GetIndex(i, index, elem)] = other(isInColumns ? 0 : i, isInColumns ? i : 0, elem);
            }
        }
    }

    /**
     * Row setter
     * @param index row index to set
     * @param other holds new row elements
     */
    void SetRow(const size_t& index, const Matrix<T>& other) {
        bool isInColumns = other.columns() > other.rows();
        auto colCount    = isInColumns ? other.columns() : other.rows();
        assert(colCount == _columns);
        assert(_element_size == other.elements());
        for(size_t i = 0; i < colCount; ++i) {
            for(size_t elem = 0; elem < elements(); ++elem) {
                _data[GetIndex(index, i, elem)] = other(isInColumns ? 0 : i, isInColumns ? i : 0, elem);
            }
        }
    }

    /**
     * ostream operator, beatified representation
     * @param ostr
     * @param m
     * @return
     */
    friend std::ostream& operator<<(std::ostream& ostr, const Matrix& m) {
        ostr.precision(17);
        ostr << "[\n";
        for(size_t row = 0; row < m.rows(); row++) {
            ostr << '\t';
            for(size_t col = 0; col < m.columns(); col++) {
                if(m.elements() > 1) { ostr << "( "; }
                for(size_t elem = 0; elem < m.elements(); elem++) {
                    ostr << m._data[m.GetIndex(row, col, elem)];
                    if(elem < m.elements() - 1) ostr << ", ";
                }
                if(m.elements() > 1) { ostr << " )"; }
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
     * @param elementSize target number of elements per cell
     */
    void Resize(const size_t& rows, const size_t& cols, const size_t& elementSize = 1) {
        _rows         = rows;
        _columns      = cols;
        _element_size = elementSize;
        if(_data != nullptr) {
            _data = (T*)realloc(_data, rows * cols * elementSize * sizeof(T));
        } else {
            _data = (T*)malloc(rows * cols * elementSize * sizeof(T));
        }
        _dataSize = rows * cols * elementSize;
        needsFree = true;
    }

    /**
     *
     * @param row \f[\in [0, rows() - 1]\f]
     * @param col \f[\in [0, columns() - 1]\f]
     * @param elem \f[\in [0, elements() - 1]\f]
     * @return elem + col * elements() + row * columns() * elements()
     */
    [[nodiscard]] inline int GetIndex(const size_t& row, const size_t& col, const size_t& elem = 0) const {
        //        assert(row < _rows && col < _columns && elem < _element_size);
        return elem + col * _element_size + row * _columns * _element_size;
    }

    /**
     * Returns a slice of given dimension from the matrix
     *
     * @param rowStart row start index
     * @param rowEnd row end index
     * @param colStart column start index
     * @param colEnd column end index
     * @return sub-matrix
     */
    [[nodiscard]] inline Matrix GetSlice(size_t rowStart, size_t rowEnd, size_t colStart, size_t colEnd) const {
        size_t numRows = (rowEnd - rowStart) + 1;
        size_t numCols = (colEnd - colStart) + 1;

        Matrix out(0, numRows, numCols, _element_size);

        for(size_t i = 0; i < numRows; ++i) {
            for(size_t j = 0; j < numCols; ++j) {
                for(size_t elem = 0; elem < _element_size; ++elem) {
                    out(i, j, elem) = _data[GetIndex(rowStart + i, colStart + j, elem)];
                }
            }
        }
        return out;
    }

    void SetSlice(
    const size_t& row_start,
    const size_t& row_end,
    const size_t& col_start,
    const size_t& col_end,
    const Matrix<T>& slice) {
        size_t numRows = (row_end - row_start) + 1;
        size_t numCols = (col_end - col_start) + 1;
        assert(numRows == slice.rows());
        assert(numCols == slice.columns());

        for(size_t i = 0; i < numRows; ++i) {
            for(size_t j = 0; j < numCols; ++j) { _data[GetIndex(row_start + i, col_start + j)] = slice(i, j); }
        }
    }

    /**
     * returns 1D-Matrix from given index
     * @param index of elements
     * @return
     */
    Matrix<T> GetComponents(const size_t& index) const {
        assert(index < _element_size);
        Matrix<T> out(0, _rows, _columns, 1);
        for(size_t i = 0; i < _rows; ++i) {
            for(size_t j = 0; j < _columns; ++j) { out(i, j, 0) = _data[GetIndex(i, j, index)]; }
        }
        return out;
    }

private:
    /**
     * Helper to test if Matrix can have a determinant
     * @return
     */
    [[nodiscard]] bool HasDet() const { return _columns > 1 && _rows > 1 && _element_size == 1; }


    //! number rows
    size_t _rows = 0;
    //! number columns
    size_t _columns = 0;
    //! number elements
    size_t _element_size = 0;

    //! ongoing array representing data
    T* _data = nullptr;
    //! total number of elements
    size_t _dataSize = 0;
    //!
    bool needsFree = false;
};

/**
 * Extra operators
 */
/**
 * Matrix-Addition
 * @tparam T
 * @param lhs
 * @param rhs
 * @return
 */
template<typename T>
inline Matrix<T> operator+(Matrix<T> lhs, const Matrix<T>& rhs) {
    lhs.assertSize(rhs);
    auto result = new Matrix<T>(0, lhs.rows(), lhs.columns(), lhs.elements());
    for(size_t i = 0; i < lhs.rows(); i++) {
        for(size_t j = 0; j < lhs.columns(); j++) { (*result)(i, j) = lhs(i, j) + rhs(i, j); }
    }
    return *result;
}
/**
 * Matrix-Subtraction
 * @tparam T
 * @param lhs
 * @param rhs
 * @return
 */
template<typename T>
inline Matrix<T> operator-(Matrix<T> lhs, const Matrix<T>& rhs) {
    lhs.assertSize(rhs);
    auto result = new Matrix<T>(0, lhs.rows(), lhs.columns(), lhs.elements());
    for(size_t i = 0; i < lhs.rows(); i++) {
        for(size_t j = 0; j < lhs.columns(); j++) {
            for(size_t elem = 0; elem < lhs.elements(); ++elem) {
                (*result)(i, j, elem) = lhs(i, j, elem) - rhs(i, j, elem);
            }
        }
    }
    return *result;
}

/**
 * Simple Matrix scalar multiplication
 * @param lhs
 * @param rhs
 * @return scaled matrix
 */
template<typename T, typename U>
inline Matrix<T> operator*(Matrix<T> lhs, const U& rhs) {
    auto result = new Matrix<T>(0.0, lhs.rows(), lhs.columns(), lhs.elements());
    for(size_t i = 0; i < lhs.rows(); i++) {
        for(size_t j = 0; j < lhs.columns(); j++) {
            for(size_t elem = 0; elem < lhs.elements(); elem++) { (*result)(i, j, elem) = lhs(i, j, elem) * rhs; }
        }
    }
    return *result;
}
/**
 * Simple Matrix scalar multiplication
 * @param lambda
 * @param A
 * @return scaled matrix lambda * A = B with B(i, j) = lambda * A(i, j)
 */
template<typename T, typename U>
inline Matrix<T> operator*(U lambda, const Matrix<T>& A) {
    auto result = new Matrix<T>(0.0, A.rows(), A.columns(), A.elements());
    for(size_t i = 0; i < A.rows(); i++) {
        for(size_t j = 0; j < A.columns(); j++) {
            for(size_t elem = 0; elem < A.elements(); elem++) { (*result)(i, j, elem) = A(i, j, elem) * lambda; }
        }
    }
    return *result;
}

/**
 * Regular Matrix-Matrix multiplication
 * Calculates LHS * RHS
 * @param lhs
 * @param rhs
 * @return Rows x C result matrix
 */
template<typename T>
inline Matrix<T>& operator*(const Matrix<T>& lhs, const Matrix<T>& rhs) {
    assert(lhs.columns() == rhs.rows());
    assert(lhs.elements() == rhs.elements());
    auto result = new Matrix<T>(0.0, lhs.rows(), rhs.columns(), rhs.elements());
    for(size_t i = 0; i < lhs.rows(); i++) {
        for(size_t j = 0; j < rhs.columns(); j++) {
            for(size_t k = 0; k < rhs.rows(); k++) {
                for(size_t elem = 0; elem < rhs.elements(); elem++) {
                    (*result)(i, j, elem) += (T)(lhs(i, k, elem) * rhs(k, j, elem));
                }
            }
        }
    }
    return *result;
}

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
    auto result = new Matrix<T>(0, lhs.rows(), lhs.columns(), lhs.elements());
    for(size_t i = 0; i < result->rows(); i++) {
        for(size_t j = 0; j < result->columns(); j++) {
            for(size_t elem = 0; elem < result->elements(); elem++) {
                (*result)(i, j, elem) = lhs(i, j, elem) * rhs(i, j, elem);
            }
        }
    }
    return *result;
}

/**
 * Element wise division
 * @tparam T value type
 * @param lhs left hand side with dimension n1, m1
 * @param rhs right hand side with dimension n1, m1
 * @return product of element wise multiplication of lhs * rhs with dimension n1, m1
 */
template<typename T>
Matrix<T>& HadamardDiv(const Matrix<T>& lhs, const Matrix<T>& rhs) {
    lhs.assertSize(rhs);
    auto result = new Matrix<T>(0, lhs.rows(), lhs.columns(), lhs.elements());
    for(size_t i = 0; i < result->rows(); i++) {
        for(size_t j = 0; j < result->columns(); j++) {
            for(size_t elem = 0; elem < result->elements(); elem++) {
                (*result)(i, j, elem) = lhs(i, j, elem) / rhs(i, j, elem);
            }
        }
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
    assert(lhs.elements() == rhs.elements());
    auto result = new Matrix<T>(0, lhs.rows() * rhs.rows(), lhs.columns() * rhs.columns(), rhs.elements());
    for(size_t m = 0; m < lhs.rows(); m++) {
        for(size_t n = 0; n < lhs.columns(); n++) {
            for(size_t p = 0; p < rhs.rows(); p++) {
                for(size_t q = 0; q < rhs.columns(); q++) {
                    for(size_t elem = 0; elem < rhs.elements(); elem++) {
                        (*result)(m * rhs.rows() + p, n * rhs.columns() + q, elem) = lhs(m, n, elem) * rhs(p, q, elem);
                    }
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
    assert(lhs.elements() == rhs.elements());
    auto result = new Matrix<T>(0.0, lhs.rows(), lhs.columns() + rhs.columns(), lhs.elements());
    for(size_t i = 0; i < lhs.rows(); ++i) {
        for(size_t j = 0; j < lhs.columns() + rhs.columns(); ++j) {
            for(size_t elem = 0; elem < lhs.elements(); ++elem) {
                (*result)(i, j, elem) = j < lhs.columns() ? lhs(i, j, elem) : rhs(i, j - lhs.columns(), elem);
            }
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
        for(size_t j = 0; j < A.columns(); j++) {
            for(size_t elem = 0; elem < A.elements(); elem++) { (*count) += (A(i, j, elem) == B(i, j, elem)); }
        }
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
        for(size_t j = 0; j < size.columns; ++j) {
            for(size_t elem = 0; elem < size.elemDim; ++elem) {
                (*out)(i, j, elem) = value[elem + j * size.elemDim + i * size.columns * size.elemDim];
            }
        }
    }
    return *out;
}

/**
 * Search index of value with maximum value
 * **Caution!** This value does represent the index in a ongoing array.
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
            for(size_t elem = 0; elem < mat.elements(); ++elem) {
                if(mat(i, j, elem) > maxVal) {
                    maxVal   = mat(i, j, elem);
                    maxIndex = elem + j * mat.elements() + i * mat.columns() * mat.elements();
                }
            }
        }
    }
    return maxIndex;
}

/**
 * Search index of value with lowest value
 * **Caution!** This value does represent the index in a ongoing array.
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
            for(size_t elem = 0; elem < mat.elements(); ++elem) {
                if(mat(i, j, elem) < maxVal) {
                    maxVal   = mat(i, j, elem);
                    maxIndex = elem + j * mat.elements() + i * mat.columns() * mat.elements();
                }
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
    auto out = refVector ? valIfTrue : Matrix<T>(0, in.rows(), in.columns(), in.elements());

    for(size_t i = 0; i < in.rows(); i++) {
        for(size_t j = 0; j < in.columns(); j++) {
            for(size_t elem = 0; elem < in.elements(); elem++) {
                if(refVector) {
                    if(!condition(in(i, j, elem))) out(i, j, elem) = valIfFalse(i, j);
                    else
                        out(i, j, elem) = in(i, j, elem);
                } else {
                    out(i, j, elem) = condition(in(i, j, elem)) ? valIfTrue(0, 0) : valIfFalse(0, 0);
                }
            }
        }
    }
    return out;
}

/**
 * Converts two input matrices into a vector of
 * row-wise pairs of `a` and `b`
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

/**
 * Max value of given Matrix
 * @tparam T given datatype
 * @param mat matrix to search max value in
 * @return max value of given matrix
 */
template<typename T>
T max(const Matrix<T>& mat) {
    T maxVal = std::numeric_limits<T>::min();
    for(size_t i = 0; i < mat.rows(); i++) {
        for(size_t j = 0; j < mat.columns(); j++) {
            for(size_t k = 0; k < mat.elements(); ++k) {
                if(mat(i, j, k) > maxVal) { maxVal = mat(i, j, k); }
            }
        }
    }
    return maxVal;
}
/**
 * Min value of given matrix
 * @tparam T given datatype
 * @param mat matrix to search min value in
 * @return min value of matrix
 */
template<typename T>
T min(const Matrix<T>& mat) {
    T minVal = std::numeric_limits<T>::max();
    for(size_t i = 0; i < mat.rows(); i++) {
        for(size_t j = 0; j < mat.columns(); j++) {
            for(size_t k = 0; k < mat.elements(); ++k) {
                if(mat(i, j, k) < minVal) { minVal = mat(i, j, k); }
            }
        }
    }
    return minVal;
}

/**
 * Max value from given element index in matrix
 * @tparam T given datatype
 * @param mat matrix to search in
 * @param elemIndex index of element to compute max value of
 * @return max value over all elements with given index
 */
template<typename T>
T elemMax(const Matrix<T>& mat, const size_t& elemIndex) {
    assert(mat.elements() - 1 >= elemIndex);
    T maxVal     = std::numeric_limits<T>::min();
    size_t index = 0;
    for(size_t i = 0; i < mat.rows(); i++) {
        for(size_t j = 0; j < mat.columns(); j++) {
            if(mat(i, j, elemIndex) > maxVal) { maxVal = mat(i, j, elemIndex); }
            index++;
        }
    }
    return maxVal;
}
/**
 * Calculates element-mean
 * @tparam T given datatype
 * @param mat matrix to calculate mean of
 * @return mean of all elements of given matrix
 */
template<typename T>
T mean(const Matrix<T>& mat) {
    T sum(0);
    size_t index = 0;
    for(size_t i = 0; i < mat.rows(); i++) {
        for(size_t j = 0; j < mat.columns(); j++) {
            sum += mat(i, j);
            index++;
        }
    }
    return sum / index;
}

/**
 * mean operation on element with given index
 * @tparam T given datatype
 * @param mat input matrix
 * @param elemIndex index of element to compute mean of
 * @return mean of all elements
 */
template<typename T>
T elemMean(const Matrix<T>& mat, const size_t& elemIndex) {
    assert(mat.elements() - 1 >= elemIndex);
    T sum(0);
    size_t index = 0;
    for(size_t i = 0; i < mat.rows(); i++) {
        for(size_t j = 0; j < mat.columns(); j++) {
            sum += mat(i, j, elemIndex);
            index++;
        }
    }
    return sum / index;
}

/**
 * Returns Vector of diagonal elements from matrix.
 * @tparam T given datatype
 * @param in input matrix
 * @return vector of diagonal elements
 */
template<typename T>
Matrix<T> diag(const Matrix<T>& in) {
    Matrix<T> out(0, in.rows(), 1, in.elements());
    for(size_t i = 0; i < in.rows(); ++i) {
        for(size_t elem = 0; elem < in.elements(); ++elem) { out(i, 0, elem) = in(i, i, elem); }
    }
    return out;
}
/**
 * \example TestMatrix.cpp
 * This is an example on how to use the Matrix class.
 */