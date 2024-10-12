#include <iostream>
#include <vector>

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
    vector<vector<int>> adjacencyMatrix;
public:
    Graph(int vertices) {
        adjacencyList.resize(vertices);
        adjacencyMatrix.resize(vertices, vector<int>(vertices, 0));
    }

    Graph() {
        adjacencyList.resize(8);
        this->adjacencyMatrix = {
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

    void saveEdgeForReset(int from, int to, int weight) {
        adjacencyMatrix[from][to] = weight;
        adjacencyMatrix[to][from] = weight;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                cout << adjacencyMatrix[i][j];
            }
            cout << endl;
        }
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
        for (int i = 0; i < adjacencyList.size(); ++i) {
            adjacencyList[i].clear();
        }
        for (int i = 0; i < this->size(); ++i) {
            for (int j = i; j < this->size(); ++j) {
                if (adjacencyMatrix[i][j] > 0) {
                    addEdge(i, j, adjacencyMatrix[i][j]);
                }
                cout << i;
            }
        }
    }

    int size() {
        return adjacencyList.size();
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
                    parent[neighbor.to] = u; //сохраняем предыдущую вершину для вершины neighbor.to
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
        if (shortestPath.size() < 2) return {};
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
        if (shortestPaths[0].size() == 0) {
            cout << "Нет пути между точками " << startVertex+1 << " - " << finishVertex+1 << endl;
            return {};
        }
        vector<vector<int>> potentialKSP;
        for (int k = 1; k < K; k++) {
            for (int i = 0; i < shortestPaths[k - 1].size() - 1; i++) {
                int spurNode = shortestPaths[k - 1][i];
                vector<int> rootPath(shortestPaths[k - 1].begin(), shortestPaths[k - 1].begin() + i + 1);
                for (vector<int> p : shortestPaths) {
                    if (rootPath.size() < p.size() && equal(rootPath.begin(), rootPath.end(), p.begin(), p.begin() + rootPath.size())) {
                        for (size_t j = 0; j < rootPath.size(); j++) {
                            int u = p[j];
                            int v = p[j + 1];
                            removeEdge(u, v);
                        }
                        for (int node : rootPath) {
                            if (node != spurNode) {
                                removeNode(node);
                            }
                        }
                    }
                }
                //путь ветвления
                vector<int> spurPath = dijkstra(spurNode, finishVertex);
                resetGraph();
                if (spurPath.size() == 0) continue; //не найден путь
                //totalPath - найденный путь
                vector<int> totalPath = rootPath;
                totalPath.insert(totalPath.end(), spurPath.begin()+1, spurPath.end());
                if (find(potentialKSP.begin(), potentialKSP.end(), totalPath) == potentialKSP.end()) {
                    potentialKSP.push_back(totalPath);
                }
            }
            if (potentialKSP.empty()) {
                break;
            }
            //сортировка потенциальных K-ых путей по длине пути
            sort(potentialKSP.begin(), potentialKSP.end(), [&](const vector<int>& a, const vector<int>& b) {
                return calculatePathCost(a) < calculatePathCost(b);
                });

            shortestPaths.push_back(potentialKSP[0]);
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
    Graph graph;
    while (style != 1 && style != 2) {
        cout << "Использовать готовый граф, или создать новый?\n(Готовый)-1\n(Новый)-2\n\n";
        cin >> style;
        cout << endl;
    }
    if (style == 1) {
        graph = Graph();
    }
    if (style == 2) {
        int vertices, edges;
        cout << "Введите количество вершин: ";
        cin >> vertices;
        while (vertices < 1) {
            cout << "Число вершин не может быть меньше 1";
            cin >> vertices;
        }
        graph = Graph(vertices);
        cout << "Введите количество ребер: ";
        cin >> edges;
        cout << "Введите ребра (from to weight):" << endl;

        for (int i = 0; i < edges; i++) {
            int from, to, weight;
            cin >> from >> to >> weight;
            graph.saveEdgeForReset(from - 1, to - 1, weight);
            graph.addEdge(from-1, to-1, weight);
        }
    }
    
    int start = 0;
    while (!(start >= 1 && start <= graph.size())) {
        cout << "Введите стартовуй точку (1 <= start <= " << graph.size() << "): ";
        cin >> start;
    }

    int finish = 0;
    while (!(finish >= 1 && finish <= graph.size() && finish!=start)) {
        cout << "Введите конечную точку (1 <= finish <= " << graph.size() << "): ";
        cin >> finish;
    }

    int k = 0;
    while (k < 1) {
        cout << "Введите требуемое количество путей (k>1): ";
        cin >> k;
    }
    graph.YenKSP(start - 1, finish - 1, k);
}