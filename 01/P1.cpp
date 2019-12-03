#include <fstream>
#include <iostream>
#include <cstdint>

using namespace std;

int main(int argc, char *argv[]) {
    ifstream input = ifstream("input.txt");
    uint64_t mass;
    uint64_t total = 0;
    while (input >> mass) {
        uint64_t fuel = mass / 3;
        fuel -= 2;
        total += fuel;
    }
    cout << "The total required fuel is: " << dec << total << endl;
    return 0;
}
