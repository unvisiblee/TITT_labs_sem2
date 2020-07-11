#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <vector>
#include <sstream>
using namespace std;
string inputpath = "input.txt";
string outputpath = "output.txt";
ifstream fin;
ofstream fout;

int main() {
    fin.open(inputpath);
    if (!fin.is_open())
        cout << "Error while opening file (" + inputpath + ")!" << endl;
    int vertexCount;
    string buff;
    getline(fin, buff);
    vertexCount = stoi(buff);
    vector <vector <int>> graph(vertexCount); // graph representation (adjacency list)
    int elem;
    int num = 0;
    while (!fin.eof()) {   // read info from the file
        int adjCount;
        getline(fin, buff);
        if (buff == "")
            break;
        stringstream ss;
        ss << buff;
        ss >> adjCount;
        graph[num].resize(adjCount);
        int j = 0;
        while (ss >> elem) {
            graph[num][j] = elem;
            j++;
        }
        num++;
    }

    int maxpath = 0;
    int maxnode = 0;
    for (int i = 0; i < graph.size(); i++) {
        vector <bool> isVisited(vertexCount);
        for (int j = 0; j < vertexCount; j++)
            isVisited[j] = false;
        int pathlength = 0;
        queue <vector <int>> verticles;
        verticles.push(graph[i]);
        isVisited[i] = true;
        int visitedCount = 1;
        while (!verticles.empty()) {
            int queuesize = verticles.size();
            bool added = false;

            for (int k = 0; k < queuesize; k++) {
            vector <int> current = verticles.front();
            verticles.pop();
                if (!current.empty()) {
                    for (int j = 0; j < current.size(); j++) {
                        if (!isVisited[current[j] - 1]) {
                            added = true;
                            verticles.push(graph[current[j] - 1]);
                            isVisited[current[j] - 1] = true;
                            visitedCount++;
                        }
                    }
                }
            }
            if (added == true)
                pathlength++;
            if (visitedCount == vertexCount) {
                if (pathlength >= maxpath) {
                    if (i + 1 > maxnode)
                        maxpath = pathlength;
                    maxnode = i + 1;
                    break;
                }
                break;
            }
        }
    }
    if (maxpath == 0) {
        fout.open(outputpath);
        fout << "impossible" << endl;
    }
    else {
        fout.open(outputpath);
        fout << maxpath << endl;
        fout << maxnode << endl;
    }
}
