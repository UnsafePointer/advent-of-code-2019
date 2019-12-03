#include <fstream>
#include <iostream>
#include <cstdint>

using namespace std;

int main(int argc, char *argv[]) {
    ifstream input = ifstream("input.txt");
    int64_t mass;
    uint64_t total = 0;
    while (input >> mass) {
        int64_t fuel;
        do {
            fuel = mass / 3;
            fuel -= 2;
            if (fuel > 0) {
                total += fuel;
            }
            mass = fuel;
        } while (fuel > 0);
    }
    cout << "The total required fuel is: " << dec << total << endl;
    return 0;
}
