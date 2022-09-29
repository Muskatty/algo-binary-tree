#include <fstream>
#include <set>
#include <iostream>

struct Node {
	Node* father;
	Node* left;
	Node* right;
	int height = 0;
	int maxHalfWayLenght;
	int fatherSideLength = 0;
	int leftHeight = 0;
	int rightHeight = 0;
	long long value;
	Node() { father = NULL; left = NULL; right = NULL; }
};

class Tree {
private:
	bool search(long long num, Node*& n)const {
		if (root == NULL) {
			n = NULL;
			return false;
		}
		n = root;
		for (;;) {
			if (n->value == num) {
				return true;
			}
			if (n->value > num) {
				if (n->left == NULL) {
					return false;
				}
				n = n->left;
			}
			else {
				if (n->right == NULL) {
					return false;
				}
				n = n->right;
			}
		}
	}

	void PreLeftOrderTraversal(Node* n, void action(long long&))const {
		if (n != NULL) {
			action(n->value);
			PreLeftOrderTraversal(n->left, action);
			PreLeftOrderTraversal(n->right, action);
		}
	}

	void PreLeftOrderTraversalNodes(Node* n, void action(Node*))const {
		if (n != NULL) {
			action(n);
			PreLeftOrderTraversalNodes(n->left, action);
			PreLeftOrderTraversalNodes(n->right, action);
		}
	}

	void ReverseOrderTraversal(Node* n, void action(long long&))const {
		if (n != NULL) {
			ReverseOrderTraversal(n->left, action);
			ReverseOrderTraversal(n->right, action);
			action(n->value);
		}
	};

	void ReverseOrderTraversalNodes(Node* n, void action(Node*))const {
		if (n != NULL) {
			ReverseOrderTraversalNodes(n->left, action);
			ReverseOrderTraversalNodes(n->right, action);
			action(n);
		}
	}

public:
	Node* root;
	Tree() { root = NULL; };
	bool insert(long long number) {
		Node* node;
		if (search(number, node)) {
			return false;
		}
		Node* q = new Node();
		q->value = number;
		if (node == NULL) {
			root = q;
			q->father = NULL;
		}
		else {
			q->father = node;
			if (number > node->value) {
				node->right = q;
			}
			else {
				node->left = q;
			}
		}
		return true;
	}
	void PreLeftOrder(void action(long long&)) {
		if (root == NULL) {
			return;
		}
		PreLeftOrderTraversal(root, action);
	}

	void PreLeftOrderNodes(void action(Node*)) {
		if (root == NULL) {
			return;
		}
		PreLeftOrderTraversalNodes(root, action);
	}

	void ReverseOrder(void action(long long&)) {
		if (root == NULL) {
			return;
		}
		ReverseOrderTraversal(root, action);
	}

	void ReverseOrderNodes(void action(Node*)) {
		if (root == NULL) {
			return;
		}
		ReverseOrderTraversalNodes(root, action);
	}

	void deleteNode(Node* node) {
		Node* temp = NULL;
		if (node->left != NULL) {
			temp = node->left;
		}
		else {
			temp = node->right;
		}
		if (temp != NULL) {
			temp->father = node->father;
		}
		if (node->father == NULL) {
			root = temp;
		}
		else {
			if ((node->father)->left == node) {
				(node->father)->left = temp;
			}
			else {
				(node->father)->right = temp;
			}
			delete node;
		}
		node = NULL;
	}

	bool rightDelete(long long number) {
		Node* tempNode;
		if (!search(number, tempNode)) {
			return false;
		}
		Node* temp2;
		if (tempNode->right != NULL && tempNode->left != NULL) {
			temp2 = tempNode->right;
			while (temp2->left != NULL) {
				temp2 = temp2->left;
			}
			tempNode->value = temp2->value;
			deleteNode(temp2);
		}
		else {
			deleteNode(tempNode);
		}
		return true;
	}

	void myTraversalGo(Node* node, int& number, int k) {
		if (node != NULL) {
			if (node->maxHalfWayLenght < k) {
				if (node->value > number) {
					number = node->value;
				}
			}
			myTraversalGo(node->left, number, k);
			myTraversalGo(node->right, number, k);
		}
	}

	void myTraversal(int& number, int k) {
		if (root == NULL) {
			return;
		}
		myTraversalGo(root, number, k);
	}
};

std::ofstream output("output.txt");

void print(long long& number) {
	output << number << "\n";
	//std::cout << number << "\n";
};

void setHeight(Node* node) {
	if (node->left == NULL && node->right == NULL) {
		node->height = 0;
	}
	else if (node->left != NULL && node->right != NULL) {
		node->leftHeight = node->left->height + 1;
		node->rightHeight = node->right->height + 1;
		node->height = std::max(node->right->height, node->left->height) + 1;
	}
	else if (node->left != NULL) {
		node->leftHeight = node->left->height + 1;
		node->height = node->left->height + 1;
	}
	else {
		node->rightHeight = node->right->height + 1;
		node->height = node->right->height + 1;
	}
}

void setFatherHeight(Node* node) {
	if (node->father != NULL) {
		if (node->father->left == node) {
			node->fatherSideLength = std::max(node->father->fatherSideLength, node->father->rightHeight) + 1;
		}
		else {
			node->fatherSideLength = std::max(node->father->fatherSideLength, node->father->leftHeight) + 1;
		}
	}
}

void setMaxHalfWays(Node* node) {
	node->maxHalfWayLenght = std::max(std::max(node->leftHeight + node->rightHeight, node->leftHeight + node->fatherSideLength),
		node->rightHeight + node->fatherSideLength);
}


int main() {
	std::ifstream input("input.txt");
	int k, delElement = -1;
	long long element;
	Tree tree;
	input >> k;
	//input.ignore();
	while (input >> element) {
		tree.insert(element);
	}
	input.close();
	tree.ReverseOrderNodes(setHeight);
	tree.PreLeftOrderNodes(setFatherHeight);
	tree.PreLeftOrderNodes(setMaxHalfWays);
	tree.myTraversal(delElement, k);
	tree.rightDelete(delElement);
	if (tree.root != NULL) {
		tree.PreLeftOrder(print);
	}
	else {
		output << "Empty" << "\n";
	}
	output.close();
	return 0;
}