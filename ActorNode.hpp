/**
 *  CSE 100 PA4 Pathfinder and Movietravler
 *  Filename: ActorNode.hpp
 *  Author: Peiqi Fan, Shihui Zhu
 *  Userid: cs100sp19an, cs100sp19ck
 *  Description: This file implements the node of the graph structure
 *  Sources of help: Piazza
 **/

#ifndef ACTORNODE_HPP
#define ACTORNODE_HPP

#include <iostream>
#include <string.h>
#include <vector>
#include <climits>
#include <vector>
#include "Movie.hpp"

using namespace std;
/* Define ActorEdge first for Makefile compilation */
class ActorEdge;
/** A class, instances of which are nodes in an ActorGraph.
*/
class ActorNode {

  public:
    // name of the actor
    string name;
    int dist;
    ActorNode * prev;
    vector<ActorEdge*> edges;
    vector<Movie*> movies;
    Movie * prevMovie;
    bool done;
    ActorNode * parent;
    int size;

    // constructor
    ActorNode(string n) : name(n) {
      dist = INT_MAX;
      prev = nullptr;
      prevMovie = nullptr;
      done = false;
      parent = this;
      size = 1;
    }

};

#endif // ACTORNODE_HPP
