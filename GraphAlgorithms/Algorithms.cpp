#include "Algorithms.h"
#include <iostream>
#include <algorithm>
#include <chrono>
#include <queue>
#include <functional> // для std::greater

using namespace std;
using namespace std::chrono;

// Відновлення шляху за масивом предків (parent)
void printPath(const Result& res, int targetNode) {
    if (res.dist[targetNode] == INF) {
        cout << "Шлях до вершини " << targetNode << " не існує." << endl;
        return;
    }

    if (res.hasNegativeCycle) {
        cout << "Неможливо визначити шлях: знайдено від'ємний цикл!" << endl;
        return;
    }

    vector<int> path;
    // Йдемо від фінішу до старту по масиву parent
    for (int v = targetNode; v != -1; v = res.parent[v]) {
        path.push_back(v);
    }
    reverse(path.begin(), path.end());

    cout << "Шлях: ";
    for (int i = 0; i < path.size(); ++i) {
        cout << path[i] << (i == path.size() - 1 ? "" : " -> ");
    }
    cout << " (Вага: " << res.dist[targetNode] << ")" << endl;
}

// Алгоритм Дейкстри
Result runDijkstra(const Graph& g, int startNode) {
    Result res;
    res.algorithmName = "Dijkstra";

    int V = g.getV();
    const auto& adj = g.getAdj(); // Використовуємо список суміжності

    res.dist.assign(V, INF);
    res.parent.assign(V, -1);
    res.dist[startNode] = 0;

    auto start = high_resolution_clock::now();

    // Пріоритетна черга: pair<відстань, вершина>
    // greater дозволяє завжди діставати вершину з МІНІМАЛЬНОЮ відстанню
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    pq.push({ 0, startNode });
    res.queueOps++;

    while (!pq.empty()) {
        int d = pq.top().first;
        int u = pq.top().second;
        pq.pop();
        res.queueOps++;

        // Якщо ми вже знайшли коротший шлях до цієї вершини раніше, то ігноруємо
        if (d > res.dist[u]) continue;
        // Перебираємо всіх сусідів поточної вершини
        for (const auto& edge : adj[u]) {
            res.relaxations++;
            if (res.dist[u] + edge.weight < res.dist[edge.to]) {
                res.dist[edge.to] = res.dist[u] + edge.weight;
                res.parent[edge.to] = u;
                pq.push({ res.dist[edge.to], edge.to });
                res.queueOps++;
            }
        }
    }

    auto end = high_resolution_clock::now();
    res.timeMs = duration_cast<microseconds>(end - start).count() / 1000.0;
    return res;
}

// Алгоритм Беллмана-Форда
Result runBellmanFord(const Graph& g, int startNode) {
    Result res;
    res.algorithmName = "Bellman-Ford";

    int V = g.getV();
    const auto& allEdges = g.getAllEdges(); // Отримуємо плоский список ребер

    res.dist.assign(V, INF);
    res.parent.assign(V, -1);
    res.dist[startNode] = 0;

    auto start = high_resolution_clock::now();

    // Робимо V-1 проходів по всіх ребрах
    for (int i = 1; i <= V - 1; ++i) {
        bool changed = false;
        for (const auto& edge : allEdges) {
            res.relaxations++;
            if (res.dist[edge.from] != INF && res.dist[edge.from] + edge.weight < res.dist[edge.to]) {
                res.dist[edge.to] = res.dist[edge.from] + edge.weight;
                res.parent[edge.to] = edge.from;
                changed = true;
            }
        }
        if (!changed) break; // Якщо за прохід нічого не змінилось, то виходимо раніше (оптимізація)
    }

    // Окремий прохід для перевірки на від'ємні цикли
    for (const auto& edge : allEdges) {
        if (res.dist[edge.from] != INF && res.dist[edge.from] + edge.weight < res.dist[edge.to]) {
            res.hasNegativeCycle = true;
            break;
        }
    }

    auto end = high_resolution_clock::now();
    res.timeMs = duration_cast<microseconds>(end - start).count() / 1000.0;
    return res;
}

// Алгоритм SPFA
Result runSPFA(const Graph& g, int startNode) {
    Result res;
    res.algorithmName = "SPFA";

    int V = g.getV();
    const auto& adj = g.getAdj();

    res.dist.assign(V, INF);
    res.parent.assign(V, -1);
    res.dist[startNode] = 0;

    // Масив для відстеження вершин, які вже знаходяться в черзі
    vector<bool> inQueue(V, false);
    // Масив для підрахунку кількості релаксацій кожної вершини (для виявлення циклів)
    vector<int> count(V, 0);

    queue<int> q;
    q.push(startNode);
    res.queueOps++;
    inQueue[startNode] = true;

    auto start = high_resolution_clock::now();

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        res.queueOps++;
        inQueue[u] = false;

        for (const auto& edge : adj[u]) {
            res.relaxations++;
            if (res.dist[u] + edge.weight < res.dist[edge.to]) {
                res.dist[edge.to] = res.dist[u] + edge.weight;
                res.parent[edge.to] = u;

                if (!inQueue[edge.to]) {
                    q.push(edge.to);
                    res.queueOps++;
                    inQueue[edge.to] = true;
                    count[edge.to]++;

                    // Якщо вершина оновлювалася більше ніж V разів, то це від'ємний цикл
                    if (count[edge.to] >= V) {
                        res.hasNegativeCycle = true;
                        goto end_spfa; // Вихід з циклів
                    }
                }
            }
        }
    }

end_spfa:
    auto end = high_resolution_clock::now();
    res.timeMs = duration_cast<microseconds>(end - start).count() / 1000.0;
    return res;
}