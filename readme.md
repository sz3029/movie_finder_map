/**
 *  CSE 100 PA2 C++ PATHFINDER
 *  Filename: readme.md
 *  Author: Peiqi Fan, Shihui Zhu
 *  Userid: cs100sp19an, cs100sp19ck
 *  Description: This file answers the question in pa4
 *  Sources of help: Piazza
 *  */

 1. To examine whether this is a connected graph, we can perform
 a BFS on the graph from any given arbitrary vertex, and if the BFS visits all
 the vertices of the graph, then we have a connected graph. We only need one
 BFS performed, but we need to check every vertex after the search. So, the
 runtime is O(|V| + |E|) + O(|V) = O(|V| + |E|).

 2. We can first perform a BFS on an arbitrary vertex. Then if it doesn't visit
 all the vertices, we perform a BFS on one of the remaining vertices, if after
 that we still have unvisited vertices, repeat the above step until there is no
 vertex left. The runtime is still O(|V| + |E|).

 3. We can create a test file with all the source actor as Kevin Bacon, and
 the destination actor as all the other actors. And we should do a check to
 see whether there is a path that exceeds 6 edges ( 6 movies ). The runtime is
 (# of actors) * O(|V| + |E|) = O(|V|^2 + |V||E|).
