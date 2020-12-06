#pragma once

struct EvaluationError {
    double Regression;
    double Classification;
};
struct EvaluationErrorSet {
    EvaluationError Training;
    EvaluationError Validation;
    EvaluationError Test;
};
struct EvaluationStatistics {
    double RegressionMean = 0.0;
    double RegressionStd  = 0.0;
    double RegressionMax  = std::numeric_limits<double>::min();

    double ClassificationMean = 0.0;
    double ClassificationStd  = 0.0;
    double ClassificationMax  = std::numeric_limits<double>::min();

    double RegressionSum            = 0.0;
    double RegressionSumSquared     = 0.0;
    double ClassificationSum        = 0.0;
    double ClassificationSumSquared = 0.0;

    size_t ElementCount = 0;

    void Calc() {
        RegressionMean     = RegressionSum / ElementCount;
        ClassificationMean = ClassificationSum / ElementCount;

        RegressionStd =
        (RegressionSumSquared - (RegressionSum * RegressionSum) / ElementCount) / (ElementCount - 1);
        ClassificationStd =
        (ClassificationSumSquared - (ClassificationSum * ClassificationSum) / ElementCount) / (ElementCount - 1);
    }

    friend std::ostream& operator<<(std::ostream& ostr, const EvaluationStatistics& err) {
        ostr << "Regression:\n\tMean: " << err.RegressionMean << " Std: " << err.RegressionStd
             << " Max: " << err.RegressionMax << "\n";
        ostr << "Classification:\n\tMean: " << err.ClassificationMean << " Std: " << err.ClassificationStd
             << " Max: " << err.ClassificationMax << "\n";
        return ostr;
    }
};
