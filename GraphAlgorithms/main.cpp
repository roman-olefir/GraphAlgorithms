#include <iostream>
#include <iomanip>
#include "Graph.h"
#include "Algorithms.h"

using namespace std;

// Резутати роботи алгоритмів
Result resultDijkstra, resultBF, resultSPFA;

// Функція для красивого виводу результатів у таблицю
void printResultRow(const Result& res) {
    cout << left << setw(15) << res.algorithmName
        << setw(10) << fixed << setprecision(4) << res.timeMs
        << setw(15) << fixed << setprecision(4) << res.relaxations
        << setw(20) << fixed << setprecision(4) << res.queueOps
        << (res.hasNegativeCycle ? "YES (Warning!)" : "No") << endl;
}

void runBenchmarkMenu(Graph& g) {
    int v, e, minW, maxW;
    cout << "\n--- Налаштування Бенчмарку ---" << endl;
    cout << "Кількість вершин: "; cin >> v;
    cout << "Кількість ребер: "; cin >> e;
    cout << "Мінімальна вага: "; cin >> minW;
    cout << "Максимальна вага: "; cin >> maxW;

    // Генеруємо граф
    g.generateRandom(v, e, minW, maxW);

    cout << "\nРезультати тестування (час у ms):" << endl;
    cout << "-----------------------------------------------------------------------------" << endl;
    cout << left << setw(15) << "Алгоритм" << setw(10) << "Час" << setw(15) << "Релаксації" << setw(20) << "Опер. з чергою" << setw(40) << "Від'ємний цикл" << endl;
    cout << "-----------------------------------------------------------------------------" << endl;

    // Запускаємо алгоритми від 0 вершини
    if (minW < 0) {
        cout << "Dijkstra: Пропущено (алгоритм не підтримує від'ємні ваги)" << endl;
    }
    else {
        resultDijkstra = runDijkstra(g, 0);
        printResultRow(resultDijkstra);
    }

    resultBF = runBellmanFord(g, 0);
    printResultRow(resultBF);

    resultSPFA = runSPFA(g, 0);
    printResultRow(resultSPFA);
    cout << "-----------------------------------------------------------------------------" << endl;
}

void runPathComparison(const Graph& g) {
    if (g.getV() == 0) {
        cout << "Спочатку згенеруйте граф!" << endl;
        return;
    }

    int startNode = 0;
    int targetNode;
    cout << "Введіть номер цільової вершини (0 - " << g.getV() - 1 << "): ";
    cin >> targetNode;

    if (targetNode < 0 || targetNode >= g.getV()) {
        cout << "Невірна вершина!" << endl;
        return;
    }

    // Перевірка на від'ємні ваги для Дейкстри
    bool hasNeg = false;
    for (const auto& e : g.getAllEdges()) if (e.weight < 0) hasNeg = true;

    cout << "\n=== ПОРІВНЯННЯ МАРШРУТІВ ВІД ВЕРШИНИ " << startNode << " ДО " << targetNode << " ===" << endl;

    // ВИВІД ДЕЙКСТРИ
    if (!hasNeg) {
        cout << "[Dijkstra]     ";
        printPath(resultDijkstra, targetNode);
    }
    else {
        cout << "[Dijkstra]     Пропущено: знайдено від'ємні ваги." << endl;
    }

    // ВИВІД БЕЛЛМАНА-ФОРДА
    cout << "[Bellman-Ford] ";
    printPath(resultBF, targetNode);

    // ВИВІД SPFA
    cout << "[SPFA]         ";
    printPath(resultSPFA, targetNode);

    cout << "==========================================================" << endl;
}

int main() {
#ifdef _WIN32
    system("chcp 1251 > nul");
#endif

    Graph myGraph;
    int choice;

    while (true) {
        cout << "\n===== КУРСОВА РОБОТА: АНАЛІЗ АЛГОРИТМІВ ГРАФІВ =====" << endl;
        cout << "1. Згенерувати випадковий граф та запустити бенчмарк" << endl;
        cout << "2. Порівняти шляхи до конкретної вершини" << endl;
        cout << "3. Вивести поточну структуру графа" << endl;
        cout << "4. Вийти" << endl;
        cout << "Ваш вибір: ";
        cin >> choice;

        if (choice == 1) {
            runBenchmarkMenu(myGraph);
        }
        else if (choice == 2) {
            runPathComparison(myGraph);
        }
        else if (choice == 3) {
            myGraph.print();
        }
        else if (choice == 4) {
            break;
        }
        else {
            cout << "Невірний вибір!" << endl;
        }
    }
    return 0;
}