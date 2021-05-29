#pragma once

#include <utility>
#include "../Matrix.h"

enum DataTypes {
    LINE = 1,
    DOTS = 2,
    NONE = -1,
};

const char* GetDataTypeName(DataTypes type){
    switch(type) {
        case LINE: return "lines";// linetype 1 linewidth 2";
        case DOTS: return "points";// pointtype 5 pointsize 1.5";
        case NONE: return "none";
    }
}

/**
 * representation of min-max boundaries for a axis
 */
struct PlotBoundary {
    //! min boundary value
    double min;
    //! max boundary value
    double max;
};

struct PlotIndex {
    int start = -1;
    int stop = -1;
};

/**
 * struct representing attributes of a plot
 */
struct PlotAttributes {
    //! title label
    char* title;
    //! x-axis label
    char* xAxis;
    //! y-axis label
    char* yAxis;
    //! point strength
    int pointStrength;
    //! line strength
    int lineStrength;
    //! identifier for statistical plots for value comparison
    unsigned int isStatPlot;
    //! color used to plot elements
    const char* color;
    //! vector representing data series
    std::vector<const char*> plotNames;
    std::vector<const char*> plotTypes;
    std::vector<PlotIndex> plotIndices;
};

/**
 * Represents a data plot, using gnuplot
 */
class Plot
{
public:
    /**
     * default constructor
     * @param title
     */
    explicit Plot(const std::string& title = "") {
        dataFileName             = "line_plot_data.txt";
        plotTypeName             = "lines";
        attributes.pointStrength = 0;
        attributes.lineStrength  = 10;
        attributes.isStatPlot    = 0;
        attributes.color         = "#008080";
        attributes.title = new char[title.size() + 1];
        std::copy(title.begin(), title.end(), attributes.title);
        attributes.title[title.size()] = '\0';

        // create file and empty it
        storageFile = fopen(dataFileName, "w");
        fprintf(storageFile, " ");
        fclose(storageFile);
    }

    /**
     * Setter for title label
     * @param name
     */
    void title(const std::string& title) {
        attributes.title = new char[title.size() + 1];
        std::copy(title.begin(), title.end(), attributes.title);
        attributes.title[title.size()] = '\0';
    }

    /**
     * Setter for x-axis label
     * @param name
     */
    void xAxis(const std::string& name) {
        attributes.xAxis = new char[name.size() + 1];
        std::copy(name.begin(), name.end(), attributes.xAxis);
        attributes.xAxis[name.size()] = '\0';
    }

    /**
     * Setter for y-axis label
     * @param name
     */
    void yAxis(const std::string& name) {
        attributes.yAxis = new char[name.size() + 1];
        std::copy(name.begin(), name.end(), attributes.yAxis);
        attributes.yAxis[name.size()] = '\0';
    }

    /**
     * setter for point size of plot data
     * @param size
     */
    void pointSize(const int& size) { attributes.pointStrength = size; }

    /**
     * setter for line width of plot data
     * @param size
     */
    void lineWidth(const int& size) { attributes.lineStrength = size; }

    /**
     * Helper to add pair-wise elements to plot context
     * @param x
     * @param y
     * @param name
     */
    void AddData(const Matrix<double>& x, const Matrix<double>& y, const std::string& name, DataTypes dataType = DataTypes::NONE) {
        x.assertSize(y);
        AddData(HorizontalConcat(x, y), name, dataType);
    }

    /**
     * Appends data to the storage file
     * @param mat
     * @param name
     * @param dataTypeName
     */
    void AddData(const Matrix<double>& mat, const std::string& name, DataTypes dataTypeName = DataTypes::NONE) {
        // calculate boundaries
        auto new_bX = getBoundaries(mat.GetSlice(0, mat.rows() - 1, 0, 0));
        auto new_bY = getBoundaries(mat.GetSlice(0, mat.rows() - 1, 1, 1));

        bX.min = bX.min > new_bX.min ? new_bX.min : bX.min;
        bX.max = bX.max < new_bX.max ? new_bX.max : bX.max;

        bY.min = bY.min > new_bY.min ? new_bY.min : bY.min;
        bY.max = bY.max < new_bY.max ? new_bY.max : bY.max;


        // append storage file
        storageFile = fopen(dataFileName, "a");
        if(storageFile == nullptr) {
            // error
            return;
        }
        fprintf(storageFile, "# ");
        fprintf(storageFile, dataFileName);
        fprintf(storageFile, "\n");

        for(size_t i = 0; i < mat.columns(); i += 2) {
            fprintf(storageFile, "# X Y\n");
            for(size_t j = 0; j < mat.rows(); ++j) {
                fprintf(storageFile, "%g ", mat(j, i));
                fprintf(storageFile, "%g\n", mat(j, i + 1));
            }
            fprintf(storageFile, "\n\n");
        }
        fclose(storageFile);

        char* newName;
        newName = new char[name.size() + 1];
        std::copy(name.begin(), name.end(), newName);
        newName[name.size()] = '\0';
        PlotIndex index = {0, -1};
        if(!attributes.plotIndices.empty()){
            index = attributes.plotIndices[attributes.plotIndices.size()-1];
            index.start++;
        }

        attributes.plotIndices.push_back(index);
        attributes.plotNames.push_back(newName);
        attributes.plotTypes.push_back(dataTypeName == DataTypes::NONE ? plotTypeName : GetDataTypeName(dataTypeName));

        numElements++;
    }

    /**
     * execution using call operator
     */
    void operator()() const {
        FILE* gnuplot = popen("gnuplot --persist", "w");
        writeAttributes(gnuplot);
        for(int i = 0; i < numElements; ++i) {
            fprintf(gnuplot, i == 0 ? "plot " : "");
            fprintf(gnuplot, "'%s'", dataFileName);
            auto index = attributes.plotIndices[i];
            if(index.stop != -1) {
                fprintf(gnuplot, " index %d:%d with ", index.start, index.stop);
            }
            else {
                fprintf(gnuplot, " index %d with ", index.start);
            }
            fprintf(gnuplot, "%s", attributes.plotTypes[i]);
            fprintf(gnuplot, " title '%s'", attributes.plotNames[i]);
            fprintf(gnuplot, i == (numElements - 1) ? "\n" : ",");
        }
        fprintf(gnuplot, "\n");
        fclose(gnuplot);
    }

private:
    /**
     * calculates boundaries of given matrix (min and max values)
     * @param x
     * @return
     */
    [[nodiscard]] inline PlotBoundary getBoundaries(const Matrix<double>& x) const {
        /*
         * determines the boundaries of a array
         * returns: boundary(min, max)
         */
        double startX = min(x);
        //startX = startX - pow_(10, getExponent(startX) - 1);
        double endX = max(x);
        //endX = endX - pow_(10, getExponent(endX) - 1);
        PlotBoundary bond = { startX, endX };
        return bond;
    }


    /**
     * helper to write attributes to gnuplot context
     * @param gnuplot
     */
    void writeAttributes(FILE* gnuplot) const {
        if(attributes.title) {
            char titleExtension[105];
            if(attributes.xAxis && attributes.yAxis && attributes.isStatPlot) {
                strcpy(titleExtension, attributes.xAxis);
                strcat(strcat(titleExtension, " vs. "), attributes.yAxis);
            }
            fprintf(gnuplot, "set title '%s", attributes.title);
            if(attributes.isStatPlot) { fprintf(gnuplot, "%s", titleExtension); }
            fprintf(gnuplot, "'\n");
        }
        if(attributes.xAxis) { fprintf(gnuplot, "set xlabel '%s'\n", attributes.xAxis); }
        if(attributes.yAxis) { fprintf(gnuplot, "set ylabel '%s'\n", attributes.yAxis); }

    }


protected:
    //! data storage file name
    const char* dataFileName;
    //! representation of plot type
    const char* plotTypeName;
    //! data storage file
    FILE* storageFile         = nullptr;
    //! attributes for plot
    PlotAttributes attributes = {};
    //! number elements within the plot
    int numElements           = 0;
    //! x-axis boundaries for the resulting plot
    PlotBoundary bY{};
    //! y-axis boundaries for the resulting plot
    PlotBoundary bX{};
};

/**
 * Wrapper class to display a scatter plot.
 */
class ScatterPlot : public Plot
{
public:
    /**
     * default constructor
     * @param name
     */
    explicit ScatterPlot(const char* name = "")
        : Plot(name) {
        dataFileName = "point_plot_data.txt";
        plotTypeName = "points";
    }
};

/**
 * Wrapper class to easily generate a plot based on a function.
 */
class FunctionPlot : public Plot
{
    std::function<double(const double&)> Function;

public:
    /**
     * default constructor, requires function fun
     * @param fun
     * @param name
     */
    explicit FunctionPlot(std::function<double(const double&)> fun, const char* name = "")
        : Plot(name)
        , Function(std::move(fun)) { }

    /**
     * Adds data to the plot context
     * @param start starting value
     * @param end
     * @param stepSize
     * @param name
     */
    void AddData(const double& start, const double& end, const double& stepSize, const char* name) {
        int size = (int)((end - start) / stepSize) + 2;
        Matrix<double> X = linspace(start, end, size);
        auto Y = X.Apply(Function);
        Plot::AddData(X, Y, name);
    }
};