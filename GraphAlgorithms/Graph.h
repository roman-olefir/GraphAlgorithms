#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <iostream>
#include <random>

// Структура для ребра у списку суміжності (для Дейкстри/SPFA)
struct Edge {
    int to; // куди
    int weight; // довжина
};

// Структура для повного опису ребра (для Беллмана-Форда)
struct FullEdge {
    int from; // звідки
    int to; // куди
    int weight; // довжина
};

// Структура результату, яку повертатимуть усі алгоритми
struct Result {
    std::vector<int> dist;      // Відстані від стартової вершини
    std::vector<int> parent;    // Масив предків для відновлення шляху
    double timeMs;              // Час виконання
    long long relaxations = 0;  // Спроби оновити шлях
    long long queueOps = 0;     // push та pop операції
    bool hasNegativeCycle;      // Чи знайдено від'ємний цикл
    std::string algorithmName;  // Назва алгоритму для звіту

    Result() : timeMs(0), hasNegativeCycle(false) {}
};

class Graph {
private:
    int V; // вершини
    int E; // ребра
    std::vector<std::vector<Edge>> adj;
    std::vector<FullEdge> allEdges;

public:
    Graph(int vertices = 0);

    void addEdge(int u, int v, int w);
    void clear();
    void generateRandom(int vertices, int edges, int minW, int maxW);

    // Гетери
    int getV() const { return V; }
    int getE() const { return E; }
    const std::vector<std::vector<Edge>>& getAdj() const { return adj; }
    const std::vector<FullEdge>& getAllEdges() const { return allEdges; }

    void print() const;
};
#endif