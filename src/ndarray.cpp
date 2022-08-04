/**
 * @file ndarray.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-08-02
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "ndarray.hpp"

using namespace std;

/**
 * @brief 
 * Class to handle multidimensional matrices. Serializes dimensions into a monodimensional array.
 * 
 * @tparam T 
 * Datatype for the matrix  
 * 
 */
template<class T>
class NDArray {

    /// @var Actual values vector
    vector<T> value;
    /// @var Matrix size
    vector<uint16_t> shape;

public:
    /**
     * @brief Construct a new NDArray object
     */
    NDArray() = default;

    /**
     * @brief Construct a new NDArray object when values are given in a C array
     * Basically a copy constructor since it copies the elements in the array in a vector
     *
     * @param lengths Dimensions vector of the resulting object
     * @param values Actual matrix values, C array format (T*)
     */
    NDArray(vector<std::uint16_t> lengths, T *values) {
        // Data checks
        if (lengths.size() == 0)
            throw "Requested 0 dimensioned array";

        this->shape = lengths;

        uint16_t values_length = 1;
        for (uint16_t i: lengths)
            values_length = values_length * i;

        for (unsigned int i = 0; i < values_length; i++) {
            value.push_back(values[i]);
        }

    }

    /**
     * @brief Construct a new NDArray object when values are already in a vector
     * Almost a move constructor, since it copies the address of both
     * vectors
     *
     * @param lengths Dimensions vector of the resulting object
     * @param values Actual matrix values, already in vector format
     */
    NDArray(vector<std::uint16_t> lengths, vector<T> values)
    {
        // Data checks
        if (lengths.size() == 0)
            throw "Requested 0 dimensioned array";

        this->shape = lengths;

        uint16_t values_length = 1;
        for (uint16_t i : lengths)
            values_length = values_length * i;

        if (values.size() != values_length)
            throw "Matrix dimensions and data length mismatch";

        this->value = values;
    }

    /**
     * @brief Get the value in position given a vector with a precise location
     *
     * @param pos a vector with a position
     * @return vector<T> value referenced by pos
     */
    NDArray<T> getPosition(vector<T> pos) {

        // Check indexing        
        if (pos.size() > shape.size())
            throw "Wrong dimensional indexing: dimensions mismatch";
        
        
        int lastdim = pos.size();

        // Calculate requested element's position up to the specified dimension
        int startIndex = 0;
        for (unsigned int i = 0; i < pos.size(); i++) {
            // Check if index out of bounds
            if (pos[i] < 0 || pos[i] >= shape[i])
                throw "Index out of bounds";

            // Calculating chunk size 
            int subDimensionSize = 1;
            for (unsigned int k = shape.size()-1; k != i ; k--) {
                subDimensionSize *= shape[k];
            }

            // Shift index by input * chunk size
            startIndex = startIndex + pos[i] * subDimensionSize;

        }

        vector<uint16_t> new_shape;
        // Starting from the missing dimension calculates the size of the identified matrix
        int endindex = 1;
        for (unsigned int i = lastdim; i < shape.size(); i++){
            endindex *= shape[i];
            new_shape.push_back(shape[i]);
        }
        endindex += startIndex;

        // Temporary output structure
        vector<T> output_temp;

        // Extract data in range
        do{
            output_temp.push_back(this->value[startIndex]);
            startIndex++;
        } while (startIndex < endindex);

        if (new_shape.size() == 0)
            new_shape.push_back(1);
        return NDArray<T>(new_shape, output_temp);
    }

    /**
     * @brief Function to directly address the underlaying vector
     * 
     * @param index index of the referenced element
     * @return T type of the element in the data structure
     */
    T operator[](unsigned int index){
        if (index < this->value.size())
            return this->value[index];
        else
            throw "Out of bound indexing";
    }

    /**
     * @brief Encapsulates underlaying vector size
     * 
     * @return int Number of elements in vector
     */
    int size(){
        return this->value.size();
    }

    /**
     * @brief Get the Data vector stored
     *
     * @return vector<T> of the contained data
     */
    vector<T> getValue()
    {
        return this->value;
    }

    /**
     * @brief Get the Shape vector
     *
     * @return vector<uint16_t> shape vector: every position is the dimension size
     */
    vector<uint16_t> getShape()
    {
        return this->shape;
    }


    /**
     * @brief Dot divide a matrix with another. ToDo: Controlli grandezze array e valori siano corretti/compatibibli
     *  tipo se un valore é di tipo int e l'altro tipo uint cast di che tipo (int imo)
     *
     * @return
     */
    template<class K>
    vector<double> dotdivide(const NDArray<K> other) {
        if(shape != other.shape)
            throw "Array shapes don't match";

        vector<double> out = {};

        for(unsigned int i = 0; i < value.size(); i++){
            out.push_back(value[i]/(double)other.value[i]);
        }

        return out;
    }

    /**
     * @brief Dot multiplication a matrix with another. ToDo: Controlli grandezze array e valori siano corretti/compatibibli
     *  tipo se un valore é di tipo int e l'altro tipo uint cast di che tipo (int imo)
     *
     * @return
     */
    vector<T> dottimes(const NDArray<T> other)
    {
        if (shape != other.shape)
            throw "Array shapes don't match";

        vector<T> out = {};

        for (unsigned int i = 0; i < value.size(); i++)
        {
            out.push_back(value[i] * other.value[i]);
        }

        return out;
    }

    /**
     * @brief Dot function a matrix with another.
     *
     * @return
     */
    template <class OP>
    vector<T> element_wise(const NDArray<T> other, OP op)
    {
        if (shape != other.shape)
            throw "Array shapes don't match";
        vector<T> out = {};

        for (unsigned int i = 0; i < value.size(); i++)
        {
            out.push_back(op(value[i], other.value[i]));
        }

        return out;
    }


    /**
     * @brief Clip values in an array above and below provided values
     *
     * @param min_value min value to clip
     * @param max_value max value to clip
     */
    void clip(const T& min_value, const T& max_value) {
                
        transform(std::begin(value), std::end(value), std::begin(value),
                       [&] (const T& v) { return clamp(v, min_value, max_value); });
    }

    NDArray<T> operator+(const NDArray<T> other)
    {
        if (shape != other.shape)
            throw "Array shapes don't match";

        vector<T> out = {};

        for (unsigned int i = 0; i < value.size(); i++)
        {
            out.push_back(value[i] + other.value[i]);
        }

        return NDArray(this->shape, out);
    }

    NDArray<T> operator-(const NDArray<T> other)
    {
        if (shape != other.shape)
            throw "Array shapes don't match";
        vector<T> out = {};

        for (unsigned int i = 0; i < value.size(); i++)
        {
            out.push_back(value[i] + other.value[i]);
        }

        return NDArray(this->shape, out);
    }

    T first()
    {
        return this->value[0];
    }

    NDArray<T> map(T (*func)(T))
    {
        return NULL;
    }

    T *toarray()
    {
        return &(this->value)[0];
    }

    /**
     * @brief Count how many least dimension elements are stored
     *
     * @return multiplication of dimension sizes except last
     */
    int count()
    {
        // Count how many least-dimensioned elements are present
        int size = 1;
        for (unsigned int i = 0; i < this->shape.size() - 1; i++)
            size *= this->shape[i];
        return size;
    }

    vector<T> filter(bool (*func)(NDArray<T>)){
        return {0};
    }

    T max()
    {
        return max(this->value);
    }
    T min()
    {
        return min(this->value);
    }
    T mean()
    {
        return this->sum() / value.size();
    }
    T sum(){
        return sum = accumulate(value.begin(), value.end(), 0.0);
    }
    T std(){
        double sq_sum = inner_product(value.begin(), value.end(), value.begin(), 0.0);
        return sqrt(sq_sum / value.size() - mean() * mean());
    }
    T var()
    {
        double sq_sum = inner_product(value.begin(), value.end(), value.begin(), 0.0);
        return sq_sum / value.size() - mean() * mean();
    }
    
};