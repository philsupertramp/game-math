#pragma once

#include "Matrix.h"
#include <algorithm>


template<typename T>
class Tensor
{
    std::vector<Matrix<T>> elements; // <!@ tensor container
public:
    size_t dim = 0;    // <!@ current tensor dimension
    size_t maxDim;     // <!@ fixed max dimension of tensor
    size_t elementDim; // <!@ enforced dimension for tensor elements
    Tensor(){};

    explicit Tensor(size_t dimension, size_t elementDimension = 1)
        : maxDim(dimension)
        , elementDim(elementDimension) {
        elements = std::vector<Matrix<T>>(dimension);
    }

    Tensor<T>& operator=(const Tensor<T>& other) {
        if(maxDim != other.maxDim){
            elements.resize(other.maxDim);
        }
        maxDim     = other.maxDim;
        dim        = 0;
        elementDim = other.elementDim;

        for(size_t i = 0; i < other.dim; ++i) { push_back(other[i]); }
        return *this;
    }

    Tensor<T>(const Tensor<T>& other)
        : Tensor<T>(other.maxDim, other.elementDim) {
        for(size_t i = 0; i < other.dim; ++i) { push_back(other[i]); }
    }

    /**
     * adds copy of dimensional "layer" to tensor
     * @param elem to add
     */
    void push_back(const Matrix<T>& elem) {
        if(dim >= maxDim) {
            std::cout << "Error pushback, max size reached." << std::endl;
            return;
        }
        assert(elem.elements() == elementDim);
        elements[dim++] = elem;
    }
    void Set(const std::initializer_list<Matrix<T>>& elementsIn) {
        assert(elementsIn.size() - 1 <= maxDim);
        dim      = elementsIn.size();
        elements = elementsIn;
    }

    // Access operators

    /**
     * tensor element access
     * @param index of element
     * @return tensor element Matrix
     */
    Matrix<T>& operator[](const size_t& index) { return elements[index]; }
    /**
     * const tensor element access
     * @param index of element
     * @return const tensor element Matrix
     */
    const Matrix<T>& operator[](const size_t& index) const { return elements[index]; }


    /**
     * Tensor-Multiplication
     * @param lhs tensor
     * @param rhs tensor
     * @return transformed lhs tensor
     */
    friend Tensor<T> operator*(Tensor<T> lhs, const Tensor<T>& rhs) {
        assert(lhs.dim == rhs.dim);
        assert(lhs.elementDim == rhs.elementDim);
        Tensor<T> out(lhs.dim, lhs.elementDim);
        for(size_t i = 0; i < lhs.dim; ++i) { out.push_back(lhs[i] * rhs[i]); }
        return out;
    }

    /**
     * Tensor-Scalar multiplication from right hand side
     * @param lhs tensor
     * @param rhs scalar
     * @return scaled tensor
     */
    friend Tensor<T> operator*(Tensor<T> lhs, const T& rhs) {
        Tensor<T> out(lhs.dim, lhs.elementDim);
        for(size_t i = 0; i < lhs.dim; ++i) { out.push_back(lhs[i] * rhs); }
        return out;
    }

    /**
     * Tensor-Scalar-Multiplication from left hand side
     * @param lhs scalar
     * @param rhs tensor
     * @return scaled tensor
     */
    friend Tensor<T> operator*(T lhs, const Tensor<T>& rhs) {
        Tensor<T> out(rhs.dim, rhs.elementDim);
        for(size_t i = 0; i < rhs.dim; ++i) { out.push_back(rhs[i] * lhs); }
        return out;
    }

    /**
     * builds element wise sum of tensor
     * @return
     */
    T sum() const {
        T out = 0;
        for(size_t i = 0; i < dim; ++i) { out += elements[i].sumElements(); }
        return out;
    }

    Tensor<T> GetComponents(const size_t& index) const {
        Tensor<T> out(dim, elementDim);
        for(size_t i = 0; i < dim; ++i){
            out[i] = elements[i].GetComponents(index);
        }
        return out;
    }

    typename std::vector<Matrix<T>>::iterator begin() { return elements.begin(); }
    typename std::vector<Matrix<T>>::iterator end() { return elements.end(); }
    typename std::vector<Matrix<T>>::const_iterator begin() const { return elements.begin(); }
    typename std::vector<Matrix<T>>::const_iterator end() const { return elements.end(); }
    typename std::vector<Matrix<T>>::const_iterator cbegin() const noexcept { return elements.cbegin(); }
    typename std::vector<Matrix<T>>::const_iterator cend() const noexcept { return elements.cend(); }

    /**
     * ostream output operator for nice formatted output
     * @param os
     * @param t
     * @return
     */
    friend std::ostream& operator<<(std::ostream& os, const Tensor<T>& t) {
        for(auto& elem : t.elements) { os << "{\n" << elem << "}\n" << std::flush; }
        return os;
    }
};

/**
 *
 * @tparam T
 * @param mat
 * @return
 */
template<typename T>
std::vector<size_t> argmax(const Tensor<T>& mat) {
    T minVal    = std::numeric_limits<T>::min();
    size_t minI = 0, minJ = 0, minIK = 0, minQ = 0;
    for(size_t q = 0; q < mat.dim; q++) {
        for(size_t i = 0; i < mat[q].rows(); i++) {
            for(size_t j = 0; j < mat[q].columns(); j++) {
                for(size_t i_k = 0; i_k < mat[q].elements(); ++i_k) {
                    if(mat[q](i, j, i_k) > minVal) {
                        minVal = mat[q](i, j, i_k);
                        minI   = i;
                        minJ   = j;
                        minIK  = i_k;
                        minQ   = q;
                    }
                }
            }
        }
    }

    return { minI, minJ, minIK, minQ };
}
/**
 * Search index of value with lowest value
 * **Caution!** This value does represent the index in a ongoing array.
 * Example:
 *      A = {{{{3,3}, {3,2}}}}
 *      argmin(A) == 3
 *      A(argmin(A)//A.colums(), argmin(A)%A.columns())(argmin(A)//A.columns(), argmin(A)%A.columns()) == 2
 *
 * @tparam T matrix value type
 * @param mat element to search in
 * @return index of minimal value
 */
template<typename T>
std::vector<size_t> argmin(const Tensor<T>& mat) {
    T minVal    = std::numeric_limits<T>::max();
    size_t minI = 0, minJ = 0, minIK = 0, minQ = 0;
    for(size_t q = 0; q < mat.dim; q++) {
        for(size_t i = 0; i < mat[q].rows(); i++) {
            for(size_t j = 0; j < mat[q].columns(); j++) {
                for(size_t i_k = 0; i_k < mat[q].elements(); ++i_k) {
                    if(mat[q](i, j, i_k) < minVal) {
                        minVal = mat[q](i, j, i_k);
                        minI   = i;
                        minJ   = j;
                        minIK  = i_k;
                        minQ   = q;
                    }
                }
            }
        }
    }

    return { minI, minJ, minIK, minQ };
}

template<typename T>
T min(const Tensor<T>& tensor) {
    T minVal = std::numeric_limits<T>::max();
    for(size_t i = 0; i < tensor.dim; ++i) {
        auto elem = tensor[i];
        for(size_t row = 0; row < elem.rows(); ++row) {
            for(size_t col = 0; col < elem.columns(); ++col) {
                for(size_t comp = 0; comp < elem.elements(); ++comp) {
                    auto component = elem(row, col, comp);
                    if(component < minVal) { minVal = component; }
                }
            }
        }
    }
    return minVal;
}
template<typename T>
T max(const Tensor<T>& tensor) {
    T maxVal = std::numeric_limits<T>::min();
    for(size_t i = 0; i < tensor.dim; ++i) {
        auto elem = tensor[i];
        for(size_t row = 0; row < elem.rows(); ++row) {
            for(size_t col = 0; col < elem.columns(); ++col) {
                for(size_t comp = 0; comp < elem.elements(); ++comp) {
                    auto component = elem(row, col, comp);
                    if(component > maxVal) { maxVal = component; }
                }
            }
        }
    }
    return maxVal;
}
