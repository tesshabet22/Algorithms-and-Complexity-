// Boston Beatty, Tessa Habet, and Grant Bytnar

#include <iostream>
#include <string>
#include <random>
#include <chrono>
//QUEST 5.1
//template<typename T>
class BinTreeNode {
public:

	//members

	int data;//salary

	std::string firstName;
	std::string lastName;
	std::string jobTitle;

	BinTreeNode* Left;
	BinTreeNode* Right;
	BinTreeNode* Parent;

//public:
	//methods
	BinTreeNode(int myData, std::string fName, std::string lName, std::string jTitle,
		BinTreeNode* nodeParent, BinTreeNode* leftKid, BinTreeNode* rightKid) {

		data = myData; firstName = fName; lastName = lName; jobTitle = jTitle;
		Parent = nodeParent; Left = leftKid; Right = rightKid;

	}

	bool isLeaf() {
		if (Left == nullptr && Right == nullptr) {
			return true;
		}
		else {
			return false;
		}
	}

	bool isRoot() {
		if (Parent == nullptr) {
			return true;
		}
		else {
			return false;
		}
	}

	//standard getters
	BinTreeNode* getLeft() {
		return Left;
	}

	BinTreeNode* getRight() {
		return Right;
	}

	BinTreeNode* getParent() {
		return Parent;
	}

	int getData() {
		return data;
	}

	//standard setters
	void setLeft(BinTreeNode* newLeft) {
		Left = newLeft;
	}

	void setRight(BinTreeNode* newRight) {
		Right = newRight;
	}

	void setParent(BinTreeNode* newParent) {
		Parent = newParent;
	}

	void setData(int newData) {
		data = newData;
	}
};

//template<typename T>
class BinTree {
protected:

	//members

	BinTreeNode* rootNode; // Specify the template parameter T here
	BinTreeNode* index;    // Specify the template parameter T here
	int size;

public:


	//methods

	BinTree(BinTreeNode* firstNode) {
		rootNode = firstNode;
		index = firstNode;
		size = 1;
	}

	//getters
	BinTreeNode* getRootNode() {
		return rootNode;
	}

	BinTreeNode* getIndexLocation() {
		return index;
	}

	int getIndexData() {
		return index->getData();
	}

	int getSize() {
		return size;
	}

	//QUEST 5.4 BSTREES IMPLEMENTATION
	//takes a target value. sets index to the root of the tree. compares the target value to the index's value.
	//if it is less than it moves left, unless there is a nullptr left, in which case it returns the index's getLeft
	//so it can easily be set in the insertion function. if it is more than it move right, unless there is a nullptr
	BinTreeNode* deleteSearch(BinTreeNode* index, int targetValue, std::string fName, std::string lName) {
		
		while (true) {

			if (targetValue < index->getData() && index->getLeft() != nullptr) {
				index = index->getLeft();
			}
			/*else if (targetValue < index->getData() && index->getLeft() == nullptr) {
				return index;
			}*/

			if (targetValue > index->getData() && index->getRight() != nullptr) {
				index = index->getRight();
			}
			/*else if (targetValue > index->getData() && index->getRight() == nullptr) {
				return index;
			}*/

			if (targetValue == index->getData() && fName == index->firstName && lName == index->lastName) {
				return index;
			}
			else if (targetValue == index->getData() ) {
				return deleteSearch(index->getLeft(), targetValue,fName,lName);
				return deleteSearch(index->getRight(), targetValue, fName, lName);
			}
		}
	}

	//finds where the node should be inserted by using the search function. It then sets the left or right pointer
	//of the insertionpoint to the the new node. It also sets the newNodes parent pointer to be the insertion
	//point
	BinTreeNode* insertSearch(int targetValue) {
		index = rootNode;
		while (true) {

			if (targetValue < index->getData() && index->getLeft() != nullptr) {
				index = index->getLeft();
			}
			else if (targetValue < index->getData() && index->getLeft() == nullptr) {
				return index;
			}

			if (targetValue > index->getData() && index->getRight() != nullptr) {
				index = index->getRight();
			}
			else if (targetValue > index->getData() && index->getRight() == nullptr) {
				return index;
			}

			if (targetValue == index->getData()) {
				return index;
			}
		}
	}
	void insertNode(BinTreeNode* newNode) {
		BinTreeNode* insertionPoint = insertSearch(newNode->getData());
		if (newNode->getData() < insertionPoint->getData()) {
			insertionPoint->setLeft(newNode);
			newNode->setParent(insertionPoint);
			size++;
		}
		else {
			insertionPoint->setRight(newNode);
			newNode->setParent(insertionPoint);
			size++;
		}
	}

	//takes a value that is going to be deleted from the tree and determines if its a leaf or not. If it is a leaf
	// itll just delete it. if it has one child it just scootches it up. if it has two children it will
	// determine which node needs moved up 
	void deleteNode(BinTreeNode* index, int targetValue, std::string fName, std::string lName) {
		// Find the node to delete
		BinTreeNode* nodeToDelete = deleteSearch(index, targetValue, fName, lName);
		//case 0- node dne
		if (nodeToDelete == nullptr) {
			return;
		}

		// case 1- deleting a leaf or a node with one child
		if (nodeToDelete->getLeft() == nullptr || nodeToDelete->getRight() == nullptr) {
			BinTreeNode* child = (nodeToDelete->getLeft() != nullptr) ? nodeToDelete->getLeft() : nodeToDelete->getRight();

			if (nodeToDelete == rootNode) {
				rootNode = child;
			}
			else {
				BinTreeNode* parent = findParent(rootNode, targetValue);
				if (parent->getLeft() == nodeToDelete) {
					parent->setLeft(child);
				}
				else {
					parent->setRight(child);
				}
			}
			delete nodeToDelete;
			size--;
		}

		// case 2: deleting a node with two children
		else {
			BinTreeNode* successorParent = nodeToDelete;
			BinTreeNode* successor = nodeToDelete->getRight();

			while (successor->getLeft() != nullptr) {
				successorParent = successor;
				successor = successor->getLeft();
			}

			nodeToDelete->setData(successor->getData());

			// Remove the successor node
			if (successorParent->getLeft() == successor) {
				successorParent->setLeft(successor->getRight());
			}
			else {
				successorParent->setRight(successor->getRight());
			}
			delete successor;
			size--;
		}
	}



	BinTreeNode* findParent(BinTreeNode* root, int targetValue) {
		if (root == nullptr || (root->getLeft() == nullptr && root->getRight() == nullptr)) {
			return nullptr;
		}

		if ((root->getLeft() != nullptr && root->getLeft()->getData() == targetValue) ||
			(root->getRight() != nullptr && root->getRight()->getData() == targetValue)) {
			return root;
		}

		if (targetValue < root->getData()) {
			return findParent(root->getLeft(), targetValue);
		}
		else {
			return findParent(root->getRight(), targetValue);
		}
	}

	BinTreeNode* findMin(BinTreeNode* node) {
		if (node == nullptr) {
			return nullptr;
		}

		while (node->getLeft() != nullptr) {
			node = node->getLeft();
		}
		return node;
	}

	//PREORDER TRAVERSAL
	//takes as input a node, to get the traversal of the whole tree, input the root node. this way allows for subtree
	// traversals. the first thing it does is output the data of the first/root node. Then moves down the left
	// subtree and outputs all elements. Then it moves onto the right subtree and outputs those elements.
	void preorderTraversal(BinTreeNode* Node) {
		if (Node != nullptr) {
			std::cout << Node->getData() << " "; // Output the data
			preorderTraversal(Node->getLeft()); // Traverse the left subtree
			preorderTraversal(Node->getRight()); // Traverse the right subtree
		}
	}

	//INORDER TRAVERSAL
	// splits the tree in half and traverses the left subtree first, outputting each element. once there
	// are no more elements in the left subtree it cannot call itself any more. This causes the function to move
	// on and output the root node then move onto the right subtree. Does the same as the left until
	// there are no more elements and it cannot call itself
	void inorderTraversal(BinTreeNode* Node) {
		if (Node != nullptr) {
			inorderTraversal(Node->getLeft()); // Traverse the left subtree
			std::cout <<"|  "<< Node->firstName << " " << Node->getData() << "  "; // Output the data
			inorderTraversal(Node->getRight()); // Traverse the right subtree
		}
	}

	//POSTORDER TRAVERSAL
	// takes a node as input as a starting point. traverses and outputs both subtrees before outputting the 
	//input nodes data
	void postorderTraversal(BinTreeNode* Node) {
		if (Node != nullptr) {
			postorderTraversal(Node->getLeft()); // Traverse the left subtree
			postorderTraversal(Node->getRight()); // Traverse the right subtree
			std::cout << Node->getData() << " "; // Output the data
		}
	}

};

void addEmployee(BinTree* myTree) {
	std::string fName;
	std::string lName;
	std::string jTitle;
	int daSalary = 0;
	std::cout << "First Name of Employee\n";
	std::cin >> fName;
	std::cout << "Last Name of Employee\n";
	std::cin >> lName;
	std::cout << "Job Title\n";
	std::cin >> jTitle;
	std::cout << "Salary\n";
	std::cin >> daSalary;
	BinTreeNode* myNode = new BinTreeNode(daSalary, fName, lName, jTitle, nullptr, nullptr, nullptr);
	myTree->insertNode(myNode);

}
void deleteEmployee(BinTree* myTree) {
	std::string fName;
	std::string lName;
	int daSalary = 0;
	std::cout << "First Name of Employee\n";
	std::cin >> fName;
	std::cout << "Last Name of Employee\n";
	std::cin >> lName;
	std::cout << "Salary\n";
	std::cin >> daSalary;
	myTree->deleteNode(myTree->getRootNode(), daSalary, fName, lName);
}

void searchTraversal(BinTreeNode* Node,int lower, int upper) {
	if (Node != nullptr) {
		searchTraversal(Node->getLeft(),lower,upper); // Traverse the left subtree
		if (Node->data > lower && Node->data < upper) {
			std::cout << "|  " << Node->firstName << " " << Node->getData() << "  "; // Output the data
		}
		searchTraversal(Node->getRight(),lower,upper); // Traverse the right subtree
	}
}
void searchRange(BinTree* myTree) {
	int lower = 0;
	int upper = 0;
	int counter = 0;
	do {
		if (counter > 0) {
			std::cout << "Lower bound is greater than upper bound. Try Again\n";
		}
		std::cout << "Give a lower bound\n";
		std::cin >> lower;
		std::cout << "Give an upper bound\n";
		std::cin >> upper;
		counter++;
	} while (lower > upper);
	BinTreeNode* currentNode = myTree->getRootNode();
	searchTraversal(currentNode, lower, upper);
	std::cout << "\n";
	//get user input for lower and upper bounds
	//go to lower bound and list til upper bound
}
void initTree(BinTree* myTree) {
	int counter = 1;
	while (counter < 10000) {
		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
		std::default_random_engine generator(seed);
		std::uniform_int_distribution<int> distribution(30000, 200000);
		int random_number = distribution(generator);
		std::cout << "Random number between 30000 and 200000: " << random_number << std::endl;
		BinTreeNode* newNode = new BinTreeNode(random_number, "random", "guy", "job", nullptr, nullptr, nullptr);
		myTree->insertNode(newNode);
		counter++;
	}
}

void Menu(BinTree* myTree) {
	std::cout << "1. Add an Employee\n2. Delete an Employee\n3. Search For Employees Within Range\n4. Quit \n";
	int choice = 0;
	std::cin >> choice;
	while (choice != 1 && choice != 2 && choice != 3 && choice != 4) {
		std::cout << "Not a valid choice\n";
		std::cin >> choice;
	}
	if (choice == 1) {
		//imp Add Employee
		addEmployee(myTree);
		Menu(myTree);
	}
	if (choice == 2) {
		deleteEmployee(myTree);
		//imp Delete Employee
		Menu(myTree);
	}
	if (choice == 3) {
		//imp Search
		searchRange(myTree);
		Menu(myTree);
	}
	if (choice == 4) {
		//quit
	}
	
}

int main() {

	BinTreeNode* node1 = new BinTreeNode(20000, "Harold", "Miller", "Mayor", nullptr, nullptr, nullptr);
	BinTree myTree(node1);
	initTree(&myTree);
	Menu(&myTree);


	return 0;
}

