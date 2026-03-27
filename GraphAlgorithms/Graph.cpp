#include <vector>
#include <iostream>
#include <random>
#include <algorithm>
#include "Graph.h"

using namespace std;

Graph::Graph(int vertices) : V(vertices), E(0) {
    adj.resize(V);
}

// Додавання ребра
void Graph::addEdge(int u, int v, int w) {
    if (u >= 0 && u < V && v >= 0 && v < V) {
        adj[u].push_back({ v, w });
        allEdges.push_back({ u, v, w });
        E++;
    }
}

// Очищення графа
void Graph::clear() {
    adj.clear();
    allEdges.clear();
    V = 0;
    E = 0;
}

// Генерація випадкового графа
void Graph::generateRandom(int vertices, int edges, int minW, int maxW) {
    clear();
    this->V = vertices;
    adj.resize(V);

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> vDist(0, V - 1);
    uniform_int_distribution<> wDist(minW, maxW);

    for (int i = 0; i < edges; ++i) {
        int u = vDist(gen);
        int v = vDist(gen);
        int w = wDist(gen);

        // Уникаємо петель (ребро саме в себе)
        if (u != v) {
            addEdge(u, v, w);
        }
        else {
            i--; // Повторна спроба, щоб набрати потрібну кількість ребер
        }
    }
}

// Вивід графа у консоль
void Graph::print() const {
    for (int i = 0; i < V; ++i) {
        cout << i << ": ";
        for (auto& edge : adj[i]) {
            cout << "-> " << edge.to << " (" << edge.weight << ") ";
        }
        cout << endl;
    }
}