#pragma once

#include <utility>

#include "plot.h"
class Plot
{
public:
    explicit Plot(const char* title="")
    {
        fileName = "line_plot_data.txt";
        plotTypeName = "lines";
        attributes.pointStrength = 0;
        attributes.lineStrength = 10;
        attributes.isStatPlot = 0;
        attributes.color = "#008080";
        attributes.title = title;

        // create file and empty it
        storageFile = fopen(fileName, "w");
        fprintf(storageFile, "");
        fclose(storageFile);
    }

    void title(const char* name){
        attributes.title = name;
    }
    void xAxis(const char* name){
        attributes.xAxis = name;
    }
    void yAxis(const char* name){
        attributes.yAxis = name;
    }
    void pointSize(const int& size){
        attributes.pointStrength = size;
    }
    void lineWidth(const int& size){
        attributes.lineStrength = size;
    }

    void AddData(const Matrix<double> &x, const Matrix<double> &y, const char* name) {
        x.assertSize(y);
        AddData(HorizontalConcat(x, y), name);
    }
    /**
     * Appends data to the storage file
     * @param mat
     */
    void AddData(const Matrix<double>& mat, const char* name){
        // calculate boundaries
        auto new_bX = getBoundaries(mat.GetSlice(0, mat.rows()-1, 0, 0));
        auto new_bY = getBoundaries(mat.GetSlice(0, mat.rows()-1, 1, 1));

        bX.min = bX.min > new_bX.min ? new_bX.min : bX.min;
        bX.max = bX.max < new_bX.max ? new_bX.max : bX.max;
        
        bY.min = bY.min > new_bY.min ? new_bY.min : bY.min;
        bY.max = bY.max < new_bY.max ? new_bY.max : bY.max;


        // append storage file
        storageFile = fopen(fileName, "a");
        if(storageFile == nullptr){
            // error
            return;
        }
        fprintf(storageFile, "# ");
        fprintf(storageFile, fileName);
        fprintf(storageFile, "\n");

        for(size_t i=0; i<mat.columns(); i+=2){
            fprintf(storageFile, "# X Y\n");
            for(size_t j = 0; j < mat.rows(); ++j) {
                fprintf(storageFile, "%g ", mat(j, i));
                fprintf(storageFile, "%g\n", mat(j, i+1));
            }
            fprintf(storageFile, "\n\n");
        }
        fclose(storageFile);
        attributes.plotNames.push_back(name);
        numElements++;
    }

    /**
     * execution using call operator
     */
    void operator()() const {
        FILE *gnuplot = popen("gnuplot --persist", "w");
        writeAttributes(gnuplot);
        for(int i=0; i<numElements; ++i){
            fprintf(gnuplot, i == 0 ? "plot " : "");
            fprintf(gnuplot, "'");
            fprintf(gnuplot, "%s", fileName);
            fprintf(gnuplot, "' index %d with ", i);
            fprintf(gnuplot, "%s", plotTypeName);
            fprintf(gnuplot, " title '%s'", attributes.plotNames[i]);
            fprintf(gnuplot, i == (numElements-1) ? "\n" : ",");

        }
        fprintf(gnuplot, "\n");
        fclose(gnuplot);
    }

private:
    /**
     *
     * @param x
     * @return
     */
    [[nodiscard]] inline boundary getBoundaries(const Matrix<double>& x) const {
        /*
         * determines the boundaries of a array
         * returns: boundary(min, max)
         */
        double startX = min(x);
        //startX = startX - pow_(10, getExponent(startX) - 1);
        double endX = max(x);
        //endX = endX - pow_(10, getExponent(endX) - 1);
        boundary bond = {startX, endX};
        return bond;
    }


    /**
     *
     * @param gnuplot
     */
    void writeAttributes(FILE *gnuplot) const{
        if(attributes.title){
            char titleExtension[105];
            if(attributes.xAxis && attributes.yAxis && attributes.isStatPlot){
                strcpy(titleExtension, attributes.xAxis);
                strcat(strcat(titleExtension, " vs. "), attributes.yAxis);
            }
            fprintf(gnuplot, "set title '%s", attributes.title);
            if(attributes.isStatPlot){
                fprintf(gnuplot, "%s", titleExtension);
            }
            fprintf(gnuplot, "'\n");
        }
        if(attributes.xAxis){
            fprintf(gnuplot, "set xlabel '%s'\n", attributes.xAxis);
        }
        if(attributes.yAxis){
            fprintf(gnuplot, "set ylabel '%s'\n", attributes.yAxis);
        }
    }


protected:
    const char* fileName;
    const char* plotTypeName;
    FILE* storageFile = nullptr;
    plotAttributes attributes = {};
    int numElements = 0;
    boundary bX{}, bY{};

};

class ScatterPlot
: public Plot
{
public:
    explicit ScatterPlot(const char* name="")
    : Plot(name)
    {
        fileName = "point_plot_data.txt";
        plotTypeName = "points";
    }
};

class FunctionPlot
: public Plot
{
    std::function<double(const double&)> Function;
public:
    explicit FunctionPlot(std::function<double(const double&)>  fun, const char* name="")
    : Plot(name)
    , Function(std::move(fun))
    { }

    void AddData(const double& start, const double& end, const double &stepSize, const char* name){
        int size =  (int)((end - start)/stepSize) + 2;
        Matrix<double> X(0, size, 1);
        double i = start - stepSize;
        for(int iter = 0; i<=end && iter < size; ++iter){
            X(iter, 0) = i;
            i += stepSize;
        }
        auto Y = X.Apply(Function);
        Plot::AddData(X, Y, name);
    }
};