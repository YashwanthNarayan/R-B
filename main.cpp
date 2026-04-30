#include <iostream>
#include <fstream>
#include <cstring>

// Color enum for Red-Black tree nodes
enum Color { RED, BLACK };

// Node structure for Red-Black Tree
struct Node {
    int value;
    Color color;
    Node* parent;
    Node* left;
    Node* right;

    // Constructor
    Node(int val) : value(val), color(RED), parent(nullptr), left(nullptr), right(nullptr) {}
};

// Red-Black Tree class
class RedBlackTree {
private:
    Node* root;
    Node* Tnil;  // Sentinel node for NULL leaves

    // Helper functions
    void leftRotate(Node* x);
    void rightRotate(Node* x);
    void insertFixup(Node* z);
    void inOrderTraversal(Node* node);
    void printTree(Node* node, int indent);

public:
    RedBlackTree();
    void insert(int value);
    void readFromFile(const char* filename);
    void print();
};

// Constructor
RedBlackTree::RedBlackTree() {
    // Create sentinel Tnil node (black leaf)
    Tnil = new Node(0);
    Tnil->color = BLACK;
    Tnil->left = Tnil->right = Tnil->parent = nullptr;
    root = Tnil;
}

// Left rotation
void RedBlackTree::leftRotate(Node* x) {
    Node* y = x->right;
    x->right = y->left;
    
    if (y->left != Tnil) {
        y->left->parent = x;
    }
    
    y->parent = x->parent;
    
    if (x->parent == Tnil) {
        root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    
    y->left = x;
    x->parent = y;
}

// Right rotation
void RedBlackTree::rightRotate(Node* x) {
    Node* y = x->left;
    x->left = y->right;
    
    if (y->right != Tnil) {
        y->right->parent = x;
    }
    
    y->parent = x->parent;
    
    if (x->parent == Tnil) {
        root = y;
    } else if (x == x->parent->right) {
        x->parent->right = y;
    } else {
        x->parent->left = y;
    }
    
    y->right = x;
    x->parent = y;
}

// Insert a value into the tree
void RedBlackTree::insert(int value) {
    Node* z = new Node(value);
    z->left = Tnil;
    z->right = Tnil;
    
    Node* y = Tnil;
    Node* x = root;
    
    // Standard BST insertion
    while (x != Tnil) {
        y = x;
        if (z->value < x->value) {
            x = x->left;
        } else {
            x = x->right;
        }
    }
    
    z->parent = y;
    
    if (y == Tnil) {
        root = z;
    } else if (z->value < y->value) {
        y->left = z;
    } else {
        y->right = z;
    }
    
    // Fix Red-Black tree properties
    insertFixup(z);
}

// Insert fixup algorithm
void RedBlackTree::insertFixup(Node* z) {
    while (z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            // Case: Parent is left child of grandparent
            Node* uncle = z->parent->parent->right;
            
            if (uncle->color == RED) {
                // Case 1: Uncle is red
                z->parent->color = BLACK;
                uncle->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                // Case 2 & 3: Uncle is black
                if (z == z->parent->right) {
                    // LR case
                    z = z->parent;
                    leftRotate(z);
                }
                // LL case
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rightRotate(z->parent->parent);
            }
        } else {
            // Case: Parent is right child of grandparent
            Node* uncle = z->parent->parent->left;
            
            if (uncle->color == RED) {
                // Case 1: Uncle is red
                z->parent->color = BLACK;
                uncle->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                // Case 2 & 3: Uncle is black
                if (z == z->parent->left) {
                    // RL case
                    z = z->parent;
                    rightRotate(z);
                }
                // RR case
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                leftRotate(z->parent->parent);
            }
        }
    }
    
    // Ensure root is always black
    root->color = BLACK;
}

// Read from file
void RedBlackTree::readFromFile(const char* filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Error: Could not open file " << filename << std::endl;
        return;
    }
    
    char line[1024];
    while (file.getline(line, 1024)) {
        char* token = strtok(line, " ");
        while (token != nullptr) {
            int num = atoi(token);
            insert(num);
            token = strtok(nullptr, " ");
        }
    }
    
    file.close();
}

// Print tree (in-order)
void RedBlackTree::print() {
    inOrderTraversal(root);
    std::cout << std::endl;
}

void RedBlackTree::inOrderTraversal(Node* node) {
    if (node == Tnil) return;
    
    inOrderTraversal(node->left);
    std::cout << node->value << "(" << (node->color == RED ? "R" : "B") << ") ";
    inOrderTraversal(node->right);
}

// Main function
int main() {
    RedBlackTree tree;
    
    // Add some numbers
    tree.insert(10);
    tree.insert(20);
    tree.insert(30);
    tree.insert(15);
    tree.insert(25);
    
    std::cout << "In-order traversal: ";
    tree.print();
    
    return 0;
}
