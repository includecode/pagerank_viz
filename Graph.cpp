#include "Graph.hpp"
#include <string.h>
#include <fstream>
#include <sstream>
#include <iomanip>
Graph::Graph(int nbVertices, double alpha){

    /* Initialise the transition matrix */
    this-> nbVertices = nbVertices;
    this->alpha = alpha;

    /* Each vertex has an outgoing degree value, usefull to set the probabilities */
    this->countOutgoingDegrees = (int*) calloc(nbVertices, sizeof(int));
    this-> P = new Eigen::MatrixXd (nbVertices, nbVertices);

    /* Each vertex has an incoming degree value, usefull to display the graphical result */
    this->countIncomingDegrees = (int*) calloc(nbVertices, sizeof(int));

    /* Create empty list of neigbors for each node*/
    // Allocate enough memory, we asume one node will be connected to (at most) nbVertices
    for(int i = 0; i < this->nbVertices; ++i){
        // memory for Outgoing vertices
        int *currentMemO =  (int*) malloc(nbVertices * sizeof(int*));
        this->listOutgoingDegrees.push_back(currentMemO);

        // memory for Incoming vertices
        int *currentMemI =  (int*) malloc(nbVertices * sizeof(int*));
        this->listIncomingDegrees.push_back(currentMemI);
    }
}

Eigen::VectorXd Graph::computePageRank(int maxIteration, float tol){
    cout << "Computing Pagerank algorithm..." <<endl;
    /* Create the initial vector x0, filled of random values.
    NOTE: VectorXd prduices a vector which is already transposed, ie a column */
    Eigen::VectorXd x0  = Eigen::VectorXd(this->nbVertices);
    
    /* Random numbers generated between 0 and 1 */
    srand( (unsigned)time( NULL ) );
    for(int i = 0; i<this->nbVertices; ++i)
        x0[i] = (double) rand()/RAND_MAX;

    /* Normalize the vector */
    x0.normalize();
    
    /* Compute pagerank, iteration 0 */
    Eigen::MatrixXd P_bis = (this->alpha * *(this->P));
    double jumping = (1 - this->alpha) / this->nbVertices;
    for(int i = 0; i < this->nbVertices; ++i)
        for(int j = 0; j < this->nbVertices; ++j)
            P_bis(i, j) += jumping;
    
    /* Compute pagerank until convergence */
    double lastErr;
    double currentErr;
    int counter = 0;
    
    do{
        counter++;
        if(counter == maxIteration){
            cout << "Failed to converge after [ " << maxIteration << " ] iterations" << endl;
            return Eigen::VectorXd::Zero(1); // This is an error code
        }
        // Save the previous sum before next iteration
        lastErr = x0.sum();

        x0 = P_bis * x0;
        currentErr = x0.sum();
    }while(lastErr - currentErr > tol);
    cout << "Finished successfully after [ " << counter << " ] iterations" << endl;
    return x0;
}

void Graph::loadGraphFromFile(const char *filePath, int nbLinesSkip){
    
    //We read the file with c method (it's Faster)
    FILE *file = fopen(filePath, "r");
    if(file == NULL){
        printf("Unable to load file %s", filePath);
        exit(1);
    }
    char* line = NULL;
    size_t len = 0;
    
    //skip unwanted lines
    for (int i = 0; i<nbLinesSkip; ++i){
        getline(&line, &len, file);
    }

    string lineStr;
    char* token;
    while((getline(&line, &len, file)) >= 0){
        lineStr = string(line);
        size_t pos = 0;
        string token;
        std::string delimiter = "\t";

        /* split according to delimiter */
        while((pos = lineStr.find(delimiter)) != string::npos){
            token = lineStr.substr(0, pos);
            cout << "[pagerank_viz]$ Loading vertex:\t" << token << endl;
            
            lineStr.erase(0, pos + delimiter.length());
            //lineStr[strlen(lineStr.c_str()) -1] = 0;
            this-> addLink(atoi(token.c_str()), atoi(lineStr.c_str()));
        }
    }
    fclose(file);
    if(line)
        free(line);
    puts("[pagerank_viz]$ Graph loaded sucessfully from file");

    for(int i = 0; i < this->nbVertices; ++i){
        printf("[%d] pointed by:\n", i);
        for (int j = 0; j < this->countIncomingDegrees[i]; ++j){
            printf("\t\t{%d}\n", this->listIncomingDegrees.at(i)[j]);
        }
    }
}

void Graph::addLink(int vertexId, int newNeighborId){
    
    /* Add new neighbor array of neighbors */
    this->listOutgoingDegrees.at(vertexId)[this->countOutgoingDegrees[vertexId]] = newNeighborId;
    this->listIncomingDegrees.at(newNeighborId)[this->countIncomingDegrees[newNeighborId]] = vertexId;


    /* Increment degrees for outgoing */
    this->countOutgoingDegrees[vertexId] ++;

    /* Increment degrees for incoming */
    this->countIncomingDegrees[newNeighborId] ++;

    /* Set the new probability for the destination vertex
    eg:
        a --> b
        a --> c
        a --> d 
        probability to reach d from a P(d, a) = 1 / 3
    */
    (*this->P)(newNeighborId, vertexId) = 1 / this->countOutgoingDegrees[vertexId];  
}

int returnMinIndex(int *arr, int arrSize, Eigen::VectorXd eigenList){
    int minIndex = 0;
    for(int i = 1; i < arrSize; ++i){
        if(eigenList[arr[i]] < eigenList[arr[minIndex]])
            minIndex = i;
    }
    return minIndex;
}


int *Graph::getHighestEigens(Eigen::VectorXd eigenList, int nbrMax){

    /* Prevent displaying more vertices than the eigen list */
    if(nbrMax > eigenList.size()){
        printf("The list of eigen values is only %d but you want %d to be displayed\n", eigenList.size(), nbrMax);
        exit(EXIT_FAILURE);
    }

    /* To the max indexes (indexes in initial eigen list) */
    int *firstIndexes = (int*)malloc(sizeof(int) * nbrMax);

   for(int i = 0; i < nbrMax; ++i){
       firstIndexes[i] = i;
   }
   
   int indexMin;
   for(int i = nbrMax; i < eigenList.size(); ++i){
        indexMin = returnMinIndex(firstIndexes, nbrMax, eigenList);
        if(eigenList[i] > eigenList(indexMin))
            firstIndexes[indexMin] = i;
   }
   
   cout << "..\n[pagerank_viz]$ Listing the first (" << nbrMax <<") highest eigen values" << endl;
   for(int i = 0; i < nbrMax; ++i){
       cout << "[" << firstIndexes[i] << "](" << eigenList(firstIndexes[i]) << ") | " << endl;
   }
   return firstIndexes;
}

string doubleToStringWithPrecision(double value){
    std::ostringstream streamObj;
    streamObj << value;
    std::string s = streamObj.str();
    return s;
}

void Graph::createGraphVizFile(int *highestEigensIndexes, int tabSize, Eigen::VectorXd eigenList){

    ofstream layoutFile("graph.gv");
    if(!layoutFile.is_open()){
        printf("Unable to create file 'graph.gv'");
        exit(1);
    }
    string markup = " digraph G {\n";
    
    for(int i = 0; i < tabSize; ++i){
        
        //printf("[%d] forwards to:\n", eigenList[highestEigensIndexes[i]]);
        //markup.append(to_string(highestEigensIndexes[i])) + "(" + to_string(eigenList(highestEigensIndexes[i])) +")\n";
        for (int j = 0; j < this->countIncomingDegrees[highestEigensIndexes[i]]; ++j){
            //printf("\t\t{%d}\n", this->listOutgoingDegrees.at(highestEigensIndexes[i])[j]);
            
            markup.append(to_string(this->listIncomingDegrees.at(highestEigensIndexes[i])[j]));
            markup.append(" -> ");
            markup.append(to_string(highestEigensIndexes[i]));
            markup.append(";\n");
            // to_string round to 6 decimals so we should use another double to string converter
            string label = to_string(highestEigensIndexes[i]) + "[label = \"" + to_string(highestEigensIndexes[i]) + " \n(" + doubleToStringWithPrecision(eigenList(highestEigensIndexes[i])) + ")\", color=\"green\", style=filled];";
            markup.append(label);
            
        }
   }
   markup.append("}");
   
   layoutFile << markup;
   layoutFile.close();
   
}
void Graph::displayGraph(){
    system("cd eigen-3.3.9/");
    if(this->nbVertices > 100)
        system("neato -Tpdf graph.gv -o graph.ps");
    else
        system("dot -Tps graph.gv -o graph.ps");
    system("xdg-open graph.ps");
    puts("The graph is created in graph.ps, just double click on it to view\n");
    system("cd ..");
}