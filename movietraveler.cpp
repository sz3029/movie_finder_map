/**
 *  CSE 100 PA2 C++ Movie Traveler
 *  Filename: movietraveler.cpp
 *  Author: Peiqi Fan, Shihui Zhu
 *  Userid: cs100sp19an, cs100sp19ck
 *  Description: This file implements main program of
 *    movietravler which connects all the movies by
 *    a connected, edge-weighted, undirected actor/actress graph
 *  Sources of help: Piazza, Professor Cao
 *  */

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <algorithm>
#include "ActorGraph.hpp"
#include <queue>
#include <climits>

using namespace std;
// For weight calculation
#define YEAR 2019
// Number of arguments
#define ARGUMENTS 3
// global variables used in command line arguments
const int fileMov = 1;// Name of text file containing the tab-delimited
                        //  movie casts
const int output = 2;// Name of the output file

/** Check if a given data file is valid. Note that his does not
 * check if the data file is in correct format
 */
bool fileValid(const char * fileName) {

  ifstream in;
  in.open(fileName, ios::binary);

  // Check if input file was actually opened
  if(!in.is_open()) {
    cout<< "Invalid input file. No file was opened. Please try again.\n";
    in.close();
    return false;
  }

  // Check for empty file
  in.seekg(0, ios_base::end);
  int len = (int) in.tellg();
  if(len == 0) {
    cout << "The file is empty. \n";
    in.close();
    return false;
  }

  in.close();
  return true;
}

/* Find() of disjoint sets
 * Find the root node of the given node, as
 * */
ActorNode * findRoot(ActorNode * node){
  while(node->parent != node) {
    node->parent = node->parent->parent;
    node = node->parent;
  }
  return node;
}

/* Union() of disjoint sets
 * Merge two sets by size
 * */
void Union( ActorNode * node1, ActorNode * node2 ) {
    ActorNode * n1 = findRoot(node1);
    ActorNode * n2 = findRoot(node2);
    // Merge by size
    if ( n1->size > n2->size ) {
      // Merge the sentinel of node2 to the sentinel of node1
      n2->parent = n1;
      n1->size += n2->size;
    } else {
      // Merge the sentinel of node1 to the sentinel of node2
      n1->parent = n2;
      n2->size += n1->size;
    }
}

/* The Kruskal algorithm
 * Find the minimum spanning tree from the graph of actor
 * */
vector<ActorEdge*> Kruskal( ActorGraph * theGraph) {

    vector<ActorEdge*> MST;
    // Sort the edges in increasing order of weight
    sort(theGraph->edgesSet.begin(), theGraph->edgesSet.end(),
         [](ActorEdge * p1, ActorEdge * p2){return p1->weight < p2->weight;});

    int i = 0;
    // Build minimum spanning tree that connects all the vertices
    while ( MST.size() != (theGraph->actGraph.size() - 1) ) {
        // For each of the edge in the edgesSet
        ActorEdge * edge = theGraph->edgesSet[i];
        i++;
        // Find the sentinel node of each vertex
        ActorNode * node1 = findRoot(edge->src);
        ActorNode * node2 = findRoot(edge->dest);

        if ( node1 != node2 ) {
          MST.push_back(edge);
          Union(node1, node2);
        }
    }
    return MST;
}

/**
 * Main driver of the function
 * Take 2 command-line arguments, The first argument is the name
 *   of a text file containing the movie casts, the second is the
 *   the name of the output file containing the information of your
 *   minimum spanning tree
 */
int main(int argc, char** argv) {

    // check if the command is valid
    if ( argc != ARGUMENTS ) {
        cout << argv[0] << " called with incorrect arguments." << endl;
        return -1;
    }

    // stop and return -1 if the input file is not valid
    if (!fileValid(argv[fileMov])){
        return -1;
    }

    // Create the actor graph
    ActorGraph * theGraph = new ActorGraph();

    // open the file
    ifstream in;
    in.open(argv[fileMov], ios::binary);

    // Load data from file
    theGraph->loadFromFile(argv[fileMov], false);

    bool have_header = false;

    // open the output file
    ofstream out;
    out.open(argv[output], ios::trunc);

    // print header
    out << "(actor)<--[movie#@year]-->(actor)" << endl;

    vector<ActorEdge*> result = Kruskal(theGraph);
    int tWeights = 0;

    // print the edges
    for (int i = 0; i < result.size(); i++) {
        out << "(" << result[i]->src->name << ")";
        out << "<--[" << result[i]->movie->name;
        out << "#@" << result[i]->movie->year << "]";
        out << "-->(" << result[i]->dest->name << ")" << endl;
        tWeights += result[i]->weight;
    }

    // print the statistics
    out << "#NODE CONNECTED: " << result.size()+1 << endl;
    out << "#EDGE CHOSEN: " << result.size() << endl;
    out << "TOTAL EDGE WEIGHTS: " << tWeights << endl;

    // close files
    in.close();
    out.close();

    // free the space
    delete theGraph;
    return true;
}
