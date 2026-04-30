#include <iostream>
#include <fstream>
#include <cstring>

enum Color { RED, BLACK };

struct Node {
    int value;
    Color color;
    Node* parent;
    Node* left;
    Node* right;

    // Constructor
    Node(int val) : value(val), color(RED), parent(nullptr), left(nullptr), right(nullptr) {}
};

class RedBlackTree {
private:
    Node* root;
    Node* Tnil;  

    // Helper functions
    void leftRotate(Node* x);
    void rightRotate(Node* x);
    void insertFixup(Node* z);
    void printDetailed(Node* node);
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

// This is the left rotation
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

// This is the right rotation
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

// This is to insert a value into the tree
void RedBlackTree::insert(int value) {
    Node* z = new Node(value);
    z->left = Tnil;
    z->right = Tnil;
    
    Node* y = Tnil;
    Node* x = root;
    
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
    
    // This is to fix Red-Black tree properties
    insertFixup(z);
}

// This is the insert fixup algorithm
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
            // This is for the case where the parent is right child of grandparent
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
    
    // This is to ensure root is always black
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

// Print tree with value, color, and parent
void RedBlackTree::print() {
    std::cout << "=== Red-Black Tree ===" << std::endl;
    std::cout << "Format: Value(Color) <- ParentValue" << std::endl;
   
    printDetailed(root);
    std::cout << std::endl;
    
    // Also print visual tree
    std::cout << "\n=== Visual Tree ===" << std::endl;
    printTree(root, 0);
}

void RedBlackTree::printDetailed(Node* node) {
    if (node == Tnil) return;
    
    printDetailed(node->left);
    
    // Print value, color, and parent
    std::cout << node->value << "(" << (node->color == RED ? "R" : "B") << ")";
    
    // Print parent
    if (node->parent != Tnil) {
        std::cout << " <- " << node->parent->value;
    } else {
        std::cout << " <- (null)";
    }
    std::cout << std::endl;
    
    printDetailed(node->right);
}

// Visual tree representation
void RedBlackTree::printTree(Node* node, int indent) {
    if (node == Tnil) return;
    
    printTree(node->right, indent + 4);
    
    for (int i = 0; i < indent; i++) {
        std::cout << " ";
    }
    
    std::cout << node->value << "(" << (node->color == RED ? "R" : "B") << ")" << std::endl;
    
    printTree(node->left, indent + 4);
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
