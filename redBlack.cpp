// Tessa Habet, Boston Beatty, Grant Bytnar
// CSC 344 
// Exercise Set 4 Modules 5 and 6 
#include <iostream>
#include <vector>
#include <string>
#include <queue> 
#include <random>
#include <chrono>
using namespace std;

struct Node {
    int data;
    string color;
    string firstName;
    string lastName;
    string jobTitle;
    Node* left;
    Node* right;
    Node* parent;

    Node(int myData, string fName, string lName, string jTitle, Node* nodeParent)
        : data(myData), firstName(fName), lastName(lName), jobTitle(jTitle),
          parent(nodeParent), left(nullptr), right(nullptr), color("RED") {}
};

class RedBlackTree {
public:
    Node* root;
    Node* NIL;

    RedBlackTree() {
        NIL = new Node(0, "", "", "", nullptr); // Create NIL node
        NIL->color = "BLACK"; // NIL node should always be black
        root = NIL; // Initially, the tree is empty
    }

    // Left Rotate function
    void leftRotate(Node* x) {
        Node* y = x->right;
        x->right = y->left;
        if (y->left != NIL) {
            y->left->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == NIL) {
            root = y; // Set new root
        }
        else if (x == x->parent->left) {
            x->parent->left = y;
        }
        else {
            x->parent->right = y;
        }
        y->left = x;
        x->parent = y;
    }

    // Function to perform right rotate
    void rightRotate(Node* x) {
        Node* y = x->left;
        x->left = y->right;
        if (y->right != NIL) {
            y->right->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == NIL) {
            root = y; // Set new root
        }
        else if (x == x->parent->right) {
            x->parent->right = y;
        }
        else {
            x->parent->left = y;
        }
        y->right = x;
        x->parent = y;
    }

    void insert(Node* z) {
        Node* y = NIL; // Initially, the parent is NIL
        Node* x = root; // Start at the root

        while (x != NIL) { // Traverse the tree
            y = x; // Set the parent
            if (z->data < x->data) {
                x = x->left;
            }
            else {
                x = x->right;
            }
        }
        z->parent = y; // Assign parent to z
        if (y == NIL) {
            root = z; // Tree was empty
        }
        else if (z->data < y->data) {
            y->left = z; // Insert as left child
        }
        else {
            y->right = z; // Insert as right child
        }
        z->left = NIL;
        z->right = NIL;
        z->color = "RED"; // New node must be red
        insertFixup(z); // Fix any violations
    }

    void insertFixup(Node* k) {
        while (k->parent->color == "RED") {
            if (k->parent == k->parent->parent->left) {
                Node* u = k->parent->parent->right; // Uncle
                if (u->color == "RED") {
                    k->parent->color = "BLACK";
                    u->color = "BLACK";
                    k->parent->parent->color = "RED";
                    k = k->parent->parent;
                }
                else {
                    if (k == k->parent->right) {
                        k = k->parent;
                        leftRotate(k);
                    }
                    k->parent->color = "BLACK";
                    k->parent->parent->color = "RED";
                    rightRotate(k->parent->parent);
                }
            }
            else {
                Node* u = k->parent->parent->left; // Uncle
                if (u->color == "RED") {
                    k->parent->color = "BLACK";
                    u->color = "BLACK";
                    k->parent->parent->color = "RED";
                    k = k->parent->parent;
                }
                else {
                    if (k == k->parent->left) {
                        k = k->parent;
                        rightRotate(k);
                    }
                    k->parent->color = "BLACK";
                    k->parent->parent->color = "RED";
                    leftRotate(k->parent->parent);
                }
            }
        }
        root->color = "BLACK"; // Ensure root is black
    }

    // Find node that does not have a left child
    Node* successor(Node* x) {
        Node* y = NIL;
        if (x->right != NIL) {
            return minimum(x->right); // Leftmost node in right subtree
        }
        else {
            y = x->parent;
            while (y != NIL && x == y->right) {
                x = y;
                y = y->parent;
            }
            return y;
        }
    }

    Node* minimum(Node* node) {
        while (node->left != NIL) {
            node = node->left;
        }
        return node;
    }

    void rbTransplant(Node* u, Node* v) {
        if (u->parent == NIL) {
            root = v;
        }
        else if (u == u->parent->left) {
            u->parent->left = v;
        }
        else {
            u->parent->right = v;
        }
        v->parent = u->parent;
    }

    void deleteNode(Node* z) {
        Node* y = z;
        Node* x;
        string originalColor = y->color;

        if (z->left == NIL) {
            x = z->right;
            rbTransplant(z, z->right);
        }
        else if (z->right == NIL) {
            x = z->left;
            rbTransplant(z, z->left);
        }
        else {
            y = successor(z);
            originalColor = y->color;
            x = y->right;
            if (y->parent == z) {
                x->parent = y;
            }
            else {
                rbTransplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }
            rbTransplant(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }
        delete z;
        if (originalColor == "BLACK") {
            deleteFix(x);
        }
    }

    void deleteFix(Node* x) {
        while (x != root && x->color == "BLACK") {
            if (x == x->parent->left) {
                Node* s = x->parent->right; // Sibling
                if (s->color == "RED") {
                    s->color = "BLACK";
                    x->parent->color = "RED";
                    leftRotate(x->parent);
                    s = x->parent->right;
                }
                if (s->left->color == "BLACK" && s->right->color == "BLACK") {
                    s->color = "RED";
                    x = x->parent;
                }
                else {
                    if (s->right->color == "BLACK") {
                        s->left->color = "BLACK";
                        s->color = "RED";
                        rightRotate(s);
                        s = x->parent->right;
                    }
                    s->color = x->parent->color;
                    x->parent->color = "BLACK";
                    s->right->color = "BLACK";
                    leftRotate(x->parent);
                    x = root;
                }
            }
            else {
                Node* s = x->parent->left; // Sibling
                if (s->color == "RED") {
                    s->color = "BLACK";
                    x->parent->color = "RED";
                    rightRotate(x->parent);
                    s = x->parent->left;
                }
                if (s->right->color == "BLACK" && s->left->color == "BLACK") {
                    s->color = "RED";
                    x = x->parent;
                }
                else {
                    if (s->left->color == "BLACK") {
                        s->right->color = "BLACK";
                        s->color = "RED";
                        leftRotate(s);
                        s = x->parent->left;
                    }
                    s->color = x->parent->color;
                    x->parent->color = "BLACK";
                    s->left->color = "BLACK";
                    rightRotate(x->parent);
                    x = root;
                }
            }
        }
        x->color = "BLACK"; // Ensure that x is black
    }

    Node* search(Node* node, int data) {
        if (node == NIL || data == node->data) {
            return node;
        }
        if (data < node->data) {
            return search(node->left, data);
        }
        return search(node->right, data);
    }

    void addEmployee(int data, string fName, string lName, string jTitle) {
        Node* newNode = new Node(data, fName, lName, jTitle, NIL);
        insert(newNode); 
    }

    void deleteEmployee(int data) {
        Node* nodeToDelete = search(root, data);
        if (nodeToDelete != NIL) {
            deleteNode(nodeToDelete);
        } else {
            cout << "Employee with data " << data << " not found." << endl;
        }
    }

    
    void inorder(Node* node) {
        if (node != NIL) {
            inorder(node->left);
            cout << node->data << " (" << node->color << ") ";
            inorder(node->right);
        }
    }

    void printTree() {
        inorder(root);
        cout << endl;
    }

    ~RedBlackTree() {
        cleanup(root);
        delete NIL;
    }

    void cleanup(Node* node) {
        if (node != NIL) {
            cleanup(node->left);
            cleanup(node->right);
            delete node; 
        }
    }
};

int main() {
    RedBlackTree tree;

    tree.addEmployee(10, "Alice", "Johnson", "Engineer");
    tree.addEmployee(20, "Bob", "Smith", "Manager");
    tree.addEmployee(30, "Charlie", "Brown", "Director");

    cout << "Inorder traversal of the tree: ";
    tree.printTree();

    tree.deleteEmployee(20);
    cout << "Inorder traversal after deleting 20: ";
    tree.printTree();

    return 0;
}
