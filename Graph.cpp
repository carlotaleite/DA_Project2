#include "Graph.h"

Graph::Graph(int index){
    this->index = index;
}

string Graph::getPath() const {
    switch (index) {
        case 0:
            return "../Data/Toy-Graphs/Toy-Graphs/shipping.csv";
        case 1:
            return "../Data/Toy-Graphs/Toy-Graphs/stadiums.csv";
        case 2:
            return "../Data/Toy-Graphs/Toy-Graphs/tourism.csv";
        case 3:
            return "../Data/Real-world Graphs/Real-world Graphs/graph1";
        case 4:
            return "../Data/Real-world Graphs/Real-world Graphs/graph2";
        case 5:
            return "../Data/Real-world Graphs/Real-world Graphs/graph3";
        default:
            cout << "Invalid graph index" << endl;
            return "";
    }
}

int Graph::getNumberNodes() const {
    switch (index) {
        case 0:
            return 14;
        case 1:
            return 11;
        case 2:
            return 5;
        case 3:
            return 1000;
        case 4:
            return 5000;
        case 5:
            return 10000;
        default:
            cout << "Invalid graph index" << endl;
            return 0;
    }
}

void Graph::loadNodesToyGraph() {
    int numberNodes = getNumberNodes();

    for(int i = 0; i < numberNodes; i++){
        Node* node = new Node(i);
        nodes.push_back(node);
    }
}

void Graph::loadNodesRWGraph() {
    string path = getPath() + "/nodes.csv";

    ifstream file(path);

    if(!file.is_open()) {
        cout << "Error opening file" << endl;
        return;
    }

    file.ignore(1000, '\n');
    string line;

    while(getline(file, line)){
        vector<string> row;
        stringstream ss(line);
        string token;
        while (getline(ss, token, ',')) {
            row.push_back(token);
        }

        int id = stoi(row[0]);
        double longitude = stod(row[1]);
        double latitude = stod(row[2]);

        Node* node = new Node(id, latitude, longitude);
        nodes.push_back(node);
    }
}

void Graph::loadEdges(){
    string path = getPath();
    if(index > 2){
        path += "/edges.csv";
    }
    ifstream file(path);

    if(!file.is_open()) {
        cout << "Error opening file" << endl;
        return;
    }

    file.ignore(1000, '\n');
    string line;

    while(getline(file, line)){
        vector<string> row;
        stringstream ss(line);
        string token;
        while (getline(ss, token, ',')) {
            row.push_back(token);
        }

        int id1 = stoi(row[0]);
        int id2 = stoi(row[1]);
        double distance = stod(row[2]);

        Node *node1 = nodes[id1];
        Node *node2 = nodes[id2];

        Edge* edge = new Edge(id1, id2, distance);
        edges.push_back(edge);

        node1->addEdge(edge);
        node2->addEdge(edge);
    }
}

void Graph::load() {
    cout << "Loading graph " << index << endl;

    if(index <= 2){
        loadNodesToyGraph();
    }
    else{
        loadNodesRWGraph();
    }
    loadEdges();
}


bool Graph::isLoaded() const {
    return !edges.empty();
}

void Graph::tspBTRec(const unsigned int **dists, unsigned int n, unsigned int curIndex, unsigned int curDist, unsigned int curPath[], unsigned int &minDist, unsigned int path[]) {
    if(curIndex == n) {
        // add the distance back to the initial node
        curDist += dists[curPath[n - 1]][curPath[0]];
        if(curDist < minDist) {
            minDist = curDist;
            // Copy the current state to the array storing the best state found so far
            for(unsigned int i = 0; i < n; i++) {
                path[i] = curPath[i];
            }
        }
        return;
    }
    // Try to move to the i-th vertex if the total distance does not exceed the best distance found and the vertex is not yet visited in curPath
    for(unsigned int i = 1; i < n; i++) { // i starts at 1 and not 0 since it is assumed that node 0 is the initial node so it will not be in the middle of the path
        if(curDist + dists[curPath[curIndex - 1]][i] < minDist) {
            bool isNewVertex = true;
            for(unsigned int j = 1; j < curIndex; j++) {
                if(curPath[j] == i) {
                    isNewVertex = false;
                    break;
                }
            }
            if(isNewVertex) {
                curPath[curIndex] = i;
                tspBTRec(dists,n,curIndex+1,curDist + dists[curPath[curIndex - 1]][curPath[curIndex]],curPath,minDist,path);
            }
        }
    }
}

unsigned int Graph::tspBT(const unsigned int **dists, unsigned int path[]) {
    unsigned int curPath[getNumberNodes()]; // static memory allocation is faster :)
    unsigned int minDist = std::numeric_limits<unsigned int>::max();

    // Assumes path starts at node 0 ...
    curPath[0] = 0;
    // ... so in the first recursive call curIndex starts at 1 rather than 0
    tspBTRec(dists, getNumberNodes(), 1, 0, curPath, minDist, path);
    return minDist;
}