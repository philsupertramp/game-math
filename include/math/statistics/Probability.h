#pragma once

#include "../Matrix.h"

struct LinearModel {
  double beta_0;
  double beta_1;
  Matrix<double> y_estimate;
  Matrix<double> residuals;	
};

enum P_NORM {
	Inf, One, Eukl
};

/** regular helpers **/
double round(double, int);
double abs(double);
double pow(double, int);
double min(const Matrix<double>&);
double max(const Matrix<double>&);
double mean(const Matrix<double>&);
double norm(const Matrix<double>&, P_NORM);
double cov(const Matrix<double>&, const Matrix<double>&);
double var(const Matrix<double>);

/** probability computation **/
LinearModel lm(const Matrix<double>&, const Matrix<double>&);
double coefficientOfDetermination(const Matrix<double>&, const Matrix<double>&);
double getExponent(double);
double likelihood(const Matrix<double>&);

