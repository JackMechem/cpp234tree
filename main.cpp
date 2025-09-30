#include "ttftree.hpp"
#include <iostream>
#include <vector>

int main() {
	RBTree<int> t;
	std::vector<int> vals = {49, 17, 34, 59, 76, 97, 10, 15, 20, 22,
							 37, 41, 46, 52, 57, 69, 75, 86, 89, 99};
	for (int v : vals)
		t.insert(v);

	std::cout << "Inorder: ";
	t.inorder();
	std::cout << "Tree:\n";
	t.printTree();
	t.verify();
}
