/**
 *  CSE 100 PA4 Pathfinder and Movietravler
 *  Filename: ActorGraph.hpp
 *  Author: Peiqi Fan, Shihui Zhu
 *  Userid: cs100sp19an, cs100sp19ck
 *  Description: This file contains and builds the graph structure
 *  Sources of help: Piazza
 **/

#ifndef ACTORGRAPH_HPP
#define ACTORGRAPH_HPP

#include <iostream>
#include <utility>
#include "Movie.hpp"
#include "ActorNode.hpp"
#include "ActorEdge.hpp"
#include <iterator>
#include <vector>
#include <queue>
#include <unordered_map>
#include <map>

/* Way to redefine the hash function of unordered_map
 * Adapted from Daniel Frey from stackoverflow
 * https://stackoverflow.com/questions/19655733/how-to-use-unordered-set-
 * that-has-elements-that-are-vector-of-pairint-int/19740245
 * Insert the self-defined hash function to the std namespace,
* and adapt the hash function from boost::hash_combine to hash the pairs
* */
#define HASH_COMBINE_CONST 0x9e3779b9
#define CONST1 6
#define CONST2 2
namespace
{
  template<typename T>
  // helper method of hash function
  std::size_t make_hash(const T& v)
  {
      return std::hash<T>()(v);
  }

  // hashfunction that adapted from boost::hash_combine
  void hash_combine(std::size_t& h, const std::size_t& v)
  {
    h ^= v + HASH_COMBINE_CONST + (h << CONST1) + (h >> CONST2);
  }

  // hash any container
  template<typename T>
    struct hash_container
    {
      size_t operator()(const T& v) const
      {
        size_t h = 0;
        for( const auto& e : v ) {
          hash_combine(h, make_hash(e));
        }
        return h;
      }
    };
}
// Inser the hash function into the namespace
namespace std
{
  // support for pair<T,U> if T and U can be hashed
  template<typename T, typename U>
  struct hash<pair<T, U>>
  {
      size_t operator()(const pair<T,U>& v) const
      {
          size_t h=make_hash(v.first);
          hash_combine(h, make_hash(v.second));
          return h;
      }
  };
}

/** A 'function class' for use as the Compare class in a
 *  priority_queue<ActorNode*>.
 *  For this to work, operator< must be defined to
 *  do the right thing on ActorNodes.
 */
class pairComp {
  public:
    bool operator()(const pair<int, ActorNode*>& p1,
        const pair<int, ActorNode*>& p2) {
      if (p1.first == p2.first){
        return p1.second > p2.second;
      }
      return p1.first > p2.first;
    }
};


/**
 * ActorGraph
 * Class that builds a graph structure as the container of all the
 *   actors/actress and the movie information
 */
class ActorGraph {

  public:

    // Maybe add class data structure(s) here
    unordered_map<string, ActorNode*> actGraph;
    unordered_map<pair<string, int>, Movie*> movGraph;
    vector<ActorEdge*> edgesSet;
    /**
     * Constuctor of the Actor graph
     */
    ActorGraph(void);

    /**
     * Destructor of the Actor graph
     */
    ~ActorGraph();

    // Maybe add some more methods here

    /** You can modify this method definition as you wish
     *
     * Load the graph from a tab-delimited file of actor->movie relationships.
     *
     * in_filename - input filename
     * use_weighted_edges
     *    - if true, compute edge weights as 1 + (2019 - movie_year),
     *    - otherwise all edge weights will be 1
     *
     * return true if file was loaded sucessfully, false otherwise
     */
    bool loadFromFile(const char* in_filename, bool use_weighted_edges);
    void updateEdges();

  private:
    void deleteAll();
};

#endif // ACTORGRAPH_HPP
