#pragma once

#include "../../Matrix.h"
#include "../utils.h"
#include <math/numerics/lin_alg/gaussSeidel.h>

class NaturalSpline
{
    Matrix<double> XI, YI, Tx, Ty;
    bool isEquidistant = true;
public:
    NaturalSpline(const Matrix<double>& X, const Matrix<double>& Y)
    {
        bool isRows = X.rows() > X.columns();
        XI = isRows ? X : X.Transpose();
        YI = isRows ? Y : Y.Transpose();
        double h = fabs(XI(1, 0) - XI(0, 0));
            for(size_t i = 0; i < XI.rows() - 1; ++i){
                auto dist = fabs(XI(i + 1, 0) - XI(i, 0));
                if((dist - h) > EPS){
                    isEquidistant = false;
                }
        }
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

    void SetAbstractionValue(const Matrix<double>& tx, const Matrix<double>& ty){
        Tx = tx;
        Ty = ty;
    }

    Matrix<double> operator()(const Matrix<double>& xi) {
        if(isEquidistant) return calculateEquidistant(xi);
        else {
            Matrix<double> ti;
            if(Tx.rows() > 0) { ti = Tx; }
            else {
                ti = linspace(min(xi), max(xi), XI.rows()).Transpose();
            }
            auto s_x = NaturalSpline(ti, XI)(xi);

            if(Ty.rows() > 0) { ti = Ty; }

            auto s_y = NaturalSpline(ti, YI)(xi);

            return HorizontalConcat(s_x, s_y);
        }
    }

    Matrix<double> calculateEquidistant(const Matrix<double>& xi){
        auto innerXI = xi.rows() > xi.columns() ? xi : xi.Transpose();
        auto mi = curv(XI(1, 0)-XI(0, 0));
        auto y = zeros(innerXI.rows(), innerXI.columns());
        // Werte die splines elementweise aus, für alle
        // elemente xi für i = 0, ..., n-1
        for(size_t j = 0; j < XI.rows() - 1; ++j){
            // alle elemente in x, die zwischen x_j und x_{j+1} liegen
            auto xl = XI(j, 0);
            auto xr = XI(j+1, 0);
            auto ind = nonzero([xl, xr](const double& x){return bool((xl <= x) && (x <= xr));}, innerXI).Transpose();
            // werden mit dem j-ten spline ausgewertet
            for(size_t i = 0; i < ind.rows(); ++i){
                y(ind(i, 0), 0) += eval_spline_j(innerXI(ind(i, 0), 0), j, mi);
            }
        }
        return y;
    }
};
