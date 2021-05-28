#pragma once

#include "../../Matrix.h"
#include "../utils.h"
#include <math/numerics/lin_alg/gaussSeidel.h>

class NaturalSpline
{
    Matrix<double> XI, YI;
    bool isEquidistant = true;
public:
    NaturalSpline(const Matrix<double>& X, const Matrix<double>& Y)
    {
	bool isRows = X.rows() > X.columns();
	size_t xRows = isRows : X.rows() ? X.columns();
	double h = X(isRows ? 1 : 0, isRows ? 0 : 1) - X(0, 0);
        for(size_t i = 0; i < xRows - 1; ++i){
	    if(X(isRows ? i+1 : 0, isRows ? 0 : i+1)-X(isRows ? i : 0, isRows ? 0 : i) != h){
                isEquidistant = false;
	    }
	}
	XI(X);
	YI(Y);
    }
    /**
     * Auswertung des kubischen Polynoms s_j
     * @return
     */
    double eval_spline_j(double x_act, size_t j, const Matrix<double>& mi){
        auto h = XI(j + 1, 0) - XI(j, 0);
        return ((mi(j, 0) * pow((XI(j + 1, 0) - x_act), 3.0) + mi(j + 1, 0) * pow((x_act - XI(j, 0)), 3.0)) / 6.0) / h
        + (YI(j, 0) * (XI(j + 1, 0) - x_act) + YI(j + 1, 0) * (x_act - XI(j, 0))) / h
        - ((mi(j, 0) * (XI(j + 1, 0) - x_act) + mi(j + 1, 0) * (x_act - XI(j, 0))) * h) / 6.0 ;
    }

    /**
     * Berechnung der zweiten Ableitungen an den Knoten
     * @param h 
     * @return 
     */
    Matrix<double> curv(double h) {
        auto mi = zeros(XI.rows(), XI.columns());
        auto dim = XI.rows() - 2;
        auto rhs = 6.0 / (h*h) * (tridiag(dim, dim, 1, -2, 1) * YI.GetSlice(1, YI.rows()-2, 0, YI.columns()-1));
        auto lhs = tridiag(dim, dim, 1, 4, 1);
        auto res = gaussSeidel(lhs, rhs);
        for(size_t i = 0; i < res.rows(); ++i){
            mi(i+1, 0) = res(i, 0);
        }
        return mi;
    }

    Matrix<double> operator()(const Matrix<double>& xi){
        auto mi = curv(XI(1, 0)-XI(0, 0));
        auto y = zeros(xi.rows(), xi.columns());
        // Werte die splines elementweise aus, für alle
        // elemente xi für i = 0, ..., n-1
        for(size_t j = 0; j < XI.rows() - 1; ++j){
            // alle elemente in x, die zwischen x_j und x_{j+1} liegen
            auto xl = XI(j, 0);
            auto xr = XI(j+1, 0);
            auto ind = nonzero([xl, xr](const double& x){return bool((xl <= x) && (x < xr));}, xi).Transpose();
            // werden mit dem j-ten spline ausgewertet
            for(size_t i = 0; i < ind.rows(); ++i){
                y(ind(i, 0), 0) += eval_spline_j(xi(ind(i, 0), 0), j, mi);
            }
        }
        return y;
    }
};
