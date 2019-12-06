#include <iostream>
#include <fstream>
#include <cstdint>
#include <sstream>
#include <vector>

using namespace std;

struct Instruction {
    uint64_t op;
    uint64_t intcode;
    vector<bool> parameterMode;
 
    Instruction(int64_t opcode) : intcode(opcode) {
        op = 0;
        parameterMode = {};
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
    };

    bool modeForParameterPosition(uint8_t position) {
        if (parameterMode.size() <= position) {
            return false;
        }
        return parameterMode[position];
    }
};

struct Computer {
    vector<int64_t> memory;
    vector<int64_t>::size_type pc;
    Instruction currentInstruction;

    Computer() : memory(vector<int64_t>()), pc(0), currentInstruction({}) {}

    void loadProgram(vector<int64_t> program) {
        copy(program.begin(), program.end(), back_inserter(memory)); 
    }

    void executeNextInstruction() {
        fetchInstruction();
        switch (currentInstruction.op) {
            case 1:
            case 2:
                executeArithmetic();
                break;
            case 3:
                executeInput();
                break;
            case 4:
                executeOutput();
                break;
            case 5:
                executeJump(true);
                break;
            case 6:
                executeJump(false);
                break;
            case 7:
                executeCompare(false);
                break;
            case 8:
                executeCompare(true);
                break;
            case 99:
                exit(0);
                break;
            default:
                exit(1);
        }
    }

    void fetchInstruction() {
        int64_t opcode = memory[pc];
        if (opcode <= 0) {
            exit(1);
        }
        pc++;
        currentInstruction = Instruction(opcode);
    }

    int64_t read(uint64_t address) {
        int64_t value = memory[address];
        return value;
    }

    void write(uint64_t address, int64_t value) {
        memory[address] = value;
    }

    void executeInput() {
        int64_t input;
        cin >> input;
        int64_t positionA = memory[pc];
        memory[positionA] = input;

        pc++;
    }

    void executeOutput() {
        uint64_t positionA = memory[pc];
        int64_t output;
        if (currentInstruction.modeForParameterPosition(0)) {
            output = positionA;
        } else {
            output = memory[positionA];
        }
        cout << output << endl;

        pc++;
    }

    void executeArithmetic() {
        int64_t positionA = memory[pc];
        pc++;
        int64_t positionB = memory[pc];
        pc++;
        int64_t positionC = memory[pc];
        pc++;
        int64_t a;
        if (currentInstruction.modeForParameterPosition(0)) {
            a = positionA;
        } else {
            a = memory[positionA];
        }
        int64_t b;
        if (currentInstruction.modeForParameterPosition(1)) {
            b = positionB;
        } else {
            b = memory[positionB];
        }
        int64_t result;
        if (currentInstruction.op == 1) {
            result = a + b;
        } else {
            result = a * b;
        }
        memory[positionC] = result;
    }

    void executeJump(bool conditionIsTrue) {
        int64_t positionA = memory[pc];
        pc++;
        int64_t positionB = memory[pc];
        pc++;
        int64_t eval;
        if (currentInstruction.modeForParameterPosition(0)) {
            eval = positionA;
        } else {
            eval = memory[positionA];
        }
        int64_t destination;
        if (currentInstruction.modeForParameterPosition(1)) {
            destination = positionB;
        } else {
            destination = memory[positionB];
        }
        if (conditionIsTrue) {
            if (eval != 0) {
                pc = destination;
            }
        } else {
            if (eval == 0) {
                pc = destination;
            }
        }
    }

    void executeCompare(bool compareIsEqual) {
        int64_t positionA = memory[pc];
        pc++;
        int64_t positionB = memory[pc];
        pc++;
        int64_t positionC = memory[pc];
        pc++;
        int64_t a;
        if (currentInstruction.modeForParameterPosition(0)) {
            a = positionA;
        } else {
            a = memory[positionA];
        }
        int64_t b;
        if (currentInstruction.modeForParameterPosition(1)) {
            b = positionB;
        } else {
            b = memory[positionB];
        }
        if (compareIsEqual) {
            if (a == b) {
                memory[positionC] = 1;
            } else {
                memory[positionC] = 0;
            }
        } else {
            if (a < b) {
                memory[positionC] = 1;
            } else {
                memory[positionC] = 0;
            }
        }
    }
};

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

    Computer computer = Computer();
    computer.loadProgram(program);

    while (true) {
        computer.executeNextInstruction();       
    }
    return 0;
}
