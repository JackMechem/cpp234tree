#include "rbtree.hpp"
#include <iostream>
#include <vector>

int main() {
	RBTree<int> tree1;
	std::vector<int> insert_vals = {47, 43, 23, 90, 95, 27, 67, 80, 88, 29,
									59, 24, 69, 44, 71, 61, 99, 42, 38};
	for (int v : insert_vals)
		tree1.insert(v);

	std::cout << "Tree 1:\n";
	tree1.printTree();
	tree1.verify();

	std::vector<int> delete_vals = {27, 38, 44, 95, 88, 59};

	for (int rm : delete_vals) {
		std::cout << "\nErase " << rm << " -> " << std::boolalpha
				  << tree1.erase(rm) << "\n";
		tree1.printTree();
		tree1.verify();
	}

	RBTree<int> tree2;
	insert_vals = {17, 15, 49, 34, 76, 59, 97, 69, 46, 86,
				   20, 99, 22, 52, 89, 57, 10, 41, 75, 37};

	for (int v : insert_vals)
		tree2.insert(v);

	std::cout << "\n\n\nTree 2:\n";
	tree2.printTree();
	tree2.verify();

	delete_vals = {99, 22, 69, 15, 10, 75};

	for (int rm : delete_vals) {
		std::cout << "\nErase " << rm << " -> " << std::boolalpha
				  << tree2.erase(rm) << "\n";
		tree1.printTree();
		tree1.verify();
	}
}
