#ifndef GRAPH_H
#define GRAPH_H
#include <iostream>
#include <string>
#include <Eigen/Dense>
#include <vector>
using namespace std;

class Graph{
    private:
        int nbVertices;
        int *countOutgoingDegrees;
        int *countIncomingDegrees;
        vector<int*> listOutgoingDegrees;
        vector<int*> listIncomingDegrees;
        double alpha;
        Eigen::MatrixXd *P;
        
    public:
        Graph(int nbVertices, double alpha);
        void loadGraphFromFile(const char *filePath, int nbLinesSkip);
        void addLink(int vertexId, int newNeighborId);
        Eigen::VectorXd computePageRank(int maxIteration, float tol);
        int *getHighestEigens(Eigen::VectorXd eigenList, int nbrMax);
        void createGraphVizFile(int *highestEigensIndexes, int tabSize, Eigen::VectorXd eigenList);
        void displayGraph();
};


#endif // GRAPH_H