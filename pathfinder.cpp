/**
 *  CSE 100 PA2 C++ Pathfinder
 *  Filename: pathfinder.cpp
 *  Author: Peiqi Fan, Shihui Zhu
 *  Userid: cs100sp19an, cs100sp19ck
 *  Description: This file implements main program of
 *    pathfinder which finds the shortes path from
 *    one actor to another through movies
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
#define ARGUMENTSPATH 5
// global variables used in command line arguments
const int fileMov = 1;// Name of text file containing the tab-delimited
//  movie casts
const int uw = 2;// denote to use weighted or unweighted graph
const int fileAct = 3;// Name of text file containing the tab-delimited
//  actor pairs
const int output = 4;

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

/* The Dijkstra algorithm
 * Find the shortest path from source actor to destination actor
 * */
void Dijkstra( ActorGraph * theGraph, string actorS, string actorD ) {
  // initialize an empty priority queue
  priority_queue<pair<int, ActorNode*>,
    vector<pair<int, ActorNode*>>, pairComp> toExplore;

  // initialize all of the ActorNodes's fields
  for (auto i = theGraph->actGraph.begin();
         i != theGraph->actGraph.end(); ++i){
    i->second->dist = INT_MAX;
    i->second->prev = nullptr;
    i->second->prevMovie = nullptr;
    i->second->done = false;
  }

  // enqueue the starting vertex
  ActorNode * start = theGraph->actGraph[actorS];
  // initialze the first element if it exists
  start->dist = 0;
  toExplore.push(make_pair(0, start));
  // applying Dijkstra algorithm
  while ( !toExplore.empty() ) {
    ActorNode * next = toExplore.top().second;
    toExplore.pop();
    // if the vertex's min path hasn't been discovered yet
    if (!next->done){
      next->done = true;
      auto it = next->edges.begin();
      // iterate the edges
      for ( ; it != next->edges.end(); ++it ) {
        ActorNode * neighbor = theGraph->actGraph[(*it)->dest->name];
        int c = next->dist + 1;
        // if a smaller-weight path has been found
        if (c < neighbor->dist){
          neighbor->dist = c;
          neighbor->prev = next;
          neighbor->prevMovie = (*it)->movie;
          toExplore.push(make_pair(c, neighbor));
          // stop if the destination is reached
          if (!neighbor->name.compare(actorD)){
            return;
          }
        }
      }
    }
  }
}

/* The Dijkstra algorithm
* Find the shortest path from source actor to destination actor
* This time the graph is weighted
* */
void DijkstraWeight( ActorGraph * theGraph, string actorS, string actorD ) {
  // initialize an empty priority queue
  priority_queue<pair<int, ActorNode*>,
    vector<pair<int, ActorNode*>>, pairComp> toExplore;

  // initialize all of the ActorNodes's fields
  for (auto i = theGraph->actGraph.begin();
         i != theGraph->actGraph.end(); ++i){
    i->second->dist = INT_MAX;
    i->second->prev = nullptr;
    i->second->prevMovie = nullptr;
    i->second->done = false;
  }

  // enqueue the starting vertex
  ActorNode * start = theGraph->actGraph[actorS];
  // initialze the first element if it exists
  start->dist = 0;
  toExplore.push(make_pair(0, start));
  // applying Dijkstra algorithm
  while ( !toExplore.empty() ) {
    ActorNode * next = toExplore.top().second;
    toExplore.pop();
    // if the vertex's min path hasn't been discovered yet
    if (!next->done){
      next->done = true;
      auto it = next->edges.begin();
      // iterate the edges
      for ( ; it != next->edges.end(); ++it ) {
        ActorNode * neighbor = theGraph->actGraph[(*it)->dest->name];
        // Calculate the year of the movie
        int c = next->dist + (1 + (YEAR - (*it)->movie->year));
        // if a smaller-weight path has been found
        if (c < neighbor->dist){
          neighbor->dist = c;
          neighbor->prev = next;
          neighbor->prevMovie = (*it)->movie;
          toExplore.push(make_pair(c, neighbor));
          // stop if the destination is reached
          if (!neighbor->name.compare(actorD)){
            return;
          }
        }
      }
    }
  }
}

/**
 * find the shortest path between two actors
 */
vector<pair<ActorNode*, Movie*>> shortestPath(ActorGraph* graph, string actorD){
  // vector to store paths
  vector<pair<ActorNode*, Movie*>> path;

  // return empty vector if actorD is not found
  if (graph->actGraph.find(actorD) == graph->actGraph.end()){
    return path;
  }

  // Find the destination actor and path to the actor
  ActorNode * dest = graph->actGraph[actorD];

  // Tranverse back to the source actor
  while (dest->prev){
    path.push_back(make_pair(dest, dest->prevMovie));
    dest = dest->prev;
  }

  // reverse the path vector
  std::reverse(path.begin(), path.end());
  return path;
}

/**
 * Main driver of the function
 * ./pathfinder will take 4 command-line arguments:
 * Name of text file containing the tab-delimited movie casts
 *   (such as imdb_2019.tsv).
 * Lower-case character u or w
 *   u -- builds the graph with unweighted edges
 *   w -- builds the graph with weighted edges (you should implement
 *        unweighted part first)
 * Name of text file containing the pairs of actors to find paths,
 *   where first line in the file is a header, and each row contains the
 *   names of the two actors separated by a single tab character
 * Name of output text file. Pathfinder will create a new file to store
 *   the results from finding the shortest path between two actors
 * The output text file should follow this exact format detailed here.
 * First line of the file is a header, and each row contains the paths for
 *   the corresponding pair of actors and input pairs file
 */
int main(int argc, char** argv) {

  // check if the command is valid
  if ( argc != ARGUMENTSPATH ) {
    cout << argv[0] << " called with incorrect arguments." << endl;
    return -1;
  }

  // stop and return -1 if the input file is not valid
  if (!fileValid(argv[fileMov])){
    return -1;
  }

  // stop and return -1 if the input mix file is not valid
  if (!fileValid(argv[fileAct])){
    return -1;
  }

  // Create the actor graph
  ActorGraph * theGraph = new ActorGraph();

  // builds the graph with unweighted edges
  if (strcmp(argv[uw], "u") == 0){

    // open the file
    ifstream in, test;
    in.open(argv[fileMov], ios::binary);
    test.open(argv[fileAct], ios::binary);

    // Load data from file
    theGraph->loadFromFile(argv[fileMov], false);

    bool have_header = false;

    // open the output file
    ofstream out;
    out.open(argv[output], ios::trunc);

    // keep reading lines until the end of file is reached
    while (test) {
      string s;
      // get the next line
      if (!getline( test, s )) break;

      if (!have_header) {
        // print header
        out << "(actor)--[movie#@year]-->(actor)--..." << endl;
        // skip the header
        have_header = true;
        continue;
      }

      istringstream ss( s );
      vector <string> record;

      while (ss) {
        string next;

        // get the next string before hitting a tab character and
        //  put it in 'next'
        if (!getline( ss, next, '\t' )) break;

        record.push_back( next );
      }

      if (record.size() != 2) {
        // we should have exactly 3 columns
        continue;
      }

      string actorS_name(record[0]);
      string actorD_name(record[1]);

      // Print the computing path line for user
      cout << "Computing path for (" << actorS_name <<
        ") -> (" << actorD_name << ")" << endl;

      // Print newline if actor to find is the same of the src
      if ( !actorS_name.compare(actorD_name) ) {
        out << endl;
        continue;
      }

      // Edge cases if actorS is not found
      if (theGraph->actGraph.find(actorS_name) == theGraph->actGraph.end()){
        cout << "Failed to locate node '" << actorS_name << "'" << endl;
        out << endl;
        continue;
      }

      // Find the shortest path using the Dijkstra algorithm
      Dijkstra(theGraph, actorS_name, actorD_name);

      // Hold the path found
      vector<pair<ActorNode*, Movie*>> path;
      path = shortestPath(theGraph, actorD_name);

      // Edge cases if actorD is not found
      if (path.empty()) {
        cout << "Failed to locate node '" << actorD_name << "'" << endl;
        out << endl;
        continue;
      }

      // print source Actor's name
      out << "(" << actorS_name << ")";

      // print the path into the output file
      for (int i = 0; i < path.size(); i++){
        out << "--[" << path[i].second->name;
        out << "#@" << path[i].second->year << "]";
        out << "-->(" << path[i].first->name << ")";
      }
      out << endl;
    }

    //  If error occurs during reading, print error message
    if (!test.eof()) {
      cerr << "Failed to read " << argv[fileAct] << "!\n";
      in.close();
      out.close();
      return false;
    }

    // close files
    in.close();
    out.close();

    // free the space
    delete theGraph;
    return true;
  }

  // builds the graph with weighted edges
  else if (strcmp(argv[uw], "w") == 0){
    // open the file
    ifstream in, test;
    in.open(argv[fileMov], ios::binary);
    test.open(argv[fileAct], ios::binary);

    // Load data from file
    theGraph->loadFromFile(argv[fileMov], false);

    bool have_header = false;

    // open the output file
    ofstream out;
    out.open(argv[output], ios::trunc);

    // keep reading lines until the end of file is reached
    while (test) {
      string s;
      // get the next line
      if (!getline( test, s )) break;

      if (!have_header) {
        // print header
        out << "(actor)--[movie#@year]-->(actor)--..." << endl;
        // skip the header
        have_header = true;
        continue;
      }

      istringstream ss( s );
      vector <string> record;

      while (ss) {
        string next;

        // get the next string before hitting a tab character and
        //  put it in 'next'
        if (!getline( ss, next, '\t' )) break;

        record.push_back( next );
      }

      if (record.size() != 2) {
        // we should have exactly 3 columns
        continue;
      }

      string actorS_name(record[0]);
      string actorD_name(record[1]);

      // Print the computing path line for user
      cout << "Computing path for (" << actorS_name <<
        ") -> (" << actorD_name << ")" << endl;

      // Print newline if actor to find is the same of the src
      if ( !actorS_name.compare(actorD_name) ) {
        out << endl;
        continue;
      }

      // Edge cases if actorS is not found
      if (theGraph->actGraph.find(actorS_name) == theGraph->actGraph.end()){
        cout << "Failed to locate node '" << actorS_name << "'" << endl;
        out << endl;
        continue;
      }

      // Find the shortest path using the Dijkstra algorithm
      DijkstraWeight(theGraph, actorS_name, actorD_name);
      // Hold the path found
      vector<pair<ActorNode*, Movie*>> path;
      path = shortestPath(theGraph, actorD_name);

      // Edge cases if actorD is not found
      if (path.empty()) {
        cout << "Failed to locate node '" << actorD_name << "'" << endl;
        out << endl;
        continue;
      }

      // print source Actor's name
      out << "(" << actorS_name << ")";

      // print the path into the output file
      for (int i = 0; i < path.size(); i++){
        out << "--[" << path[i].second->name;
        out << "#@" << path[i].second->year << "]";
        out << "-->(" << path[i].first->name << ")";
      }
      out << endl;
    }

    //  If error occurs during reading, print error message
    if (!test.eof()) {
      cerr << "Failed to read " << argv[fileAct] << "!\n";
      in.close();
      out.close();
      return false;
    }

    // close files
    in.close();
    out.close();

    // free the space
    delete theGraph;
    return true;
  }
  // if the charater is wrong or is not entered, return false
  else {
    cout << "wrong parameter '" << argv[fileMov] << "', must be u or w"
      << endl;
    return -1;
  }
}
