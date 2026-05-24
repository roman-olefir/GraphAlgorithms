#include <iostream>
#include <iomanip>
#include <limits>
#include "Graph.h"
#include "Algorithms.h"
#include <string>

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

    // функція для валідації
    auto getValidatedInt = [](string prompt, int minVal, int maxVal) {
        int val;
        while (true) {
            cout << prompt;
            if (cin >> val && val >= minVal && val <= maxVal) {
                return val;
            }
            else {
                cout << "Помилка! Введіть ціле число в діапазоні від " << minVal << " до " << maxVal << "." << endl;
                cin.clear(); // скидаємо стан помилки
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // очищуємо буфер
            }
        }
        };

    cout << "\n--- Налаштування Бенчмарку ---" << endl;

    // обмежененя 10000 вершин для стабільності 
    v = getValidatedInt("Кількість вершин (2-50000): ", 2, 50000);

    // кількість ребер не може бути більше ніж v*(v-1) для орієнтованого графа
    long long maxPossibleEdges = (long long)v * (v - 1);
    if (maxPossibleEdges > 50000) maxPossibleEdges = 500000; // обмеження

    e = getValidatedInt("Кількість ребер (1-" + to_string(maxPossibleEdges) + "): ", 1, (int)maxPossibleEdges);

    minW = getValidatedInt("Мінімальна вага (-1000..1000): ", -1000, 1000);
    maxW = getValidatedInt("Максимальна вага (повинна бути >= " + to_string(minW) + "): ", minW, 1000);

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