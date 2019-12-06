#include <iostream>
#include <fstream>
#include <cstdint>
#include <sstream>
#include <vector>

using namespace std;

struct Instruction {
    uint64_t op;
    vector<bool> parameterMode;
 
    static Instruction withOpcode(int64_t opcode) {
        uint64_t op = 0;
        vector<bool> parameterMode = {};
        while (opcode != 0) {
            if (op == 0) {
                op = opcode % 100;
                opcode /= 100;
            } else {
                bool mode = opcode % 10;
                parameterMode.push_back(mode);
                opcode /= 10;
            }
        }
        return { op, parameterMode };
    };

    bool modeForParameterPosition(uint8_t position) {
        if (parameterMode.size() <= position) {
            return false;
        }
        return parameterMode[position];
    }
};

Instruction fetchInstruction(uint64_t &pc, vector<int64_t> &program) {
    int64_t opcode = program[pc];
    if (opcode <= 0) {
        exit(1);
    }
    pc++;
    return Instruction::withOpcode(opcode);
}

void executeArithmetic(Instruction instruction, uint64_t &pc, vector<int64_t> &program) {
    int64_t positionA = program[pc];
    pc++;
    int64_t positionB = program[pc];
    pc++;
    int64_t positionC = program[pc];
    pc++;
    int64_t a;
    if (instruction.modeForParameterPosition(0)) {
        a = positionA;
    } else {
        a = program[positionA];
    }
    int64_t b;
    if (instruction.modeForParameterPosition(1)) {
        b = positionB;
    } else {
        b = program[positionB];
    }
    int64_t result;
    if (instruction.op == 1) {
        result = a + b;
    } else {
        result = a * b;
    }
    program[positionC] = result;
}

void executeInput(Instruction instruction, uint64_t &pc, vector<int64_t> &program) {
    int64_t input;
    cin >> input;
    int64_t positionA = program[pc];
    pc++;
    program[positionA] = input;
}

void executeOutput(Instruction instruction, uint64_t &pc, vector<int64_t> &program) {
    uint64_t positionA = program[pc];
    pc++;
    int64_t output;
    if (instruction.modeForParameterPosition(0)) {
        output = positionA;
    } else {
        output = program[positionA];
    }
    cout << output << endl;   
}

int main(int argc, char *argv[]) {
    ifstream input = ifstream("input.txt");
    stringstream buffer;
    buffer << input.rdbuf();
    input.close();

    vector<int64_t> program;
    for (int64_t i = 0; buffer >> i;) {
        if (i < 0) {
            int a = 0;
        }
        program.push_back(i);    
        if (buffer.peek() == ',')
            buffer.ignore();
    }

    vector<uint64_t>::size_type pc = 0;
    while (true) {
        Instruction instruction = fetchInstruction(pc, program);
        switch (instruction.op) {
        case 1:
        case 2:
            executeArithmetic(instruction, pc, program);
            break;
        case 3:
            executeInput(instruction, pc, program);
            break;
        case 4:
            executeOutput(instruction, pc, program);
            break;
        case 99:
            goto exit;
            break;
        default:
            exit(1);
        }    
    }
exit:
    return 0;
}
