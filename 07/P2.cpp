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

    Computer() : memory(vector<int64_t>()), pc(0), currentInstruction({}), input(queue<int64_t>()), output(queue<int64_t>()), state(Running) {}

    void loadProgram(vector<int64_t> program) {
        copy(program.begin(), program.end(), back_inserter(memory));
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

    int64_t read(uint64_t address) {
        int64_t value = memory[address];
        return value;
    }

    void write(uint64_t address, int64_t value) {
        memory[address] = value;
    }

    void executeInput() {
        if (input.empty()) {
            state = WaitingInput;
            return;
        } else if (state == WaitingInput) {
            state = Running;
        }
        int64_t value =  input.front();
        input.pop();
        int64_t positionA = memory[pc];
        memory[positionA] = value;

        pc++;
    }

    void executeOutput() {
        uint64_t positionA = memory[pc];
        int64_t value;
        if (currentInstruction.modeForParameterPosition(0)) {
            value = positionA;
        } else {
            value = memory[positionA];
        }
        output.push(value);
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

    array<uint8_t, 5> phaserSequence = { 5, 6, 7, 8, 9 };

    vector<uint8_t> maxOutputPhaseSequence = {};
    int64_t maxOutput = numeric_limits<int64_t>::min();
    do {
        array<Computer, 5> computers = { Computer() };
        for (array<Computer, 5>::size_type i = 0; i < computers.size(); i++) {
            Computer &computer = computers[i];
            computer.loadProgram(program);
            computer.input.push(phaserSequence[i]);
        }

        Computer &computer = computers[0];
        computer.input.push(0);

        array<Computer, 5>::size_type computerIndex = 0;
        while (computerIndex < computers.size()) {
            Computer &currentComputer = computers[computerIndex];
            if (currentComputer.state == Halted) {
                break;
            }
            while (true) {
                currentComputer.executeNextInstruction();
                if (currentComputer.state == WaitingInput || currentComputer.state == Halted) {
                    break;
                }
            }
            computerIndex++;
            if (computerIndex >= computers.size()) {
                computerIndex = 0;
            }
            Computer &nextComputer = computers[computerIndex];
            while (!currentComputer.output.empty()) {
                nextComputer.input.push(currentComputer.output.front());
                currentComputer.output.pop();
            }
        }
        if (computers[0].input.front() > maxOutput) {
            maxOutput = computers[0].input.front();
            maxOutputPhaseSequence.clear();
            copy(phaserSequence.begin(), phaserSequence.end(), back_inserter(maxOutputPhaseSequence));
        }
    } while (next_permutation(phaserSequence.begin(), phaserSequence.end()));

    cout << "Max output: " << dec << maxOutput << endl;
    for (const auto& phaserValue : maxOutputPhaseSequence) {
        cout << dec << unsigned(phaserValue) << endl;
    }
    return 0;
}
