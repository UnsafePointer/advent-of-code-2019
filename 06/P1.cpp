#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <memory>

using namespace std;

struct Node {
    string name;
    shared_ptr<Node> parent;

    Node(string name) : name(name), parent(nullptr) {}
};

shared_ptr<Node> findOrCreate(unordered_map<string, shared_ptr<Node>> &nodes, string nodeName) {
    shared_ptr<Node> node = nullptr;
    unordered_map<string, shared_ptr<Node>>::iterator found = nodes.find(nodeName);
    if (found == nodes.end()) {
        node = make_shared<Node>(nodeName);
        nodes[nodeName] = node;
    } else {
        node = found->second;
    }
    return node;
}

uint64_t calculareOrbits(shared_ptr<Node> node) {
    if (node->parent == nullptr) {
        return 0;
    }
    uint64_t directOrbits = 1;
    uint64_t indirectOrbits = calculareOrbits(node->parent);
    return indirectOrbits + directOrbits;
}

int main(int argc, char *argv[]) {
    ifstream input = ifstream("input.txt");
    unordered_map<string, shared_ptr<Node>> nodes = unordered_map<string, shared_ptr<Node>>();
    for (string line; getline(input, line);) {
        stringstream lineStream = stringstream(line);
        string nodeName;
        shared_ptr<Node> parent = nullptr;
        while(getline(lineStream, nodeName, ')')) {
            if (parent == nullptr) {
                parent = findOrCreate(nodes, nodeName);
            } else {
                shared_ptr<Node> child = findOrCreate(nodes, nodeName);
                child->parent = parent;
            }
        }
    }
    uint64_t orbits = 0;
    for (const auto& node : nodes) {
        orbits += calculareOrbits(node.second);
    }
    cout << "Total of orbits: " << dec << orbits << endl;
    return 0;
}
