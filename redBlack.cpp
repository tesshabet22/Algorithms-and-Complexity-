// Tessa Habet & Boston Beatty 
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
public: 
	int data;
	string color;
	string firstName; 
	string lastName; 
	string jobTitle;
	Node* left, * right, * parent;

	Node(int myData, string fName, string lName, string jTitle, Node* nodeParent, Node* leftKid, Node* rightKid) {
		data = myData;
		firstName = fName;
		lastName = lName;
		jobTitle = jTitle;
		parent = nodeParent;
		left = leftKid;
		right = rightKid;
		color = "RED"; 
	}

	bool isLeaf() {
		if (left == nullptr && right == nullptr) {
			return true;
		}
		else {
			return false;
		}
	}

	bool isRoot() {
		if (parent == nullptr) {
			return true;
		}
		else {
			return false;
		}
	}

	Node* getLeft() {
		return left;
	}

	Node* getRight() {
		return right;
	}

	Node* getParent() {
		return parent;
	}

	int getData() {
		return data;
	}

	// standard setters 
	void setLeft(Node* newLeft) {
		left = newLeft;
	}

	void setRight(Node* newRight) {
		right = newRight;
	}

	void setParent(Node* newParent) {
		parent = newParent;
	}

	void setData(int newData) {
		data = newData;
	}
};



// Red-Black Tree class 
class RedBlackTree {
public:
	Node* root;
	Node* NIL;

	RedBlackTree(Node* firstNode) {
		root = firstNode;
		NIL = new Node(0,"","","",nullptr, nullptr, nullptr);
	}

	// getters 
	Node* getRootNode() {
		return root;
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
			root = y;
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
			root = y;
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

	void Insert(RedBlackTree* myTree, Node* z) {
		Node* x = root; // node being compared with z 
		Node* y = nullptr; // y will be parent of z 
		while (x != nullptr) { // descend until reaching the sentinel 
			y = x;
			if (z->data < x->data) {
				x = x->left;
			}
			else {
				x = x->right;
			}
		}
		z->parent = y; // found the location - insert z with parent y 
		if (y == NIL) {
			root = z; // tree T was empty 
		}
		else if (z->data < y->data) {
			y->left = z; 
		}
		else {
			y->right = z; 
		}
		z->left = NIL;
		z->right = NIL;
		z->color = "RED";
		insertFixup(myTree, z); 
	}

	// Function to re balance red black tree after insertion
	void insertFixup(RedBlackTree* myTree, Node* k) {
		Node* u = NIL; 
		while (k->parent->color == "RED") {
			if (k->parent == k->parent->parent->left) {
				u = k->parent->parent->right; // uncle 
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
				u = k->parent->parent->left; // uncle 
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
		root->color = "BLACK"; // root must be black 
	}

	// find node that does not have a left child
	// in the subtree of a given node 
	Node* successor(Node* x) {
		Node* y = NIL; 
		if (x->right != NIL) {
			return minimum(x->right); // leftmost node in right subtree
		}
		else // find the lowest ancestor of x whose left child is an ancestor of x 
		{
			y = x->parent; 
			while (y != NIL and x == y->right) {
				x = y; 
				y = y->parent; 
			}
			return y; 
		}
	}

	// For balancing the tree after deletion 
	void deleteFix(Node* x) {
		Node* s = nullptr;
		while (x != root && x->color == "RED") {
			if (x == x->parent->right) {
				s = x->parent->right;
				if (s->color == "BLACK") {
					s->color = "RED";
					x->parent->color = "BLACK";
					leftRotate(x->parent);
					s = x->parent->right;
				}

				if (s->left->color == "RED" && s->right->color == "RED") {
					s->color = "BLACK";
					s = x->parent;
				}
				else {
					if (s->right->color == "RED") {
						s->left->color = "RED";
						s->color = "BLACK";
						rightRotate(s);
						s = x->parent->right;
					}
					s->color = x->parent->color;
					x->parent->color = "RED";
					s->right->color = "RED";
					leftRotate(x->parent);
					x = root;
				}
			}
			else {
				s = x->parent->left;
				if (s->color == "BLACK") {
					s->color = "RED";
					x->parent->color = "BLACK";
					rightRotate(x->parent);
					s = x->parent->left;
				}

				if (s->right->color == "RED" && x->right->color == "RED") {
					s->color = "BLACK";
					x = x->parent;
				}
				else {
					if (s->left->color == "RED") {
						s->right->color = "RED";
						s->color = "BLACK";
						leftRotate(s);
						s = x->parent->left;
					}

					s->color = x->parent->color;
					x->parent->color = "RED";
					s->left->color = "RED";
					rightRotate(x->parent);
					x = root;
				}
			}
		}
		x->color = "RED";
	}

	Node* minimum(Node* node) {
		while (node->left != nullptr) {
			node = node->left;
		}
		return node;
	}

	// Transplant 
	void rbTransplant(Node* u, Node* v) {
		if (u->parent == nullptr) {
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


	void deleteNode(Node* node) {
		Node* y = nullptr;
		Node* x = nullptr;
		Node* z = node;
		y = z;
		string y_original_color = y->color;
		if (z->left == nullptr) {
			x = z->right;
			rbTransplant(z, z->right);
		}
		else if (z->right == nullptr) {
			x = z->left;
			rbTransplant(z, z->left);
		}
		else {
			y = minimum(z->right);
			y_original_color = y->color;
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
		if (y_original_color == "RED") {
			deleteFix(x);
		}
	}
};
void addEmployee(RedBlackTree* myTree) {
	string fName;
	string lName;
	string jTitle;
	int daSalary = 0;
	cout << "First Name of Employee\n";
	cin >> fName;
	cout << "Last Name of Employee\n";
	cin >> lName;
	cout << "Job Title\n";
	cin >> jTitle;
	cout << "Salary\n";
	cin >> daSalary;
	Node* myNode = new Node(daSalary, fName, lName, jTitle, nullptr, nullptr, nullptr);
	myTree->Insert(myTree, myNode);
}

void deleteEmployee(RedBlackTree* myTree) {
	string fName;
	string lName;
	string jTitle;
	int daSalary = 0;
	cout << "First name of the Employee \n";
	cin >> fName;
	cout << "Last name of the Employee \n";
	cin >> lName;
	cout << "Job Title\n";
	cin >> jTitle;
	cout << "Salary\n";
	cin >> daSalary;
	Node* myNode = new Node(daSalary, fName, lName, jTitle, nullptr, nullptr, nullptr);
	myTree->deleteNode(myNode);
}

void searchTraversal(Node* Node, int lower, int upper) {
	if (Node != nullptr) {
		searchTraversal(Node->getLeft(), lower, upper); // Traverse the left subtree
		if (Node->data > lower && Node->data < upper) {
			cout << "|  " << Node->firstName << " " << Node->getData() << " "; // Output the data 
		}
		searchTraversal(Node->getRight(), lower, upper); // Traverse the right subtree 

	}
}
void searchRange(RedBlackTree* myTree) {
	int lower = 0;
	int upper = 0;
	int counter = 0;
	do {
		if (counter > 0) {
			cout << "Lower bound is greater than upper bound. Try again\n";
		}
		cout << "Give a lower bound\n";
		cin >> lower;
		cout << "Give an upper bound\n";
		cin >> upper;
		counter++;
	} while (lower > upper);
	Node* currentNode = myTree->getRootNode();
	searchTraversal(currentNode, lower, upper);
	cout << "\n";
	// get user input for lower and upper bounds
	// go to lower bound and list till upper bound 
}

void initTree(RedBlackTree* myTree) {
	int counter = 1;
	while (counter < 100) {
		unsigned seed = chrono::system_clock::now().time_since_epoch().count();
		default_random_engine generator(seed);
		uniform_int_distribution<int> distribution(30000, 200000);
		int random_number = distribution(generator);
		cout << "Random number between 30000 and 200000: " << random_number << endl;
		Node* newNode = new Node(random_number, "random", "guy", "job", nullptr, nullptr, nullptr);
		myTree->Insert(myTree, newNode);
		counter++;
	}
}

void Menu(RedBlackTree* myTree) {
	cout << "1. Add an Employee\n2. Delete an Employee\n3. Search for Employees within Range\n4. Quit\n";
	int choice = 0;
	cin >> choice;
	while (choice != 1 && choice != 2 && choice != 3 && choice != 4) {
		cout << "Not a valid choice\n";
		cin >> choice;
	}
	if (choice == 1) {
		// imp Add Employee
		addEmployee(myTree);
		Menu(myTree);
	}
	if (choice == 2) {
		deleteEmployee(myTree);
		//imp Delete Employee
		Menu(myTree);
	}
	if (choice == 3) {
		searchRange(myTree);
		//imp search
		Menu(myTree);
	}
	if (choice == 4) {
		//quit
	}
}

int main() {
	Node* node1 = new Node(20000, "Harold", "Miller", "Mayor", nullptr, nullptr, nullptr);
	RedBlackTree myTree(node1);
	initTree(&myTree);
	Menu(&myTree);

	return 0;
}