#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <limits>
#include <algorithm>
#include <set>

using namespace std;

struct Point {
    int32_t x;
    int32_t y;
};

inline bool operator<(const Point& lhs, const Point& rhs) {
  return lhs.x < rhs.x || (lhs.x == rhs.x && lhs.y < rhs.y);
}

inline bool operator==(const Point& lhs, const Point& rhs) {
  return lhs.x == rhs.x && lhs.y == rhs.y;
}

enum Direction {
    Left,
    Up,
    Right,
    Down
};

struct Wire {
    set<Point> points;
    vector<Point> steps;
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

void inserPointInWire(Wire &wire, Point point) {
    wire.points.insert(point);
    wire.steps.push_back(point);
}

void moveWire(Wire &wire, Direction direction, int32_t places) {
    Point origin = wire.position;
    Point destination = wire.position;
    if (direction == Left || direction == Right) {
        if (direction == Right) {
            destination.x = origin.x + places;
            for (int32_t i = origin.x; i < destination.x; i++) {
                inserPointInWire(wire, {i, destination.y});
            }
        } else {
            destination.x = origin.x - places;
            for (int32_t i = origin.x; i > destination.x; i--) {
                inserPointInWire(wire, {i, destination.y});
            }
        }
    } else {
        if (direction == Up) {
            destination.y = origin.y + places;
            for (int32_t i = origin.y; i < destination.y; i++) {
                inserPointInWire(wire, {destination.x, i});
            }
        } else {
            destination.y = origin.y - places;
            for (int32_t i = origin.y; i > destination.y; i--) {
                inserPointInWire(wire, {destination.x, i});
            }
        }
    }
    wire.position = destination;
}

uint32_t distance(Point crossing, Wire wireA, Wire wireB) {
    vector<Point>::iterator pointA = find(wireA.steps.begin(), wireA.steps.end(), crossing);
    vector<Point>::iterator pointB = find(wireB.steps.begin(), wireB.steps.end(), crossing);
    int32_t stepsToPointA = distance(wireA.steps.begin(), pointA);
    int32_t stepsToPointB = distance(wireB.steps.begin(), pointB);
    return stepsToPointA + stepsToPointB;
}

int main(int argc, char *argv[]) {
    Point origin = { 1, 1 };
    ifstream input = ifstream("input.txt");
    
    vector<Wire> wires = vector<Wire>();

    for (string line; getline(input, line);) {
        Wire wire = { set<Point>(), vector<Point>(), origin };
        stringstream lineStream = stringstream(line);
        string move;
        while(getline(lineStream, move, ',')) {
            Direction direction = directionWithChar(move[0]);
            string pos = move.substr(1, move.length());
            int32_t position = stoi(pos);
            moveWire(wire, direction, position);
        }
        wires.push_back(wire);      
    }

    set<Point> crossings = set<Point>();
    set_intersection(wires[0].points.begin(), wires[0].points.end(), wires[1].points.begin(), wires[1].points.end(), std::inserter(crossings, crossings.begin()));

    uint32_t minimumDistance = numeric_limits<uint32_t>::max();
    for (const auto& crossing : crossings) {
        if (crossing.x == origin.x && crossing.y == origin.y) {
            continue;
        }
        uint32_t dist = distance(crossing, wires.front(), wires.back());
        if (minimumDistance > dist) {
            minimumDistance = dist;
        }
    }

    cout << "Crossings: " << dec << crossings.size() << endl;
    cout << "Minimum distance: " << dec << minimumDistance << endl;

    return 0;
}