/**
 * File              : visualization.c
 * Author            : Philipp Zettl <philipp.zettl@godesteem.de>
 * Date              : 28.09.2019
 * Last Modified Date: 10.11.2019
 * Last Modified By  : Philipp Zettl <philipp.zettl@godesteem.de>
 */
/**
 * lib/visualization.c
 * Copyright (c) 2019 Philipp Zettl <philipp.zettl@godesteem.de>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <time.h>


typedef struct boundary {
    double min;
    double max;
} boundary;
/**
 * Example:
 *
 * FILE *gnuplot = popen("gnuplot --persist", "w");
 * plotAttributes attrs = {"exponential growth", "X", "Y", 0, 10, 0, "#008080", gnuplot: gnuplot, plotNames: plotNames};
 */
typedef struct plotAttributes {
    char title[50];
    char xAxis[50];
    char yAxis[50];
    int pointStrength;
    int lineStrength;
    u_int isStatPlot;
    char color[8];
    u_int isFirstPlot;
    FILE *gnuplot;
    const char **plotNames;
} plotAttributes;

boundary getBoundaries(const Matrix<double>& x){
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

void writeAttributes(plotAttributes attrs){
    FILE *gnuplot = attrs.gnuplot;
    if(attrs.title){
        char titleExtension[105];
        if(attrs.xAxis && attrs.yAxis && attrs.isStatPlot){
            strcpy(titleExtension, attrs.xAxis);
            strcat(strcat(titleExtension, " vs. "), attrs.yAxis);
        }
        fprintf(gnuplot, "set title '%s", attrs.title);
        if(attrs.isStatPlot){
            fprintf(gnuplot, "%s", titleExtension);
        }
        fprintf(gnuplot, "'\n");
    }
    if(attrs.xAxis){
        fprintf(gnuplot, "set xlabel '%s'\n", attrs.xAxis);
    }
    if(attrs.yAxis){
        fprintf(gnuplot, "set ylabel '%s'\n", attrs.yAxis);
    }
}

void scatterPlot(const Matrix<double> &x, const Matrix<double> &y, int len, plotAttributes attrs){
    /*
     * generates a scatter visualization
     */

    boundary bX, bY;
    bX = getBoundaries(x);
    bY = getBoundaries(y);

    writeAttributes(attrs);

    fprintf(attrs.gnuplot, "visualization [%f:%f] [%f:%f] '-' with points pt 7\n", bX.min, bX.max, bY.min, bY.max);
    for(int i=0; i<len; i++){
        fprintf(attrs.gnuplot, "%g %g\n", x(i,0), y(i,0));
    }
    fprintf(attrs.gnuplot, "e\n");

    pclose(attrs.gnuplot);
}

/**
 * each row of `mat` contains a vector
 * each 2 columns contain points
 * Generates a temporary file like:
 *
    # X   Y
      1   2
      2   3

      3   2
      4   1
 *
 * and forwarding it into attrs.gnuplot
 * as a plot of multiple datasets
 * @param mat
 * @param attrs
 *
 * http://www.gnuplotting.org/plotting-data/
 */
void multiPlot(const Matrix<double>& mat, plotAttributes attrs){
    writeAttributes(attrs);
    time_t now = time(NULL);
    FILE* file = fopen("multiPlot_data.txt", "w");
    fprintf(file, "# X\tY");
    auto numElements = mat.columns() / 2;

    for(int i=0; i<numElements; ++i){
        for(size_t j = 0; j < mat.rows(); ++j) {
            double elem = mat(j, i);
            fprintf(file, "%g", elem);
            fprintf(file, j == mat.rows() - 1 ? " " : ", ");
        }
        fprintf(file, "\n");
    }
    for(int i=0; i<numElements; i++){
        fprintf(attrs.gnuplot, i == 0 ? "plot " : "");
        fprintf(attrs.gnuplot, "'multiPlot_data.txt' using %d:%d with lines title '%s'", (i*2), (i*2) + 1, attrs.plotNames[i]);
        fprintf(attrs.gnuplot, i == (numElements-1) ? "\n" : ",");
        std::cout << (i == 0 ? "plot " : "");
        std::cout << "'multiPlot_data.txt' using "<< (i*2) <<":"<< (i*2) + 1<<" with lines title '"<< attrs.plotNames[i] << "'";
        std::cout << (i == (numElements-1) ? "\n" : ",");

    }
    fprintf(attrs.gnuplot, "\n");
    std::cout << "\n" << std::flush;
}


void addDataToPlot(const Matrix<double>& X, const Matrix<double>& Y, int size, FILE *gnuplot){
    for(int i=0;i<size; ++i){
        fprintf(gnuplot, "%g %g\n", X(i, 0), Y(i, 0));
    }
}
void plotFun(const std::function<double(const double&)> &fun, int startX, int endX, int startY, int endY, double stepSize, plotAttributes attrs){
    // Plots a given function fun
    // in the range x = [startX, endX] y = [startY, endY] with step size stepSize
    FILE *gnuplot = attrs.gnuplot;

    int size =  ((int)(endX - startX)/stepSize) + 2;
    printf("%d\n", size);
    Matrix<double> X(0, size, 1);
    Matrix<double> Y(0, size, 1);
    int iter = 0;
    for(double i=startX-stepSize; i<=endX && iter < size; i+=stepSize){
        X(iter, 0) = i;
        Y(iter, 0) = fun(i);
        iter += 1;
    }
#if DEBUG
    printVec(X);
  printVec(Y);
#endif
    writeAttributes(attrs);

    // configure
    fprintf(gnuplot, "set style line 1 linecolor rgb '%s' linetype 1 ", attrs.color ? attrs.color : "#4682B4");
    fprintf(gnuplot, "linewidth %d pointtype 7 ", attrs.lineStrength | 1);
    fprintf(gnuplot, "pointsize %d\n", attrs.pointStrength | 1);

    fprintf(gnuplot, "visualization [%d:%d] [%d:%d] '-' with linespoints linestyle 1\n", startX, endX, startY, endY);
    addDataToPlot(X, Y, size, gnuplot);
    fprintf(gnuplot, "e\n");
    //pclose(gnuplot);
}
