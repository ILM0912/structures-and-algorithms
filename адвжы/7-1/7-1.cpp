#include <iostream>
#include <queue>

using namespace std;

enum Color { RED, BLACK };

struct Node {
    int value;
    Color color;
    Node* left;
    Node* right;
    Node* parent;
    Node(int value) {
        this->value = value;
        this->color = RED;
        this->left = nullptr;
        this->right = nullptr;
        this->parent = nullptr;
    }
};

class RedBlackTree {
private:
    Node* root;
    void printHelper(Node* root, string indent, bool last) {
        if (root != nullptr) {
            cout << indent;
            if (last) {
                cout << "R----";
                indent += "   ";
            }
            else {
                cout << "L----";
                indent += "|  ";
            }
            cout << root->value << "(" << ((root->color == RED) ? "RED" : "BLACK") << ")" << endl;
            printHelper(root->left, indent, false);
            printHelper(root->right, indent, true);
        }
    }

    void fixInsert(Node*& node) {
        Node* parent = nullptr;
        Node* grandparent = nullptr;
        while (node != root && node->color == RED && node->parent->color == RED) {
            parent = node->parent; //отец 
            grandparent = parent->parent; //дед
            if (parent == grandparent->left) { //отец - левый, дядя - правый
                Node* uncle = grandparent->right; //дядя
                if (uncle != nullptr && uncle->color == RED) { //красный дядя
                    grandparent->color = RED; //перекрашивание отца и дяди в черный, дед - красный
                    parent->color = BLACK;
                    uncle->color = BLACK;
                    node = grandparent; //балансировка для деда
                }
                else {
                    if (node == parent->right) { //отец - левый, ребенок - правый
                        rotateLeft(parent); //теперь - ребенок(раньше отец)-левый, отец(раньше ребенок)-левый -> с одной стороны
                        node = parent;
                        parent = node->parent; // для реализации большого поворота
                    }
                    rotateRight(grandparent); // поворот для отца (или большой поворот)
                    swap(parent->color, grandparent->color); //дед стал красным, отец - черным
                    node = parent;
                }
            }
            else { //тоже самое, но отец - правый, дядя - левый
                Node* uncle = grandparent->left;
                if (uncle != nullptr && uncle->color == RED) { //красный дядя
                    grandparent->color = RED; //перекрашивание отца и дяди в черный, дед - красный
                    parent->color = BLACK;
                    uncle->color = BLACK;
                    node = grandparent; //балансировка для деда
                }
                else {
                    if (node == parent->left) { //отец - правый, ребенок - левый
                        rotateRight(parent); //теперь - ребенок(раньше отец)-правый, отец(раньше ребенок)-правый -> с одной стороны
                        node = parent;
                        parent = node->parent; // для реализации большого поворота
                    }
                    rotateLeft(grandparent); // поворот для отца (или большой поворот)
                    swap(parent->color, grandparent->color); //дед стал красным, отец - черным
                    node = parent;
                }
            }
        }
        root->color = BLACK;
    }

    void rotateLeft(Node* node) {
        Node* child = node->right; //child - правый потомок node
        node->right = child->left; //замена child у node на левого ребенка child
        if (node->right != nullptr)
            node->right->parent = node; //установка для нового ребенка node значения parent = node
        child->parent = node->parent; //родитель child теперь не node, а родитель node
        if (node->parent == nullptr)
            root = child; //если node было корнем, то теперь child - корень
        else if (node == node->parent->left) 
            node->parent->left = child; //установка для родителя node нового сына вместо node - левого или правого
        else node->parent->right = child;
        child->left = node;
        node->parent = child; //установка связи сын-родитель между элементами node-child
    }

    void rotateRight(Node* node) {
        Node* child = node->left; //child - левый потомок node
        node->left = child->right; //замена child у node на правого ребенка child
        if (node->left != nullptr)
            node->left->parent = node; //установка для нового ребенка node значения parent = node
        child->parent = node->parent; //родитель child теперь не node, а родитель node
        if (node->parent == nullptr)
            root = child; //если node было корнем, то теперь child - корень
        else if (node == node->parent->left) 
            node->parent->left = child; //установка для родителя node нового сына вместо node - левого или правого
        else
            node->parent->right = child;
        child->right = node;
        node->parent = child; //установка связи сын-родитель между элементами node-child
    }

public:
    RedBlackTree() {
        this->root = nullptr;
    }

    Node* getRoot() {
        return this->root;
    }

    void insert(int value) {
        Node* node = new Node(value);
        Node* parent = nullptr;
        Node* current = root;
        while (current != nullptr) { //поиск вглубь дерева для нахождения свободного подходящего места
            parent = current;
            if (node->value < current->value) {
                current = current->left;
            }
            else if (node->value > current->value) {
                current = current->right;
            }
            else {
                delete node;
                return;
            }
        }
        node->parent = parent;
        if (parent == nullptr)
            root = node;
        else if (node->value < parent->value)
            parent->left = node;
        else
            parent->right = node;
        fixInsert(node); //балансировка
    }

    void inOrder(Node* node) {
        if (node == nullptr) {
            return;
        }
        inOrder(node->left);
        cout << node->value << " ";
        inOrder(node->right);
    }

    void breadthFirstSearch(Node* root) {
        if (root == nullptr) {
            return; // дерево пустое
        }
        queue<Node*> q; 
        q.push(root); //добавление в очередь корня
        while (!q.empty()) { //пока очередь не закончится
            Node* current = q.front(); //считывание первого элемента очереди
            q.pop(); //его удаление
            cout << current->value << " ";
            if (current->left != nullptr) { 
                q.push(current->left); //добавление (если он есть) левого сына текущего элемента
            }
            if (current->right != nullptr) {
                q.push(current->right); //и правого
            }
        }
    }

    int sumOfLeaves(Node* node) {
        if (node == nullptr) {
            return 0;
        }
        if (node->left == nullptr && node->right == nullptr) {
            return node->value;
        }
        int sumLeft = sumOfLeaves(node->left);
        int sumRight = sumOfLeaves(node->right);
        return sumLeft + sumRight;
    }

    int sumNodes(Node* root) {
        if (root == nullptr) {
            return 0;
        }
        return root->value + sumNodes(root->left) + sumNodes(root->right);
    }

    int countNodes(Node* root) {
        if (root == nullptr) {
            return 0;
        }
        return 1 + countNodes(root->left) + countNodes(root->right);
    }

    double averageOfTree(Node* root) {
        int sum = sumNodes(root);
        int count = countNodes(root);
        if (count == 0) {
            throw logic_error("!!!Zero division because tree is empty!!!");
        }
        return static_cast<double>(sum) / count;
    }


    void printTree()
    {
        if (root == nullptr)
            cout << "Tree is empty." << endl;
        else {
            cout << "Red-Black Tree:" << endl;
            printHelper(root, "", true);
        }
    }
};

int main() {
    RedBlackTree Tree = RedBlackTree();
    for (int i = 1; i <= 10; i++) {
        //Tree.insert(0);
    }
    Tree.printTree();
    try {
        cout << Tree.averageOfTree(Tree.getRoot());
    }
    catch (const logic_error e) {
        cerr << e.what();
    }
}
