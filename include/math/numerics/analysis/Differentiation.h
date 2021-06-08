#pragma once

Matrix<double> forwardDiff(const Matrix<double>& x, const Matrix<double>& y){
    auto df = zeros(x.rows(), x.columns());
    auto dy = y.GetSlice(1, y.rows()-1, 0, y.columns()-1) - y.GetSlice(0, y.rows()-2, 0, y.columns()-1);
    auto dx = x.GetSlice(1, x.rows()-1, 0, x.columns()-1) - x.GetSlice(0, x.rows()-2, 0, x.columns()-1);
    auto res = HadamardDiv(dy, dx);
    for(size_t i = 0; i < df.rows()-1; ++i){
        df.SetRow(i, res.GetSlice(i, i, 0, res.columns()-1));
    }
    return df;
}

Matrix<double> backwardDiff(const Matrix<double>& x, const Matrix<double>& y){
    auto df = zeros(x.rows(), x.columns());
    auto dy = y.GetSlice(1, y.rows()-1, 0, y.columns()-1) - y.GetSlice(0, y.rows()-2, 0, y.columns()-1);
    auto dx = x.GetSlice(1, x.rows()-1, 0, x.columns()-1) - x.GetSlice(0, x.rows()-2, 0, x.columns()-1);
    auto res = HadamardDiv(dy, dx);
    for(size_t i = 1; i < df.rows(); ++i){
        df.SetRow(i, res.GetSlice(i, i, 0, res.columns()-1));
    }
    return df;
}

Matrix<double> backwardDiff2(const Matrix<double>& x, const Matrix<double>& y){
    auto df = zeros(x.rows(), x.columns());
    auto dy = (3*y.GetSlice(2, y.rows()-1, 0, y.columns()-1))-(4*y.GetSlice(1, y.rows()-2, 0, y.columns()-1)) + y.GetSlice(0, y.rows()-3, 0, y.columns()-1);
    auto dx = x.GetSlice(2, x.rows()-1, 0, x.columns()-1) - x.GetSlice(0, x.rows()-3, 0, x.columns()-1);
    auto res = HadamardDiv(dy, dx);
    for(size_t i = 2; i < df.rows(); ++i){
        df.SetRow(i, res.GetSlice(i, i, 0, res.columns()-1));
    }
    return df;
}

Matrix<double> centralDiff(const Matrix<double>& x, const Matrix<double>& y){
    auto df = zeros(x.rows(), x.columns());
    auto dy = y.GetSlice(2, y.rows()-1, 0, y.columns()-1) - y.GetSlice(0, y.rows()-3, 0, y.columns()-1);
    auto dx = x.GetSlice(2, x.rows()-1, 0, x.columns()-1) - x.GetSlice(0, x.rows()-3, 0, x.columns()-1);
    auto res = HadamardDiv(dy, dx);
    for(size_t i = 1; i < df.rows()-1; ++i){
        df.SetRow(i, res.GetSlice(i, i, 0, res.columns()-1));
    }
    return df;
}

Matrix<double> centralDiff4(const Matrix<double>& x, const Matrix<double>& y){
    auto df = zeros(x.rows(), x.columns());
    auto dy = (-1*y.GetSlice(4, y.rows()-1, 0, y.columns()-1))+(8*y.GetSlice(3, y.rows()-2, 0, y.columns()-1)) - (8*y.GetSlice(1, y.rows()-4, 0, y.columns()-1)) - y.GetSlice(0, y.rows()-5, 0, y.columns()-1);
    auto dx = 3*(x.GetSlice(4, x.rows()-1, 0, x.columns()-1) - x.GetSlice(0, x.rows()-5, 0, x.columns()-1));
    auto res = HadamardDiv(dy, dx);
    for(size_t i = 2; i < df.rows()-2; ++i){
        df.SetRow(i, res.GetSlice(i, i, 0, res.columns()-1));
    }
    return df;
}
