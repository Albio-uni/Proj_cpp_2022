//
// Created by Aleardo on 03/08/22.
//
#include <iostream>

#include "images.hpp"
#include <string>

template<class T>
class Images : public NDArray<T> {
public:
    Images() = default;

    Images(std::vector<uint16_t> shape, std::vector<T> input) : NDArray<T>(shape, input) {
        if (shape.size() < 2)
            throw "Image item must have at least 2 dimensions, got 1";
    }

    T getPosition() {
        return NULL;
    }

    // ToDo: https://en.cppreference.com/w/cpp/filesystem/path
    /** @brief Read a file as is to the class with some checks on possible errors
     *
     * @param shape a vector with the desired dimension
     * @param path path to a file
     */
    void frombinary(std::vector<uint16_t> shape, std::string path) {
        this->shape = shape;

        std::ifstream file(path, std::ios::in | std::ios::binary);
        if (!file.is_open())
            throw "Can't open file. Some error occurred.";

        // Disables skipping of leading whitespace by the formatted input functions
        // https://en.cppreference.com/w/cpp/io/manip/skipws
        file.unsetf(std::ios::skipws);

        std::streampos fileSize;
        file.seekg(0, std::ios::end);
        fileSize = file.tellg();
        file.seekg(0, std::ios::beg);

        // reserve capacity in vector
        int dimension = (int) (fileSize / sizeof(T));
//        this->value.reserve(dimension); // Maybe I should use this code for file.read directly on this->value

        if (NDArray<T>::get_current_dimension() != dimension)
            throw "File is larger or shorter then expected.";

        std::vector<T> vec(fileSize / sizeof(T));
        file.read(reinterpret_cast<char *>(vec.data()), vec.size() * sizeof(T));

        this->value = vec;

        return;
    }

    /** @brief Construct this class with an example image. Default is "dot1_grey.png", usable images are:
     * "dot2_grey.png" and "dot3_grey.png".
     *
     */
    void fromexample(std::string image_name="dot1_grey.png") {
        frompng("../data/" + image_name);
        return;
    }

    //ToDo
    T fromtif() {
        return NULL;
    }

    //ToDo
    T frompng() {
        return NULL;
    }

    /** @brief ToDo: implement when fromtif and frompng and frombinary works so I can redirect input to correct function
     *
     */
    T frompath() {
        return NULL;
    }

    /** @brief The only use of this class is for auto instancing these three default values to a standard. Then call
     * subclass constructor to create the real values
     *
     * @param shape a vector with the desired dimension
     * @param seed seed value for the random function
     */
    void fromrandom(std::vector<uint16_t> shape = {10, 50, 50}, int seed = 42) {
        NDArray<T>::fromrandom(shape, seed);
        return;
    }

    /**
     * @brief Utility to prepare the matrix for processing by base functions
     * Squares the matrix collapsing all dimensions but last 2 and then last 2. E.g matrix (2, 3, 2, 2) becomes (6, 4)
     * It then gets transposed so that the resulting matrix is (4, 6).
     *
     * @return NDArray<T> Square matrix transposed. Collapses all dimensions and then last 2.
     */
    NDArray<T> prepareMat()
    {

        // Saving previous dimensions for restoring them later
        std::vector<uint16_t> old_shape = NDArray<T>::getShape();
        // Collapses all dimensions but last
        NDArray<T>::reshape({(uint16_t)count(), (uint16_t)(NDArray<T>::getShape()[NDArray<T>::getShape().size() - 1] * NDArray<T>::getShape()[NDArray<T>::getShape().size() - 2])});

        // Get the transpose
        NDArray<T> temp = NDArray<T>::transpose(*this);

        

        // Restore old dimensions
        NDArray<T>::reshape(old_shape);

        return temp;
    }

    /**
     * @brief Counts how many elements with the last 2 dimensions there are.
     * e.g (2, 3, 2) = 2
     *
     * @return int Number of elements
     */
    int count()
    {

        int output = 1;

        // Multiplies all dimensions except last 2
        for (uint16_t i = 0; i < NDArray<T>::getShape().size() - 2; i++)
        {
            output *= NDArray<T>::shape[i];
        }

        return output;
    }

    Images<T> filter(bool (*func)(NDArray<T>))
    {
        return (Images<T>)NDArray<T>::filter(prepareMat(), func);
    }
    /**
     * @brief Gets the max of all matrices in the last 2 dimensions.
     * Given an image (2, 3, 2) -> (1, 1, 2) -> (2)
     *
     * @return Images<T> Contains maximum 2D matrix
     */
    Images<T> max()
    {
        return NDArray<T>::max(prepareMat());
    }
    /**
     * @brief Gets the min of all matrices in the last 2 dimensions.
     * Given an image (2, 3, 2) -> (1, 1, 2) -> (2)
     *
     * @return Images<T> Contains minimum 2D matrix
     */
    Images<T> min()
    {
        return NDArray<T>::min(prepareMat());
    }
    /**
     * @brief Gets the sum of all matrices in the last 2 dimensions.
     * Given an image (2, 3, 2) -> (1, 1, 2) -> (2)
     *
     * @return Images<T> Contains sum of 2D matrices
     */
    Images<T> sum()
    {
        return NDArray<T>::sum(prepareMat());
    }
    /**
     * @brief Gets the mean of all matrices in the last 2 dimensions.
     * Given an image (2, 3, 2) -> (1, 1, 2) -> (2)
     *
     * @return Images<T> Contains mean of 2D matrices
     */
    Images<T> mean()
    {
        return NDArray<T>::mean(prepareMat());
    }
    /**
     * @brief Gets the standard deviation of all least-dimensioned elements
     * Given a series (2, 3, 2) -> (1, 1, 2) -> (2)
     *
     * @return Series<T> Serie of standard deviations
     */
    Images<T> std()
    {
        return NDArray<T>::std(prepareMat());
    }
    /**
     * @brief Gets the variance of all least-dimensioned elements
     * Given a series (2, 3, 2) -> (1, 1, 2) -> (2)
     *
     * @return Series<T> Serie of variances
     */
    Images<T> var()
    {
        return NDArray<T>::var(prepareMat());
    }
};