#include <iostream>
#include <cstdint>

using namespace std;

int main(int argc, char *argv[]) {
    uint32_t lower = 137683;
    uint32_t upper = 596253;
    uint32_t numberOfPassword = 0;
    for (uint32_t i = lower; i < upper; i++) {
        uint32_t n = i;
        uint32_t previousR = 10;
        bool hasConsecutiveDigits = false;
        bool isIncreasingLeftToRight = true;
        while (n > 0) {
            uint32_t r = n % 10;
            n /= 10;
            if (previousR == r) {
                hasConsecutiveDigits = true;
            }
            if (previousR < r) {
                isIncreasingLeftToRight = false;
            }
            previousR = r;
        }
        if (hasConsecutiveDigits && isIncreasingLeftToRight) {
            numberOfPassword++;
        }
    }
    cout << "Number of passwords: " << dec << numberOfPassword << endl;
    return 0;
}
