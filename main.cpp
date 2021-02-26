#include "Graph.hpp"
#include <string>
int main(int argc, char** argv){
    if(argc != 5){
        puts("Please launch this program with all the 4 parameters");
        cout << "\nExample: "<< argv[0] << " 8114    0.85    5    4" << endl;
        cout << "\tType the above command to launch and display the default test program" << endl;
        cout << "\t- here 8114 is the number of vertices (size of the graph)" << endl;
        cout << "\t- here alpha is (0.8)" << endl;
        cout << "\t- We display the (5) highest eigen values" << endl;
        cout << "\t- We skip (4) lines in the input file" << endl;
        cout << "Lines to be skipped is usefull when there is a leading description of the file. Set to 0 if there are no lines to be skipped" << endl;
        exit(EXIT_FAILURE);
    }
    
    int graphSize = atoi(argv[1]);
    double alpha = atof(argv[2]);
    int max_display = atoi(argv[3]);
    int nbLinesToskip = atoi(argv[4]);
    
    Graph* graph = new Graph(8114, alpha);
    graph->loadGraphFromFile("./p2p-Gnutella09.txt", nbLinesToskip);
    //Graph* graph = new Graph(graphSize, alpha);
    //graph->loadGraphFromFile("./test.txt", nbLinesToskip);
    Eigen::VectorXd eigenList = graph->computePageRank(200, 0.0005);
    int *highestEigensIndexes = graph->getHighestEigens(eigenList, max_display);
    graph->createGraphVizFile(highestEigensIndexes, max_display, eigenList);
    graph->displayGraph();
    
/*
     Graph* graph = new Graph(6, 0.85);
    //graph->loadGraphFromFile("./p2p-Gnutella09.txt", 4);
    graph->loadGraphFromFile("./test.txt", 0);
    Eigen::VectorXd eigenList = graph->computePageRank(100, 0.000005);
    int *highestEigensIndexes = graph->getHighestEigens(eigenList, 5);
    graph->createGraphVizFile(highestEigensIndexes, 5, eigenList);
    graph->displayGraph();
    
   Eigen::VectorXd test = Eigen::VectorXd(20);
   test[0] = 42;
   test[1] = 2;
   test[2] = 33;
   test[3] = 7;
   test[4] = 44;
   test[5] = 6;
   test[6] = 45;
   test[7] = 8;
   test[8] = 46;
   test[9] = 10;
   test[10] = 0;
   test[11] = 47;
   test[12] = 13;
   test[13] = 48;
   test[14] = 15;
   test[15] = 16;
   test[16] = 17;
   test[17] = 158;
   test[18] = 149;
   test[19] = 250;
   graph->createGraphVizFile(test, 5);*/
    return 0;
}