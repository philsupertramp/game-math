#pragma once


class Layer
{
public:
    Layer(size_t in, size_t out)
    : in(in)
    , out(out)
    {
        weights   = MatrixDS<double>::Random(out, in);
        bias      = MatrixDS<double>::Random(out, 1);
        output = MatrixDS<double>(0, out, 1);
    }
    Layer()                     = default;
    Layer(const Layer& other) = default;

    [[nodiscard]] constexpr size_t In() const { return in; }
    [[nodiscard]] constexpr size_t Out() const { return out; }

    [[nodiscard]] MatrixDS<double> Evaluate(const MatrixDS<double>& input) const{
        auto a = (input * weights);
        for(size_t i = 0; i < a.columns(); i++){
            for(size_t j = 0; j < a.rows(); j++){
                a[j][i] += bias[i][0];
            }
        }
        return  a;
    }

    size_t in  = 0;
    size_t out = 0;

    MatrixDS<double> weights{};
    MatrixDS<double> bias{};
    MatrixDS<double> output{};
};

class LayerConfiguration
{
public:
    LayerConfiguration(std::initializer_list<Layer> layerConfig){
//        validateLayerConfig(layerConfig);
        layers = layerConfig;
    }

    static void validateLayerConfig(std::initializer_list<Layer> layerConfig){
        assert(layerConfig.size() > 0);

        auto inputSize = layerConfig.begin()->in;
        for(const auto& layer : layerConfig){
            assert(layer.in == inputSize);
        }
    }

    inline std::vector<Layer>::iterator begin() noexcept { return layers.begin(); }
    [[nodiscard]] inline std::vector<Layer>::const_iterator cbegin() const noexcept { return layers.cbegin(); }
    inline std::vector<Layer>::iterator end() noexcept { return layers.end(); }
    [[nodiscard]] inline std::vector<Layer>::const_iterator cend() const noexcept { return layers.cend(); }

    inline std::vector<Layer>::reverse_iterator rbegin() noexcept { return layers.rbegin(); }
    [[nodiscard]] inline std::vector<Layer>::const_reverse_iterator crbegin() const noexcept { return layers.crbegin(); }
    inline std::vector<Layer>::reverse_iterator rend() noexcept { return layers.rend(); }
    [[nodiscard]] inline std::vector<Layer>::const_reverse_iterator crend() const noexcept { return layers.crend(); }

    Layer operator[](size_t index){
        return layers[index];
    }
    Layer operator[](size_t index) const {
        return layers[index];
    }

    [[nodiscard]] size_t size() const { return layers.size(); }

private:
    std::vector<Layer> layers;
};

class LinearLayer
: public Layer
{
public:
    LinearLayer(size_t inputCount, size_t numNeurons)
    : Layer(inputCount, numNeurons)
    {

    }
};


double sigmoid(const double& in) { return 1.0 / (1.0 + exp(-in)); }
double sigmoidDx(const double& in) { return in * (1.0 - in); }

MatrixDS<double> Activate(const MatrixDS<double>& in) {
    auto out = new MatrixDS<double>(0, in.rows(), in.columns());
    for(size_t i = 0; i < out->rows(); i++) {
        for(size_t j = 0; j < out->columns(); j++) { (*out)[i][j] = sigmoid(in[i][j]); }
    }
    return *out;
}
MatrixDS<double> ActivateDerivative(const MatrixDS<double>& in) {
    auto out = new MatrixDS<double>(0, in.rows(), in.columns());
    for(size_t i = 0; i < out->rows(); i++) {
        for(size_t j = 0; j < out->columns(); j++) { (*out)[i][j] = sigmoidDx(in[i][j]); }
    }
    return *out;
}
