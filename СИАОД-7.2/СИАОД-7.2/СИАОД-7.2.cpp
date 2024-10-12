#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <limits>

using namespace std;

struct Edge {
    int to;
    int weight;
    Edge(int to, int weight) {
        this->to = to;
        this->weight = weight;
    }
};

class Graph {
private:
    vector<vector<Edge>> adjacencyList;
public:
    Graph(int vertices) {
        adjacencyList.resize(vertices);
    }

    Graph() {
        adjacencyList.resize(8);
        int adjacencyMatrix[8][8] = {
            {-1, 23, 12,  0,  0,  0,  0,  0},
            {23, -1, 25,  0, 22,  0,  0, 35},
            {12, 25, -1, 18,  0,  0,  0,  0},
            { 0,  0, 18, -1,  0, 20,  0,  0},
            { 0, 22,  0,  0, -1, 23, 14,  0},
            { 0,  0,  0, 20, 23, -1, 24,  0},
            { 0,  0,  0,  0, 14, 24, -1, 16},
            { 0, 35,  0,  0,  0,  0, 16, -1}
        };
        for (int i = 0; i < 8; ++i) {
            for (int j = i; j < 8; ++j) {
                if (adjacencyMatrix[i][j] > 0) {
                    addEdge(i, j, adjacencyMatrix[i][j]);
                }
            }
        }
    }

    void addEdge(int from, int to, int weight) {
        adjacencyList[from].push_back({ to, weight });
        adjacencyList[to].push_back({from, weight});
    }

    int getWeight(int u, int v) { //вес ребра между u и v
        for (Edge edge : adjacencyList[u]) {
            if (edge.to == v) {
                return edge.weight;
            }
        }
        return -1;
    }

    void removeEdge(int u, int v, bool first = true) { //удаление ребра между вершинами u и v
        for (int i = 0; i < adjacencyList[u].size(); i++) {
            Edge rmEdge = adjacencyList[u][i];
            if (rmEdge.to == v) {
                adjacencyList[u].erase(adjacencyList[u].begin() + i); //удаляем элемент из матрицы смежности
                break;
            }
        }
        if (first) removeEdge(v, u, false); //удаление обратного пути
    }

    void removeNode(int node) {
        for (int i = 0; i < adjacencyList.size(); ++i) {
            adjacencyList[i].erase(remove_if(adjacencyList[i].begin(), adjacencyList[i].end(),
                [node](const Edge& edge) { return edge.to == node; }), adjacencyList[i].end());
        }
        adjacencyList[node].clear();
        for (int i = 0; i < adjacencyList.size(); ++i) {
            adjacencyList[i].erase(remove_if(adjacencyList[i].begin(), adjacencyList[i].end(),
                [node](const Edge& edge) { return edge.to == node; }), adjacencyList[i].end());
        }
    }

    void resetGraph() {
        // Восстанавливаем все удаленные ребра до первоначального состояния
        for (int i = 0; i < adjacencyList.size(); ++i) {
            adjacencyList[i].clear(); // Очищаем список ребер
        }

        // Заново добавляем ребра в граф, исходя из начальной конфигурации или матрицы смежности
        int adjacencyMatrix[8][8] = {
            {-1, 23, 12,  0,  0,  0,  0,  0},
            {23, -1, 25,  0, 22,  0,  0, 35},
            {12, 25, -1, 18,  0,  0,  0,  0},
            { 0,  0, 18, -1,  0, 20,  0,  0},
            { 0, 22,  0,  0, -1, 23, 14,  0},
            { 0,  0,  0, 20, 23, -1, 24,  0},
            { 0,  0,  0,  0, 14, 24, -1, 16},
            { 0, 35,  0,  0,  0,  0, 16, -1}
        };

        for (int i = 0; i < 8; ++i) {
            for (int j = i; j < 8; ++j) {
                if (adjacencyMatrix[i][j] > 0) {
                    addEdge(i, j, adjacencyMatrix[i][j]);
                }
            }
        }
    }



    void printGraph() {
        for (int i = 0; i < adjacencyList.size(); i++) {
            cout << "Vertex " << i+1 << " has edges to:\n";
            for (Edge edge : adjacencyList[i]) {
                cout << "  Vertex " << edge.to+1 << " with weight " << edge.weight << "\n";
            }
        }
    }

    //алгоритм дейкстра
    vector<int> dijkstra(int startVertex, int finishVertex) {
        const int INF = 1000000;
        vector<int> distance(adjacencyList.size(), INF); //кратчайшее расстояние из startVertex до всех остальных вершин
        vector<bool> visited(adjacencyList.size(), false);
        vector<int> parent(adjacencyList.size(), -1); //из какой вершины быстрее всего дойти в каждую

        distance[startVertex] = 0;

        for (int count = 0; count < adjacencyList.size(); count++) {
            int u = minDistance(distance, visited); //u - ближайшая непосещенная точка к startVertex
            visited[u] = true;

            for (Edge neighbor : adjacencyList[u]) { //перебор всех соседей для u
                //новый путь через вершину u короче текущего расстояния до соседней вершины
                if (!visited[neighbor.to] && distance[u] != INF && distance[u] + neighbor.weight < distance[neighbor.to]) {
                    distance[neighbor.to] = distance[u] + neighbor.weight;
                    parent[neighbor.to] = u; // Сохраняем предыдущую вершину для вершины neighbor.to
                }
            }
        }

        int v = finishVertex;
        vector<int> shortestPath;
        while (v != -1) {
            shortestPath.push_back(v);
            v = parent[v];
        }

        reverse(shortestPath.begin(), shortestPath.end());
        if (shortestPath.size() < 2) return vector<int>();
        return shortestPath;
    }

    int minDistance(vector<int> distance, vector<bool> visited) {
        const int INF = 1000000;
        int minDist = INF;
        int minIndex = -1;

        for (int i = 0; i < distance.size(); i++) {
            if (!visited[i] && distance[i] <= minDist) {
                minDist = distance[i];
                minIndex = i;
            }
        }
        return minIndex;
    }

    int calculatePathCost(vector<int> path) {
        int totalCost = 0;
        for (int i = 0; i < path.size() - 1; i++) {
            int u = path[i];
            int v = path[i + 1];
            int weight = getWeight(u, v);
            if (weight == -1) {
                return -1;
            }
            totalCost += weight;
        }
        return totalCost;
    }


    vector<vector<int>> YenKSP(int startVertex, int finishVertex, int K) {
        vector<vector<int>> shortestPaths;
        shortestPaths.push_back(dijkstra(startVertex, finishVertex));
        vector<vector<int>> potentialKSP;
        for (int k = 1; k < K; k++) {
            for (int i = 0; i < shortestPaths[k - 1].size() - 1; i++) {
                int spurNode = shortestPaths[k - 1][i];
                vector<int> rootPath(shortestPaths[k - 1].begin(), shortestPaths[k - 1].begin() + i + 1);
                for (vector<int> p : shortestPaths) {
                    if (rootPath.size() < p.size() && equal(rootPath.begin(), rootPath.end(), p.begin(), p.begin() + rootPath.size())) {
                        // Удаление ребер, принадлежащих предыдущим кратчайшим путям с тем же rootPath
                        for (size_t j = 0; j < rootPath.size(); j++) {
                            int u = p[j];
                            int v = p[j + 1];
                            removeEdge(u, v);
                        }
                        // Удаление узлов rootPathNode из графа, за исключением spurNode
                        for (int node : rootPath) {
                            if (node != spurNode) {
                                // Удаление узла node из графа
                                removeNode(node);
                            }
                        }
                    }
                }

                // Вычисление пути от "шпоры" до стока
                vector<int> spurPath = dijkstra(spurNode, finishVertex);
                resetGraph();
                if (spurPath.size() == 0) continue; //не найден путь
                // Объединение пути rootPath и spurPath
                vector<int> totalPath = rootPath;
                totalPath.insert(totalPath.end(), spurPath.begin()+1, spurPath.end());
                vector<int> path = totalPath;
                // Добавление потенциального K-го кратчайшего пути в B
                if (find(potentialKSP.begin(), potentialKSP.end(), totalPath) == potentialKSP.end()) {
                    potentialKSP.push_back(totalPath);
                }
            }
            if (potentialKSP.empty()) { // Если B пуст, значит больше нет шпорных путей
                break;
            }
            // Сортировка потенциальных K-ых путей по стоимости
            sort(potentialKSP.begin(), potentialKSP.end(), [&](const vector<int>& a, const vector<int>& b) {
                // Сравнение стоимости путей - можно использовать вес ребер
                return calculatePathCost(a) < calculatePathCost(b);
                });

            // Добавление самого дешевого пути из B в A
            shortestPaths.push_back(potentialKSP[0]);
            // Удаление первого пути из B
            potentialKSP.erase(potentialKSP.begin());
        }
        printYenKShortestPaths(shortestPaths);
        return shortestPaths;
    }


    void printYenKShortestPaths(vector<vector<int>> shortestPaths) {
        cout << "Кратчайшие пути:" << endl;
        for (int i = 0; i < shortestPaths.size(); i++) {
            cout << "Путь №" << i + 1 << ": ";
            vector<int> path = shortestPaths[i];
            for (int j = 0; j < path.size(); j++) {
                cout << path[j] + 1;
                if (j < path.size() - 1) cout << " -> ";
            }
            cout << " Стоимость: " << calculatePathCost(path) << endl;
        }
    }
};

int main() {
    setlocale(LC_ALL, "ru");
    int style = 0;
    while (style != 1 && style != 2) {
        cout << "Использовать готовый граф, или создать новый?\n(Готовый)-1\n(Новый)-2\n\n";
        cin >> style;
        cout << endl;
    }
    if (style == 1) {
        Graph graph = Graph();
        graph.YenKSP(0, 7, 20);
    }
    if (style == 2) {
        int vertices, edges;
        cout << "Введите количество вершин: ";
        cin >> vertices;
        while (vertices < 1) {
            cout << "Число вершин не может быть меньше 1";
            cin >> vertices;
        }
        Graph graph = Graph(vertices);
        cout << "Введите количество ребер: ";
        cin >> edges;
        cout << "Введите ребра (from to weight):" << endl;

        for (int i = 0; i < edges; i++) {
            int from, to, weight;
            cin >> from >> to >> weight;
            graph.addEdge(from-1, to-1, weight);
        }
        graph.printGraph();
        graph.dijkstra(1, 3);
    }
    
}