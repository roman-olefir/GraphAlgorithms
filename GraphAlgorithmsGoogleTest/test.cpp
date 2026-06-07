#include "pch.h"
#include "../GraphAlgorithms/Graph.h"
#include "../GraphAlgorithms/Algorithms.h"

// Тест 1. Базовий граф де всі алгоритми мають працювати однаково
TEST(ShortestPathTests, BasicGraphAllAlgorithms) {
    // Створюємо граф: 0 -> 1 (вага 2), 1 -> 2 (вага 3), 0 -> 2 (вага 10)
    Graph g(3);
    g.addEdge(0, 1, 2);
    g.addEdge(1, 2, 3);
    g.addEdge(0, 2, 10);

    // Запускаємо всі три алгоритми з вершини 0
    Result resDijkstra = runDijkstra(g, 0);
    Result resBF = runBellmanFord(g, 0);
    Result resSPFA = runSPFA(g, 0);

    // Перевіряємо найкоротшу відстань до вершини 2 (має бути 2 + 3 = 5, а не 10)
    EXPECT_EQ(resDijkstra.dist[2], 5);
    EXPECT_EQ(resBF.dist[2], 5);
    EXPECT_EQ(resSPFA.dist[2], 5);

    // Перевіряємо правильність масиву предків (parent) для відновлення шляху
    // Для вершини 2 предком має бути 1, для 1 предком має бути 0
    EXPECT_EQ(resDijkstra.parent[2], 1);
    EXPECT_EQ(resBF.parent[2], 1);
    EXPECT_EQ(resSPFA.parent[2], 1);
}

// Тест 2. Вершини, до яких немає шляху (незв'язний граф)
TEST(ShortestPathTests, UnreachableVertex) {
    // Граф з 3 вершин: 0 -> 1 (вага 5), вершина 2 ізольована
    Graph g(3);
    g.addEdge(0, 1, 5);

    Result resDijkstra = runDijkstra(g, 0);
    Result resBF = runBellmanFord(g, 0);
    Result resSPFA = runSPFA(g, 0);

    // Відстань до вершини 2 має залишатися INF
    EXPECT_EQ(resDijkstra.dist[2], INF);
    EXPECT_EQ(resBF.dist[2], INF);
    EXPECT_EQ(resSPFA.dist[2], INF);

    // Предка у вершини 2 бути не повинно (-1)
    EXPECT_EQ(resDijkstra.parent[2], -1);
    EXPECT_EQ(resBF.parent[2], -1);
    EXPECT_EQ(resSPFA.parent[2], -1);
}

// Тест 3. Граф з від'ємними ребрами (без циклів)
TEST(ShortestPathTests, NegativeEdgesNoCycles) {
    // Граф: 0 -> 1 (вага 4), 0 -> 2 (вага 3), 2 -> 1 (вага -2)
    // Найкоротший шлях до 1: 0 -> 2 -> 1 (вага 3 + (-2) = 1)
    Graph g(3);
    g.addEdge(0, 1, 4);
    g.addEdge(0, 2, 3);
    g.addEdge(2, 1, -2);

    Result resBF = runBellmanFord(g, 0);
    Result resSPFA = runSPFA(g, 0);

    // Беллман-Форд та SPFA ПОВИННІ знайти правильний шлях (= 1)
    EXPECT_EQ(resBF.dist[1], 1);
    EXPECT_EQ(resSPFA.dist[1], 1);

    EXPECT_FALSE(resBF.hasNegativeCycle);
    EXPECT_FALSE(resSPFA.hasNegativeCycle);

    // Примітка: Дейкстра на графах з від'ємними вагами може видати 4 замість 1,
    // тому ми його тут свідомо не перевіряємо на рівність 1.
}

// Тест 4. Виявлення від'ємного циклу
TEST(ShortestPathTests, NegativeCycleDetection) {
    // Створюємо від'ємний цикл: 0 -> 1 (вага 1), 1 -> 2 (вага -5), 2 -> 0 (вага 1)
    // Сумарна вага циклу: 1 + (-5) + 1 = -3 (це від'ємний цикл)
    Graph g(3);
    g.addEdge(0, 1, 1);
    g.addEdge(1, 2, -5);
    g.addEdge(2, 0, 1);

    Result resBF = runBellmanFord(g, 0);
    Result resSPFA = runSPFA(g, 0);

    // Перевіряємо, чи алгоритми засікли від'ємний цикл
    EXPECT_TRUE(resBF.hasNegativeCycle);
    EXPECT_TRUE(resSPFA.hasNegativeCycle);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}