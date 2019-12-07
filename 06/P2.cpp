#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <memory>
#include <vector>
#include <algorithm>
#include <tuple>

using namespace std;

struct Node {
    string name;
    shared_ptr<Node> parent;
    bool visited;

    Node(string name) : name(name), parent(nullptr), visited(false) {}
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

pair<uint64_t, shared_ptr<Node>> getParents(shared_ptr<Node> node, vector<shared_ptr<Node>> &parents) {
    if (node->visited) {
        return { parents.size(), node };
    }
    node->visited = true;
    if (node->parent == nullptr) {
        return { parents.size(), nullptr };
    }
    parents.push_back(node->parent);
    return getParents(node->parent, parents);
}

int main(int argc, char *argv[]) {
    shared_ptr<Node> you = nullptr;
    shared_ptr<Node> san = nullptr;
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
                if (nodeName.compare("YOU") == 0) {
                    you = child;
                }
                if (nodeName.compare("SAN") == 0) {
                    san = child;
                }
            }
        } 
    }
    vector<shared_ptr<Node>> youParents = vector<shared_ptr<Node>>();
    getParents(you, youParents);
    vector<shared_ptr<Node>> sanParents = vector<shared_ptr<Node>>();
    shared_ptr<Node> commonParent = nullptr;
    uint64_t parentsCount = 0;
    tie(parentsCount, commonParent) = getParents(san, sanParents);
    parentsCount -= 1; // Because we just need the orbit of the parent
    
    vector<shared_ptr<Node>>::iterator it = find(youParents.begin(), youParents.end(), commonParent);
    uint64_t commonParentIndex = distance(youParents.begin(), it);

    uint64_t orbitalDistance = parentsCount + commonParentIndex;
    cout << "Orbital distance: " << dec << orbitalDistance << endl;
    return 0;
}
