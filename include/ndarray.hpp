#pragma once

#include <vector>
#include <algorithm>
#include <functional>
#include <iostream>
#include <list>
#include <random>
#include <type_traits>

#include "series.hpp"
#include "images.hpp"

template <class T>
class ndarray{
        
    /// @var Actual values vector
    std::vector<T> value;
    /// @var Matrix size
    std::vector<uint16_t> shape;

    protected:
    // Constructors
    ndarray(std::vector<std::uint16_t> lengths, T *values);    
    ndarray(std::vector<std::uint16_t> lengths, std::vector<T> values);


    public:
    // Converters
    explicit operator Images<T>();
    explicit operator Series<T>();
    

    // Operator overload
    template <class OP, class K>
    [[nodiscard]] ndarray<T> element_wise(const ndarray<K> other, OP op) const;
    template <class K>
    [[nodiscard]] ndarray<T> operator+(const ndarray<K> other) const;
    template <class K>
    [[nodiscard]] ndarray<T> operator-(const ndarray<K> other) const;
    template <class K>
    [[nodiscard]] ndarray<T> operator*(const ndarray<K> other) const;
    template <class K>
    [[nodiscard]] ndarray<T> operator/(const ndarray<K> other) const;
    [[nodiscard]] const T& operator[](const uint16_t index) const;

    // Operator wrapper
    template <class K>
    [[nodiscard]] ndarray<T> plus(const ndarray<K> other) const;
    template <class K>
    [[nodiscard]] ndarray<T> minus(const ndarray<K> other) const;
    template <class K>
    [[nodiscard]] ndarray<T> dotdivide(const ndarray<K> other) const;
    template <class K>
    [[nodiscard]] ndarray<T> dottimes(const ndarray<K> other) const;

    // vector Wrapper
    [[nodiscard]] uint16_t size() const;
    [[nodiscard]] const std::vector<T>& getValue() const;
    [[nodiscard]] const std::vector<uint16_t>& getShape() const;
    [[nodiscard]] const T& first() const;
    [[nodiscard]] auto begin() const;
    [[nodiscard]] auto end() const;

    // Utils
    /**
     * @brief Get the value in position given a vector with a precise location
     *
     * @param pos a vector with a position
     * @return vector<T> value referenced by pos
     */
    ndarray<T> getPosition(std::vector<uint16_t> pos);
    /**
     * @brief Check if shape is correct or plausibile and return number of elements
     *
     * @return values_length Number of elements in vector
     */
    int get_current_dimension();
    /**
     * @brief Extract the underlaying array
     *
     * @return T* pointer to start of array (same as contained vector)
     */
    T *toarray();
    /**
     * @brief Count how many elements there are in every matrix along the first dimension
     *
     * @return multiplication of dimension sizes except first
     */
    int count();
    int count(ndarray<T> input);
    static ndarray<T> transpose(ndarray<T> input);
    void reshape(std::vector<uint16_t> new_shape);

    // Initializers
     /** @brief Initialize with random values this->value
     *
     * @param shape a vector with the desired dimension
     * @param seed seed value for the random function
     */
    static ndarray<T> fromrandom(std::vector<uint16_t> shape = {2, 2}, int seed = 42);
    /**
     * @brief Passed a list assign all values to current data structure
     * as one dimensional matrix.
     *
     */
    static ndarray<T> fromlist(std::list<T> l = {});
    // ToDo: https://en.cppreference.com/w/cpp/filesystem/path
    /** @brief Read a file as is to the class with some checks on possible errors
     *
     * @param shape a vector with the desired dimension
     * @param path path to a file
     */
    static ndarray<T> frombinary(std::vector<uint16_t> new_shape, std::string path);
    
    static ndarray<T> fromarray(std::vector<uint16_t> shape, T *input);
    static ndarray<T> fromarray(std::vector<uint16_t> shape, std::vector<T> input);
    static ndarray<T> fromarray(std::vector<T> input);

    /**
     * @brief Clip values in an array above and below provided values
     *
     * @param min_value min value to clip
     * @param max_value max value to clip
     */
    void clip(const T &min_value, const T &max_value);
    /**
     * @brief Applied the supplied function to every element in matrix
     *
     * @param func Unary function
     * @return ndarray<T> Structure with result
     */
    void map(T (*func)(T));



    /**
     * @brief Applies a function which should evaluate to boolean, along the first axis
     *
     * @param func A function that accepts data of type ndarray<T>
     * @return ndarray<T> Filtered data
     */
    ndarray<T> filter(bool (*func)(T));
    ndarray<T> filter(ndarray<T> input, bool (*func)(T));
    ndarray<T> applyFunc(ndarray<T> input, std::function<T(ndarray<T>)> func);
    /**
     * @brief Returns a vector of max values.
     * If matrix is monodimensional returns a single max value, if there is more than a
     * dimension, it cycles through the first returning the maximum along all other dimensions.
     *
     * @return ndarray<T> Structure containing the vector of maximum value(s)
     */
    ndarray<T> max();
    static ndarray<T> max(ndarray<T> input);
    /**
     * @brief Returns a vector of min values.
     * If matrix is monodimensional returns a single min value, if there is more than a
     * dimension, it cycles through the first returning the minimum along all other dimensions.
     *
     * @return ndarray<T> Structure containing the vector of minimum value(s)
     */
    ndarray<T> min();
    static ndarray<T> min(ndarray<T> input);
    /**
     * @brief Returns a vector of sum values.
     * If matrix is monodimensional returns a single sum value, if there is more than a
     * dimension, it cycles through the first returning the sum along all other dimensions.
     *
     * @return ndarray<T> Structure containing the vector of sum value(s)
     */
    ndarray<T> sum();
    static ndarray<T> sum(ndarray<T> input);
    /**
     * @brief Returns a vector of mean values.
     * If matrix is monodimensional returns a single mean value, if there is more than a
     * dimension, it cycles through the first returning the mean along all other dimensions.
     *
     * @return ndarray<T> Structure containing the vector of mean value(s)
     */
    ndarray<T> mean();
    static ndarray<T> mean(ndarray<T> input);
    /**
     * @brief Returns a vector of standard deviation values.
     * If matrix is monodimensional returns a single standard deviation value, if there is more than a
     * dimension, it cycles through the first returning the standard deviation along all other dimensions.
     *
     * @return ndarray<T> Structure containing the vector of standard deviation value(s)
     */

    ndarray<T> std();
    static ndarray<T> std(ndarray<T> input);
    /**
     * @brief Returns a vector of variance values.
     * If matrix is monodimensional returns a single variance value, if there is more than a
     * dimension, it cycles through the first returning the variance along all other dimensions.
     *
     * @return ndarray<T> Structure containing the vector of variance value(s)
     */
    ndarray<T> var();
    static ndarray<T> var(ndarray<T> input);

};

#include "ndarray/ndarray_constructors.i.hpp"
#include "ndarray/ndarray_converters.i.hpp"
#include "ndarray/ndarray_functions.i.hpp"
#include "ndarray/ndarray_initializers.i.hpp"
#include "ndarray/ndarray_operator_overload.i.hpp"
#include "ndarray/ndarray_operator_wrapper.i.hpp"
#include "ndarray/ndarray_utils.i.hpp"
#include "ndarray/ndarray_wrapper.i.hpp"