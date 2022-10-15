#include <iostream>
#include <fstream>
using namespace std;


// Node class
class Node{
    public:
        int val;
        Node(){
            val = 0;
        }
        Node(int value){
            val = value;
        }
};


// Edge class
class Edge{
    public:
        int startingEdge, endingEdge;
};


// Function to print row of nodes
void printRow(Node* Nodes, int noOfNodes){
    for(int i = 0; i < noOfNodes; i++){
        cout << Nodes[i].val << "  ";
    }
}


// Function to return position of starting node
int positionNode(int edge, Node* nodes, int noOfNodes){
    for(int i = 0; i < noOfNodes; i++){
        if(edge == nodes[i].val)
            return i;
    }
    return -1;
}


// Function to read no. of edges and no. of nodes from file
int** ReadFile(string filename, int& noOfNodes, int& noOfEdges){
    ifstream reading(filename);
    int readone;
    reading >> noOfNodes;
    while(!reading.eof()){
        reading >> readone;
        noOfEdges++;
    }
    noOfEdges /= 2;
    int** edgeArray = new int*[noOfEdges];
    for(int i = 0; i < noOfEdges; i++)
        edgeArray[i] = new int[2];
    reading.close();
    reading.open(filename);
    reading >> noOfNodes;
    int i = 0;
    while(!reading.eof()){
        reading >> edgeArray[i][0];
        reading >> edgeArray[i][1];
        i++;
    }
    reading.close();
    return edgeArray;
}


// Function to check edge
bool isEdge(int start, int end, Edge* edges, int noOfEdges){
    for(int i = 0; i < noOfEdges; i++)
        if(edges[i].startingEdge == start && edges[i].endingEdge == end)
            return true;    
    return false;
}


int main(){
    // Reading file
    int noOfNodes, noOfEdges = 0, readone;
    int** edgeArray = ReadFile("Input-Graph.txt", noOfNodes, noOfEdges);
    Node* topNodes = new Node[noOfNodes];
    Node* bottomNodes = new Node[noOfNodes];
    for(int i = 0; i < noOfNodes; i++){
        topNodes[i].val = i + 1;
        bottomNodes[i].val = i + 1;
    }


    // Storing edges in class array
    Edge* edges = new Edge[noOfEdges];
    for(int i = 0; i < noOfEdges; i++){
        edges[i].startingEdge = edgeArray[i][0];
        edges[i].endingEdge = edgeArray[i][1];
    }
    int topLabel[noOfNodes], bottomLabel[noOfNodes];


    Edge* newedges = new Edge[noOfEdges];
    int temp, iteration = 0, n = 0;
    Node tempNode;
    bool sorted;
    int cuts[noOfNodes - 1];


    // Looping until sorted
    while(sorted == false){
        iteration++;
        cout << "Iteration No. " << iteration << ": " << endl;
        // Printing
        if(iteration % 2){
            printRow(topNodes, noOfNodes);
            cout << endl;
            printRow(bottomNodes, noOfNodes);
        }
        else{
            printRow(bottomNodes, noOfNodes);
            cout << endl;
            printRow(topNodes, noOfNodes);
        }
        cout << endl;


        // Finding cuts
        int cuts;
        int total = 0;
        int max = 0;
        cout << endl << "Cuts: ";
        for(int i = 0; i < noOfNodes - 1; i++){
            cuts = 0;
            for(int j = 0; j < i + 1; j++){
                for(int k = i + 1; k < noOfNodes; k++){
                    if(isEdge(topNodes[j].val, bottomNodes[k].val, edges, noOfEdges)){
                        cuts++;
                    }
                }             
            }
            for(int j = i + 1; j < noOfNodes; j++){
                for(int k = 0; k < i + 1; k++){
                    if(isEdge(topNodes[j].val, bottomNodes[k].val, edges, noOfEdges)){
                        cuts++;
                    }
                }
            }
            if(cuts > max)
                max = cuts;
            total += cuts;
            cout << cuts << " ";
        }
        cout << endl << "Total Cuts: " << total << "  Max Cut: " << max;


        // Sorting Edges
        n = 0;
        for(int i = 0; i < noOfNodes; i++){
            for(int j = 0; j < noOfNodes; j++){
                for(int k = 0; k < noOfEdges; k++){
                    if(edges[k].startingEdge == topNodes[i].val && edges[k].endingEdge == bottomNodes[j].val){
                        newedges[n++] = edges[k];
                        break;
                    }
                }
            }   
        }
        for(int i = 0; i < noOfEdges; i++)
            edges[i] = newedges[i];
        

        // Finding Intersections
        int crosses = 0;
        for(int i = 0; i < noOfEdges; i++){
            for(int j = i; j < noOfEdges; j++){
                if(positionNode(edges[i].startingEdge, topNodes, noOfNodes) < positionNode(edges[j].startingEdge, topNodes, noOfNodes) && positionNode(edges[i].endingEdge, bottomNodes, noOfNodes) > positionNode(edges[j].endingEdge, bottomNodes, noOfNodes))
                    crosses++;
            }
        }
        cout << "  Intersections: " << crosses << endl;


        // Labelling
        for(int i = 0; i < noOfNodes; i++){
            int k = 0;
            while(edges[k].startingEdge != topNodes[i].val)
                k++;
            topLabel[i] = k + 1;
        }
        for(int i = 0; i < noOfNodes; i++){
            int k = 0;
            while(edges[k].endingEdge != bottomNodes[i].val)
                k++;
            bottomLabel[i] = k + 1;
        }
        sorted = true;
        for(int i = 0; i < noOfNodes - 1; i++){
            if(topLabel[i] > topLabel[i + 1] || bottomLabel[i] > bottomLabel[i + 1]){
                sorted = false;
            }
        }
        if(sorted){
            break;
        }


        // Sorting Nodes
        for(int i = 1; i < noOfNodes; i++){
            for(int j = 0; j < noOfNodes - 1; j++){
                if(bottomLabel[j] > bottomLabel[j + 1]){
                    temp = bottomLabel[j];
                    bottomLabel[j] = bottomLabel[j + 1];
                    bottomLabel[j + 1] = temp;
                    tempNode = bottomNodes[j];
                    bottomNodes[j] = bottomNodes[j + 1];
                    bottomNodes[j + 1] = tempNode;
                }
            }
        }


        // Swapping Bottom Nodes and Top Nodes
        for(int i = 0; i < noOfNodes; i++){
            temp = topNodes[i].val;
            topNodes[i].val = bottomNodes[i].val;
            bottomNodes[i].val = temp;
        }


        // Swaping Starting Nodes and Ending Nodes
        for(int i = 0; i < noOfEdges; i++){
            temp = edges[i].startingEdge;
            edges[i].startingEdge = edges[i].endingEdge;
            edges[i].endingEdge = temp;
        }
        cout << endl << endl;
    }

    return 0;
}