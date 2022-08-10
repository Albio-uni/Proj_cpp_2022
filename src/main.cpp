#include "ndarray.cpp"
#include "images.cpp"
#include "series.cpp"

#include <stdio.h>
#include <iostream>

using namespace std;
using namespace cimg_library;

template <typename T>
void printMat(NDArray<T> input)
{
    for (uint16_t i = 0; i < input.size(); i++)
    {
        /*
        if  (i % (input.count()) == 0)
            cout << "\n";
        */
        
        if (i % input.getShape()[0] == 0)
            cout << "\n";
        
        cout << input[i] << "\t";
    }
    
    cout << "\n";
}

int main() {

    CImg<float> image("./data/singlelayer_png/dot1_grey.png");

    // Images<float> k = Images<float>({1, 2}, {1, 1});
    // k.frompng("./data/singlelayer_png/dot1_grey.png");

    return 0;
}