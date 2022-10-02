/**
 *  CSE 100 PA4 Pathfinder and Movietravler
 *  Filename: ActorEdge.hpp
 *  Author: Peiqi Fan, Shihui Zhu
 *  Userid: cs100sp19an, cs100sp19ck
 *  Description: This file implements the edge of the graph structure
 *  Sources of help: Piazza
 **/
#ifndef ACTOREDGE_HPP
#define ACTOREDGE_HPP

#include <iostream>
#include <string.h>
#include <vector>
#include <climits>
#include "ActorNode.hpp"
#include "Movie.hpp"

using namespace std;

/** A class, instances of which are edges in an ActorGraph.
*/
class ActorEdge {

  public:
    // source and destination node
    ActorNode * src, * dest;
    Movie * movie;
    int weight;

    // constructor
    ActorEdge(ActorNode* source, ActorNode* destination, Movie* mov)
      : src(source), dest(destination), movie(mov) {}
};

/** The comparator used in sorting edges based on dest's name */
struct CompareWeight {
  bool operator() (ActorEdge* const & p1, ActorEdge* const & p2) {
    return p1->weight > p2->weight;
  }
};
#endif // ACTOREDGE_HPP
