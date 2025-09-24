#include "ttftree.hpp"
#include <iostream>

int main(int argv, char **argc) {

	Tree234<int> t; // create a tree storing int keys

	// Insert a sequence of numbers (forces many splits)
	std::vector<int> vals = {17, 15, 49, 34, 76, 59, 97, 69, 46, 86, 20,
							 99, 22, 52, 89, 57, 10, 41, 75, 37

	};
	for (int v : vals)
		t.insert(v);

	// Print tree in sorted order
	std::cout << "Inorder: ";
	t.inorder();
	std::cout << "\n Tree Structure: \n";
	t.printTree();

	return 0;
}
