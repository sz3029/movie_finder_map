/**
 *  CSE 100 PA4 Pathfinder and Movietravler
 *  Filename: ActorGraph.cpp
 *  Author: Peiqi Fan, Shihui Zhu
 *  Userid: cs100sp19an, cs100sp19ck
 *  Description: This file contains and builds the graph structure
 *  Sources of help: Piazza
 **/

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "ActorGraph.hpp"
#include <algorithm>

// For weight calculation
#define YEAR 2019

using namespace std;

/**
 * Constructor of the Actor graph
 */
ActorGraph::ActorGraph(void) {}

/**
 * Destructor of the Actor graph
 */
ActorGraph::~ActorGraph(){
  deleteAll();
}

/** 
 * Load the graph from a tab-delimited file of actor->movie relationships.
 *
 * in_filename - input filename
 * use_weighted_edges
 *    - if true, compute edge weights as 1 + (2019 - movie_year),
 *    - otherwise all edge weights will be 1
 *
 * return true if file was loaded sucessfully, false otherwise
 */
bool ActorGraph::loadFromFile(const char* in_filename, bool use_weighted_edges) {
	cout << "Reading " << in_filename << " ..." << endl;

  // Initialize the file stream
  ifstream infile(in_filename);

  bool have_header = false;

  // keep reading lines until the end of file is reached
  while (infile) {
    string s;

    // get the next line
    if (!getline( infile, s )) break;

    if (!have_header) {

      // skip the header
      have_header = true;
      continue;
    }

    istringstream ss( s );
    vector <string> record;

    while (ss) {
      string next;

      // get the next string before hitting a tab character and put it in 'next'
      if (!getline( ss, next, '\t' )) break;

      record.push_back( next );
    }

    if (record.size() != 3) {
      // we should have exactly 3 columns
      continue;
    }

    string actor_name(record[0]);
    string movie_title(record[1]);
    int movie_year = stoi(record[2]);

	// TODO: we have an actor/movie relationship, now what?
    // search if the actor_name exists
    unordered_map<string, ActorNode*>::const_iterator resAct
      = actGraph.find (actor_name);

    // search if the movie exists
    unordered_map<pair<string, int>, Movie*>::const_iterator resMov
      = movGraph.find (make_pair(movie_title, movie_year));

    // create a new ActorNode if the actor does not exist
    if (resAct == actGraph.end()){
      ActorNode * actor = new ActorNode(actor_name);
      actGraph[actor_name] = actor;
      // create a new Movie and change the fields
      if (resMov == movGraph.end()){
        Movie * mov = new Movie(movie_title, movie_year);
        movGraph[make_pair(movie_title, movie_year)] = mov;
        (actor->movies).push_back(mov);
        (mov->actors).push_back(actor);
      }
      // change the fields of the new Actor and existed Movie
      else {
        (resMov->second->actors).push_back(actor);
        (actor->movies).push_back(resMov->second);
      }
    }
    // if the actor exists
    else {
      // create a new Movie and change fields
      if (resMov == movGraph.end()){
        Movie * mov = new Movie(movie_title, movie_year);
        movGraph[make_pair(movie_title, movie_year)] = mov;
        (resAct->second->movies).push_back(mov);
        mov->actors.push_back(resAct->second);
      }
      // change fields of the existed Actor and Movie
      else {
        (resMov->second->actors).push_back(resAct->second);
        (resAct->second->movies).push_back(resMov->second);
      }
    }
  }

  updateEdges();  // connect the verteices in the graph

  /*sort the edges based on the dest vertex's name
  for (auto i = actGraph.begin(); i != actGraph.end(); ++i){
    sort(i->second->edges.begin(), i->second->edges.end(), CompareDestName());
  }*/

  if (!infile.eof()) {
    cerr << "Failed to read " << in_filename << "!\n";
    return false;
  }
  infile.close();

  cout << "done" << endl;
  return true;
}

/**
 * Helper method to update the edges of the ActorNodes
 */
void ActorGraph::updateEdges() {
  // iterate the map (actorNode)
  for (auto node = actGraph.begin(); node != actGraph.end(); ++node){
    // iterate each movie in one actor node
    for (auto mov = node->second->movies.begin();
        mov != node->second->movies.end(); ++mov){
      // iterate each actor in one movie vector
      for (auto dst = (*mov)->actors.begin();
           dst != (*mov)->actors.end(); ++dst) {
        ActorEdge * newEdge = new ActorEdge(node->second, *dst, *mov);
        newEdge->weight = 1 + (YEAR - newEdge->movie->year);
        node->second->edges.push_back(newEdge);
        // Store the edge into the edgesSet
        edgesSet.push_back(newEdge);
      }
    }
  }
}

/**
 * Helper method of destructor
 */
void ActorGraph::deleteAll(){
  // delete movies
  for (auto i = movGraph.begin(); i != movGraph.end(); ++i){
    delete i -> second;
  }
  // delete actorNodes
  for (auto i = actGraph.begin(); i != actGraph.end(); ++i){
    for (auto j = i->second->edges.begin(); j != i->second->edges.end(); ++j){
      delete * j;
    }
    delete i -> second;
  }
}
