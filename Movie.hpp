/**
 *  CSE 100 PA4 Pathfinder and Movietravler
 *  Filename: Movie.hpp
 *  Author: Peiqi Fan, Shihui Zhu
 *  Userid: cs100sp19an, cs100sp19ck
 *  Description: This file implements instances that
 *    store the movie information
 *  Sources of help: Piazza
 **/

#ifndef MOVIE_HPP
#define MOVIE_HPP

#include <iostream>
#include <string.h>
#include <vector>

/* Define ActorNode first for Makefile compilation */
class ActorNode;

using namespace std;

/** A class, stores movie information in an ActorGraph.
*/
class Movie {

  public:
    // name and year of the movie
    string name;
    int year;
    vector<ActorNode *> actors;

    // constructor
    Movie(string n, int y)
      : name(n), year(y) {}
};

#endif // MOVIE_HPP
