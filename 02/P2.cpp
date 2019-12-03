#include <iostream>
#include <fstream>
#include <cstdint>
#include <sstream>
#include <vector>

using namespace std;

uint64_t fetchOperation(uint64_t &pc, vector<uint64_t> &program) {
    uint64_t op = program[pc];
    pc++;
    if ((op != 1) && (op != 2) && (op != 99)) {
        exit(1);
    }
    if ((op == 1 || op == 2) && (pc + 3) >= program.size()) {
        exit(1);
    }
    return op;
}

void execute(uint64_t &pc, vector<uint64_t> &program, bool addition) {
    uint64_t positionA = program[pc];
    pc++;
    uint64_t positionB = program[pc];
    pc++;
    uint64_t positionC = program[pc];
    pc++;
    if ((positionA >= program.size()) || positionB >= program.size() || positionC >= program.size()) {
        exit(1);
    }
    uint64_t a = program[positionA];
    uint64_t b = program[positionB];
    uint64_t result;
    if (addition) {
        result = a + b;
    } else {
        result = a * b;
    }
    program[positionC] =  result;
}

int main(int argc, char *argv[]) {
    ifstream input = ifstream("input.txt");
    stringstream buffer;
    buffer << input.rdbuf();
    input.close();

    vector<uint64_t> program;
    for (uint64_t i = 0; buffer >> i;) {
        program.push_back(i);    
        if (buffer.peek() == ',')
            buffer.ignore();
    }

    vector<uint64_t> memory;
    for (uint8_t i = 0; i <= 99; i++) {
        for (uint8_t j = 0; j <= 99; j++) {
            memory = vector<uint64_t>(program);
            memory[1] = i;
            memory[2] = j;
            vector<uint64_t>::size_type pc = 0;
            while (true) {
                uint64_t op = fetchOperation(pc, memory);
                switch (op) {
                case 1:
                    execute(pc, memory, true);
                    break;
                case 2:
                    execute(pc, memory, false);
                    break;
                case 99:
                    goto exit;
                    break;
                default:
                    exit(1);
                }    
            }
        exit:
            if (memory[0] == 19690720) {
                cout << "Values: " << dec << unsigned(i) << ", " << dec << unsigned(j) << " produce 19690720" << endl;
                exit(0);
            }
        }
    }
    return 0;
}
