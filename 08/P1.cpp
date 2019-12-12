#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <array>

using namespace std;

int main(int argc, char *argv[]) {
    ifstream input = ifstream("input.txt");
    stringstream buffer;
    buffer << input.rdbuf();
    input.close();

    uint32_t width = 25;
    uint32_t height = 6;
    uint32_t size = width * height;

    uint32_t counter = 0;
    int32_t minimumNumberOfZeroes = numeric_limits<int32_t>::max();
    uint32_t layerWithMinimumNumberOfZeroes = 0;
    array<uint32_t, 10> digitsCount = { 0 };
    uint32_t result;
    uint32_t layer = 0;

    for (char n = 0; buffer >> n;) {
        uint8_t nn = n - '0';
        digitsCount[nn]++;
        counter++;
        if (counter >= size) {
            if (digitsCount[0] < minimumNumberOfZeroes) {
                layerWithMinimumNumberOfZeroes = layer;
                minimumNumberOfZeroes = digitsCount[0];
                result = digitsCount[1] * digitsCount[2];
            }
            digitsCount = { 0 };
            counter = 0;
            layer++;
        }
    }

    cout << "Minimum number of zeroes found in layer " << dec << layerWithMinimumNumberOfZeroes << endl;
    cout << "Result: " << dec << result << endl;

    return 0;
}
