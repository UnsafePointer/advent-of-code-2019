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

    const uint32_t width = 25;
    const uint32_t height = 6;
    const uint32_t size = width * height;

    uint32_t counter = 0;
    array<uint32_t, size> image = {};
    image.fill(2);

    for (char n = 0; buffer >> n;) {
        uint8_t color = n - '0';
        uint32_t indexInLayer = counter % size;
        if (image[indexInLayer] == 2 && color < image[indexInLayer]) {
            image[indexInLayer] = color;
        }
        counter++;
    }

    for (array<uint32_t, size>::size_type i = 0; i < height; i++) {
        for (array<uint32_t, size>::size_type j = 0; j < width; j++) {
            cout << dec << image[j + width * i];
        }
        cout << endl;
    }
    // AURCY

    return 0;
}
