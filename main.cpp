//Yashwanth Narayan Shravanaboyina Besta
//4/1/2026
//This is a C++ implementation of a Red-Black Tree with a menu-driven interface. The program allows users to insert numbers, read from a file, print the tree, search for values, and show tree statistics. The tree maintains the properties of a Red-Black Tree to ensure balanced operations.


#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>

using namespace std;

enum Color { RED, BLACK };

struct Node {
    int value;
    Color color;
    Node* parent;
    Node* left;
    Node* right;

    Node(int val) : value(val), color(RED), parent(nullptr), left(nullptr), right(nullptr) {}
};

class RedBlackTree {
private:
    Node* root;
    Node* Tnil;  

    void leftRotate(Node* x);
    void rightRotate(Node* x);
    void insertFixup(Node* z);
    void transplant(Node* u, Node* v);
    Node* minimum(Node* node);
    Node* searchNode(Node* node, int value);
    void deleteFixup(Node* x);
    void printDetailed(Node* node);
    void printTree(Node* node, int indent);
    bool search(Node* node, int value);
    int countNodes(Node* node);
    int getHeight(Node* node);

public:
    RedBlackTree();
    void insert(int value);
    bool remove(int value);
    void readFromFile(const char* filename);
    void print();
    bool find(int value);
    void showStats();
};

// Constructor
RedBlackTree::RedBlackTree() {
    // Create sentinel Tnil node (black leaf)
    Tnil = new Node(0);
    Tnil->color = BLACK;
    Tnil->left = Tnil;
    Tnil->right = Tnil;
    Tnil->parent = Tnil;
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

// Replace one subtree with another
void RedBlackTree::transplant(Node* u, Node* v) {
    if (u->parent == Tnil) {
        root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }

    v->parent = u->parent;
}

// Find the smallest node in a subtree
Node* RedBlackTree::minimum(Node* node) {
    while (node->left != Tnil) {
        node = node->left;
    }

    return node;
}

// Return the node that stores value, or Tnil if it is not in the tree
Node* RedBlackTree::searchNode(Node* node, int value) {
    while (node != Tnil && value != node->value) {
        if (value < node->value) {
            node = node->left;
        } else {
            node = node->right;
        }
    }

    return node;
}

// Restore Red-Black tree properties after deleting a black node
void RedBlackTree::deleteFixup(Node* x) {
    while (x != root && x->color == BLACK) {
        if (x == x->parent->left) {
            Node* sibling = x->parent->right;

            if (sibling->color == RED) {
                sibling->color = BLACK;
                x->parent->color = RED;
                leftRotate(x->parent);
                sibling = x->parent->right;
            }

            if (sibling->left->color == BLACK && sibling->right->color == BLACK) {
                sibling->color = RED;
                x = x->parent;
            } else {
                if (sibling->right->color == BLACK) {
                    sibling->left->color = BLACK;
                    sibling->color = RED;
                    rightRotate(sibling);
                    sibling = x->parent->right;
                }

                sibling->color = x->parent->color;
                x->parent->color = BLACK;
                sibling->right->color = BLACK;
                leftRotate(x->parent);
                x = root;
            }
        } else {
            Node* sibling = x->parent->left;

            if (sibling->color == RED) {
                sibling->color = BLACK;
                x->parent->color = RED;
                rightRotate(x->parent);
                sibling = x->parent->left;
            }

            if (sibling->right->color == BLACK && sibling->left->color == BLACK) {
                sibling->color = RED;
                x = x->parent;
            } else {
                if (sibling->left->color == BLACK) {
                    sibling->right->color = BLACK;
                    sibling->color = RED;
                    leftRotate(sibling);
                    sibling = x->parent->left;
                }

                sibling->color = x->parent->color;
                x->parent->color = BLACK;
                sibling->left->color = BLACK;
                rightRotate(x->parent);
                x = root;
            }
        }
    }

    x->color = BLACK;
}

// Remove a value from the tree
bool RedBlackTree::remove(int value) {
    Node* z = searchNode(root, value);

    if (z == Tnil) {
        return false;
    }

    Node* y = z;
    Node* x;
    Color yOriginalColor = y->color;

    if (z->left == Tnil) {
        x = z->right;
        transplant(z, z->right);
    } else if (z->right == Tnil) {
        x = z->left;
        transplant(z, z->left);
    } else {
        y = minimum(z->right);
        yOriginalColor = y->color;
        x = y->right;

        if (y->parent == z) {
            x->parent = y;
        } else {
            transplant(y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }

        transplant(z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }

    delete z;

    if (yOriginalColor == BLACK) {
        deleteFixup(x);
    }

    return true;
}

// Read from file
void RedBlackTree::readFromFile(const char* filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Could not open file " << filename << endl;
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
    cout << "Red-Black Tree " << endl;
    cout << "Format: Value(Color) <- ParentValue" << endl;
   
    printDetailed(root);
    cout << endl;
    
    // Also print visual tree
    cout << "Visual Tree" << endl;
    printTree(root, 0);
}

void RedBlackTree::printDetailed(Node* node) {
    if (node == Tnil) return;
    
    printDetailed(node->left);
    
    // Print value, color, and parent
    cout << node->value << "(" << (node->color == RED ? "R" : "B") << ")";
    
    // Print parent
    if (node->parent != Tnil) {
        cout << " <- " << node->parent->value;
    } else {
        cout << " <- (null)";
    }
    cout << endl;
    
    printDetailed(node->right);
}

// Visual tree representation
void RedBlackTree::printTree(Node* node, int indent) {
    if (node == Tnil) return;
    
    printTree(node->right, indent + 4);
    
    for (int i = 0; i < indent; i++) {
        cout << " ";
    }
    
    cout << node->value << "(" << (node->color == RED ? "R" : "B") << ")" << endl;
    
    printTree(node->left, indent + 4);
}

// Search function to find a value in tree
bool RedBlackTree::search(Node* node, int value) {
    if (node == Tnil) return false;
    
    if (value == node->value) {
        return true;
    } else if (value < node->value) {
        return search(node->left, value);
    } else {
        return search(node->right, value);
    }
}

// Count all nodes in the tree
int RedBlackTree::countNodes(Node* node) {
    if (node == Tnil) return 0;
    
    return 1 + countNodes(node->left) + countNodes(node->right);
}

// Get height of the tree
int RedBlackTree::getHeight(Node* node) {
    if (node == Tnil) return 0;
    
    int leftHeight = getHeight(node->left);
    int rightHeight = getHeight(node->right);
    
    return 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
}

// Find a value in the tree
bool RedBlackTree::find(int value) {
    return search(root, value);
}

// Show tree statistics
void RedBlackTree::showStats() {
    int nodeCount = countNodes(root);
    int height = getHeight(root);
    
    cout << "Tree Statistics" << endl;
    cout << "Total nodes: " << nodeCount << endl;
    cout << "Tree height: " << height << endl;
}

// This is the main function with menu
int main() {
    RedBlackTree tree;
    int choice;
    int num;
    char filename[100];
    
    do {
        cout << "Red-Black Tree Menu" << endl;
        cout << "1. Add a number" << endl;
        cout << "2. Read from file" << endl;
        cout << "3. Print tree" << endl;
        cout << "4. Search for a number" << endl;
        cout << "5. Remove a number" << endl;
        cout << "6. Show tree statistics" << endl;
        cout << "7. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;
        
        switch (choice) {
            case 1:
                cout << "Enter number to add (1-999): ";
                cin >> num;
                if (num >= 1 && num <= 999) {
                    tree.insert(num);
                    cout << "Added " << num << " to tree." << endl;
                } else {
                    cout << "Invalid number. Use 1-999." << endl;
                }
                break;
            case 2:
                cout << "Enter filename: ";
                cin >> filename;
                tree.readFromFile(filename);
                cout << "File read complete." << endl;
                break;
            case 3:
                tree.print();
                break;
            case 4:
                cout << "Enter number to search (1-999): ";
                cin >> num;
                if (tree.find(num)) {
                    cout << num << " found in tree!" << endl;
                } else {
                    cout << num << " not found in tree." << endl;
                }
                break;
            case 5:
                cout << "Enter number to remove (1-999): ";
                cin >> num;
                if (tree.remove(num)) {
                    cout << "Removed " << num << " from tree." << endl;
                } else {
                    cout << num << " not found in tree." << endl;
                }
                break;
            case 6:
                tree.showStats();
                break;
            case 7:
                cout << "Goodbye!" << endl;
                break;
            default:
                cout << "Invalid choice." << endl;
        }
    } while (choice != 7);
    
    return 0;
}
