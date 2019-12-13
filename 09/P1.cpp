#include <iostream>
#include <fstream>
#include <cstdint>
#include <sstream>
#include <vector>
#include <array>
#include <queue>
#include <algorithm>
#include <limits>

using namespace std;

struct Instruction {
    uint64_t op;
    uint64_t intcode;
    vector<uint8_t> parameterMode;

    Instruction(int64_t opcode) : intcode(opcode) {
        op = 0;
        parameterMode = {};
        while (opcode != 0) {
            if (op == 0) {
                op = opcode % 100;
                opcode /= 100;
            } else {
                uint8_t mode = opcode % 10;
                parameterMode.push_back(mode);
                opcode /= 10;
            }
        }
    };

    uint8_t modeForParameterPosition(uint8_t position) {
        if (parameterMode.size() <= position) {
            return 0;
        }
        return parameterMode[position];
    }
};

enum ComputerState {
    Running,
    WaitingInput,
    Halted,
};

struct Computer {
    vector<int64_t> memory;
    vector<int64_t>::size_type pc;
    Instruction currentInstruction;
    queue<int64_t> input;
    queue<int64_t> output;
    ComputerState state;
    int64_t relativeBase;

    Computer() : memory(vector<int64_t>()), pc(0), currentInstruction({}), input(queue<int64_t>()), output(queue<int64_t>()), state(Running), relativeBase(0) {}

    void initializeMemory() {
        memory.reserve(numeric_limits<uint16_t>::max());
        fill(memory.begin(), memory.end(), 0);
    }

    void loadProgram(vector<int64_t> program) {
        for (vector<int64_t>::size_type i = 0; i < program.size(); i++) {
            memory[i] = program[i];
        }
    }

    void executeNextInstruction() {
        if (state == Running) {
            fetchInstruction();
        }
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
            case 9:
                executeRelativeBaseAdjust();
                break;
            case 99:
                executeHalt();
                break;
            default:
                exit(1);
        }
        return;
    }

    void fetchInstruction() {
        int64_t opcode = memory[pc];
        if (opcode <= 0) {
            exit(1);
        }
        pc++;
        currentInstruction = Instruction(opcode);
    }

    int64_t read(int64_t address, uint8_t position) {
        int64_t value;
        switch (currentInstruction.modeForParameterPosition(position)) {
            case 0:
                value = memory[address];
                break;
            case 1:
                value = address;
                break;
            case 2:
                value = memory[address + relativeBase];
                break;
            default:
                exit(1);
        }
        return value;
    }

    void write(uint64_t address, int64_t value, uint8_t position) {
        switch (currentInstruction.modeForParameterPosition(position)) {
            case 0:
                memory[address] = value;
                break;
            case 1:
                exit(1);
            case 2:
                memory[address + relativeBase] = value;
                break;
            default:
                exit(1);
        }
    }

    void executeInput() {
        if (input.empty()) {
            state = WaitingInput;
            return;
        } else if (state == WaitingInput) {
            state = Running;
        }
        int64_t value = input.front();
        input.pop();
        int64_t positionA = memory[pc];
        write(positionA, value, 0);

        pc++;
    }

    void executeOutput() {
        int64_t positionA = memory[pc];
        int64_t value = read(positionA, 0);

        output.push(value);
        cout << value << endl;
        pc++;
    }

    void executeArithmetic() {
        int64_t positionA = memory[pc];
        pc++;
        int64_t positionB = memory[pc];
        pc++;
        int64_t positionC = memory[pc];
        pc++;
        int64_t a = read(positionA, 0);
        int64_t b = read(positionB, 1);
        int64_t result;
        if (currentInstruction.op == 1) {
            result = a + b;
        } else {
            result = a * b;
        }
        write(positionC, result, 2);
    }

    void executeJump(bool conditionIsTrue) {
        int64_t positionA = memory[pc];
        pc++;
        int64_t positionB = memory[pc];
        pc++;
        int64_t eval = read(positionA, 0);
        int64_t destination = read(positionB, 1);
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
        int64_t a = read(positionA, 0);
        int64_t b = read(positionB, 1);
        int64_t result;
        if (compareIsEqual) {
            if (a == b) {
                result = 1;
            } else {
                result = 0;
            }
        } else {
            if (a < b) {
                result = 1;
            } else {
                result = 0;
            }
        }
        write(positionC, result, 2);
    }

    void executeRelativeBaseAdjust() {
        int64_t positionA = memory[pc];
        pc++;
        int64_t a = read(positionA, 0);
        relativeBase += a;
    }

    void executeHalt() {
        state = Halted;
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
    computer.initializeMemory();
    computer.loadProgram(program);
    computer.input.push(1);

    while (true) {
        computer.executeNextInstruction();
        if (computer.state == WaitingInput || computer.state == Halted) {
            break;
        }
    }

    return 0;
}
