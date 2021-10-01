#pragma once
#include <functional>
#include "../../Matrix.h"
#include "../utils.h"


std::pair<double, std::vector<double>> quad_adaptive_rec(
  const std::function<double(double)>& fun,
  double a, double b, double tol, double hMin,
  std::vector<double>& nodes, Matrix<double>& values){

  double h = b-a;
  if(h < hMin){
    // error
    std::cerr << "H too small!" << std::endl;
    exit(-1);
  }

  Matrix<double> x = {{a, a+(b/2.0), b}};
  x = x.Transpose();
  if(values.rows() == 0){
    values = x.Apply(fun);
  } else if(values.rows() < 3){
    values = {{values(0, 0), fun(x(1, 0)), values(1, 0)}};
    values = values.Transpose();
  }

  auto qSimpson = (h/6.0) * (values(2, 0) + 4.0 * values(1, 0) + values(0, 0));
  auto qTrapez = (h / 2.0) * (values(2, 0) + values(0, 0));

  auto error = fabs(qSimpson - qTrapez);

  nodes.push_back(x(1, 0));

  if(error <= tol){
    return {qSimpson, nodes};
  }

  auto hTol = tol/2.0;
  auto leftValues = values.GetSlice(0, 1, 0, 0);
  auto resLeft = quad_adaptive_rec(fun, a, x(1, 0), hTol, hMin, nodes, leftValues);
  auto rightValues = values.GetSlice(1, 2, 0, 0);
  auto resRight = quad_adaptive_rec(fun, x(1, 0), b, hTol, hMin, nodes, rightValues);

  std::move(resLeft.second.begin(), resLeft.second.end(), std::back_inserter(nodes));
  std::move(resRight.second.begin(), resRight.second.end(), std::back_inserter(nodes));
  
  return {resLeft.first + resRight.first, nodes};

}
std::pair<double, std::vector<double>> quad_adaptive(const std::function<double(double)>& fun, double a, double b, double tol, double hMin){
  std::vector<double> nodes = {a, b};
  Matrix<double> q;
  return quad_adaptive_rec(fun, a, b, tol, hMin, nodes, q);
}


