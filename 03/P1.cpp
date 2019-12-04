#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <limits>

using namespace std;

struct Point {
    uint16_t x;
    uint16_t y;
};

enum Direction {
    Left,
    Up,
    Right,
    Down
};

struct Wire {
    vector<vector<bool>> path;
    Point position;
};

Direction directionWithChar(char dir) {
    switch (dir) {
        case 'L':
            return Left;
        case 'R':
            return Right;
        case 'U':
            return Up;
        case 'D':
            return Down;
        default:
            exit(1);
    }
}

void moveWire(Wire &wire, Direction direction, uint16_t places, Wire *firstWire, vector<Point> &crossings) {
    Point origin = wire.position;
    Point destination = wire.position;
    if (direction == Left || direction == Right) {
        if (direction == Right) {
            destination.x = origin.x + places;
            for (uint16_t i = origin.x; i <= destination.x; i++) {
                wire.path[i][destination.y] = true;
                if (firstWire != nullptr) {
                    if (firstWire->path[i][destination.y]) {
                        crossings.push_back({i, destination.y});
                    }
                }
            }
        } else {
            destination.x = origin.x - places;
            for (uint16_t i = destination.x; i <= origin.x; i++) {
                wire.path[i][destination.y] = true;
                if (firstWire != nullptr) {
                    if (firstWire->path[i][destination.y]) {
                        crossings.push_back({i, destination.y});
                    }
                }
            }
        }
    } else {
        if (direction == Up) {
            destination.y = origin.y + places;
            for (uint16_t i = origin.y; i <= destination.y; i++) {
                wire.path[destination.x][i] = true;
                if (firstWire != nullptr) {
                    if (firstWire->path[destination.x][i]) {
                        crossings.push_back({destination.x, i});
                    }
                }
            }
        } else {
            destination.y = origin.y - places;
            for (uint16_t i = destination.y; i <= origin.y; i++) {
                wire.path[destination.x][i] = true;
                if (firstWire != nullptr) {
                    if (firstWire->path[destination.x][i]) {
                        crossings.push_back({destination.x, i});
                    }
                }
            }
        }
    }
    wire.position = destination;
}

uint16_t distance(Point a, Point b) {
    return abs(b.x - a.x) + abs(b.y - a.y); 
}

int main(int argc, char *argv[]) {
    uint16_t size = numeric_limits<uint16_t>::max();
    uint16_t middle = numeric_limits<uint16_t>::max() / 2;
    Point origin = { middle, middle };
    ifstream input = ifstream("input.txt");
    
    vector<Wire> wires = vector<Wire>();
    vector<Point> crossings = vector<Point>();

    for (string line; getline(input, line);) {
        Wire wire = { vector<vector<bool>>(size, vector<bool>(size)), origin };
        stringstream lineStream = stringstream(line);
        string move;
        while(getline(lineStream, move, ',')) {
            Direction direction = directionWithChar(move[0]);
            string pos = move.substr(1, move.length());
            uint16_t position = stoi(pos);
            Wire *firstWire = nullptr;
            if (wires.size() > 0) {
                firstWire = &wires[0];
            }
            moveWire(wire, direction, position, firstWire, crossings);
        }
        wires.push_back(wire);      
    }

    uint16_t minimumDistance = size;
    for (const auto& crossing : crossings) {
        if (crossing.x == origin.x && crossing.y == origin.y) {
            continue;
        }
        uint16_t dist = distance(origin, crossing);
        if (minimumDistance > dist) {
            minimumDistance = dist;
        }
    }

    cout << "Crossings: " << dec << crossings.size() << endl;
    cout << "Minimum distance: " << dec << minimumDistance << endl;

    return 0;
}
