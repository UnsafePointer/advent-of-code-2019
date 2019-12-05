#include <iostream>
#include <cstdint>
#include <set>
#include <vector>
#include <algorithm>

using namespace std;

int main(int argc, char *argv[]) {
    uint32_t lower = 137683;
    uint32_t upper = 596253;
    uint32_t numberOfPassword = 0;
    for (uint32_t i = lower; i < upper; i++) {
        uint32_t n = i;
        uint32_t previousR = 10;
        vector<bool> hasConsecutiveDigits = vector<bool>(10, false);
        bool isIncreasingLeftToRight = true;
        set<uint32_t> consecutiveDigits = {};
        while (n > 0) {
            uint32_t r = n % 10;
            n /= 10;
            set<uint32_t>::iterator found = consecutiveDigits.find(r);
            if (found != consecutiveDigits.end()) {
                hasConsecutiveDigits[r] = false;
            } else if (previousR == r) {
                hasConsecutiveDigits[r] = true;
                consecutiveDigits.insert(r);
            }
            if (previousR < r) {
                isIncreasingLeftToRight = false;
            }
            previousR = r;
        }
        vector<bool>::iterator found = find(hasConsecutiveDigits.begin(), hasConsecutiveDigits.end(), true);
        if (found != hasConsecutiveDigits.end() && isIncreasingLeftToRight) {
            numberOfPassword++;
        }
    }
    cout << "Number of passwords: " << dec << numberOfPassword << endl;
    return 0;
}
