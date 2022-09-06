
/**
 * @brief Creates an ndarray with random values
 * 
 * @tparam T Underlaying type
 * @param shape Shape of the new ndarray
 * @param seed Seed for RNG
 * @return ndarray<T> Ndarray with shape shape and random values
 */
template <class T>
ndarray<T> ndarray<T>::fromrandom(std::vector<uint16_t> shape, int seed)
{
    std::vector<T> output;
    int num_values = std::accumulate(shape.begin(), shape.end(), 1, std::multiplies<T>());

    // ToDo: Works only with float values not int
    std::random_device dev;
    std::default_random_engine rng(seed);
    std::uniform_real_distribution<T> dist6(1, 100);
    for (int k = 0; k < num_values; k++)
    {
        output.push_back(dist6(rng));
    }

    return ndarray<T>(shape, output);
}

/**
 * @brief Creates an ndarray from a given list
 *
 * @tparam T Underlaying data type
 * @param l Input list
 * @return ndarray<T> Resulting ndarray 1 by list length
 */
template <class T>
ndarray<T> ndarray<T>::fromlist(std::list<T> l)
{
    if (l.size() == 0)
        throw "List empty. Need to have a non empty list assigned.";

    std::vector<T> output;
    output.reserve(l.size());
    output.assign(l.begin(), l.end());

    return ndarray<T>({l.size()}, output);
}

/**
 * @brief Creates an ndarray from a given vector
 *
 * @tparam T Underlaying data type
 * @param input Input vector
 * @return ndarray<T> Resulting ndarray 1 by vector length
 */
template <class T>
ndarray<T> ndarray<T>::fromarray(std::vector<T> input){
    return ndarray<T>({static_cast<uint16_t>(input.size())}, input);
}
/**
 * @brief Creates an ndarray from a given vector with a given shape
 *
 * @tparam T Underlaying data type
 * @param shape Final ndarray shape
 * @param input Input vector
 * @return ndarray<T> Resulting ndarray
 */
template <class T>
ndarray<T> ndarray<T>::fromarray(std::vector<uint16_t> shape, std::vector<T> input)
{
    return ndarray<T>(shape, input);
}
/**
 * @brief Creates an ndarray from a C array with a given shape
 *
 * @tparam T Underlaying data type
 * @param shape Final ndarray shape
 * @param input Input array
 * @return ndarray<T> Resulting ndarray
 */
template <class T>
ndarray<T> ndarray<T>::fromarray(std::vector<uint16_t> shape, T* input)
{
    return ndarray<T>(shape, input);
}

/**
 * @brief Read file and writes it directly into ndarray
 *
 * @tparam T Underlaying data type
 * @param new_shape Final ndarray shape
 * @param path Input file path
 * @return ndarray<T> Resulting ndarray
 */
// ToDo: https://en.cppreference.com/w/cpp/filesystem/path
template <class T>
ndarray<T> ndarray<T>::frombinary(std::vector<uint16_t> new_shape, std::string path)
{
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
    //int dimension = (int)(fileSize / sizeof(T));
    // Maybe I should use this code for file.read directly on this->value
    // this->value.reserve(dimension);

    /*
    if (NDArray<T>::get_current_dimension() != dimension)
        throw "File is larger or shorter then expected.";
    */
    std::vector<T> vec(fileSize / sizeof(T));
    file.read(reinterpret_cast<char *>(vec.data()), vec.size() * sizeof(T));

    return ndarray<T>(new_shape, vec);
}
