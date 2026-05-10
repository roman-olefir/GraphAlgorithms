#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "Graph.h"

constexpr int INF = 1e9;

Result runDijkstra(const Graph& g, int startNode);
Result runBellmanFord(const Graph& g, int startNode);
Result runSPFA(const Graph& g, int startNode);

void printPath(const Result& res, int targetNode);

#endif