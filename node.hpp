#pragma once
#include <memory>
#include <vector>

template <typename Key> class Node {
  public:
	bool leaf;			   // True if node has no children
	std::vector<Key> keys; // Keys sorted in order (max 3)
	std::vector<std::unique_ptr<Node<Key>>>
		children; // If not a leaf, one more child pointer than keys (max 4)

	// Constructor, create a node, defining if it is a leaf or not
	explicit Node(bool isLeaf) : leaf(isLeaf) {}
};
