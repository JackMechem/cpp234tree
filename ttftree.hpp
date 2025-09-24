#pragma once
#include "node.hpp"
#include <iostream>
#include <memory>

template <typename Key, typename Compare = std::less<Key>> class Tree234 {
  public:
	// Constructor: initially the root is a leaf node with no keys
	Tree234() : root(std::make_unique<Node<Key>>(true)) {}

	// Insert a new key into the tree
	void insert(const Key &k) {
		// If the root is full (3 keys), we must split it first
		if (root->keys.size() == MAX_KEYS) {
			auto newRoot = std::make_unique<Node<Key>>(false);
			// old root becomes child 0 of the new root
			newRoot->children.push_back(std::move(root));
			// split that full child
			splitChild(newRoot.get(), 0);
			// update root
			root = std::move(newRoot);
		}
		// Now the root is guaranteed to be non-full, so we can insert
		insertNonFull(root.get(), k);
	}

	void inorder(std::ostream &os = std::cout) const {
		inorderVisit(root.get(), os);
		os << '\n';
	}

	// Print the structure of the tree
	void printTree(std::ostream &os = std::cout) const {
		printNode(root.get(), 0, os);
	}

  private:
	static constexpr std::size_t MIN_DEGREE = 2; // B-tree with t=2
	static constexpr std::size_t MAX_KEYS = 2 * MIN_DEGREE - 1; // 3 keys max
	static constexpr std::size_t MIN_KEYS =
		MIN_DEGREE - 1; // 1 key min (except root)

	// Root node
	std::unique_ptr<Node<Key>> root;
	// Compare function
	Compare cmp;

	// Small wrapper for comparisons
	bool less(const Key &a, const Key &b) const { return cmp(a, b); }

	void splitChild(Node<Key> *x, std::size_t i) {
		Node<Key> *y = x->children[i].get(); // child to split

		auto z = std::make_unique<Node<Key>>(y->leaf); // new sibling node

		// median index (for t=2, this is index 1 of 3 keys)
		const std::size_t m = MIN_DEGREE - 1;
		Key median = y->keys[m];

		// move keys after median into z
		z->keys.assign(y->keys.begin() + m + 1, y->keys.end());
		// erase median and right half from y
		y->keys.erase(y->keys.begin() + m, y->keys.end());

		// move children if y is not a leaf
		if (!y->leaf) {
			z->children.assign(
				std::make_move_iterator(y->children.begin() + MIN_DEGREE),
				std::make_move_iterator(y->children.end()));
			y->children.erase(y->children.begin() + MIN_DEGREE,
							  y->children.end());
		}

		// insert median into x
		x->keys.insert(x->keys.begin() + i, median);
		// add z as child after y
		x->children.insert(x->children.begin() + i + 1, std::move(z));
	}

	// --------------------
	// Insert into non-full node
	// --------------------
	void insertNonFull(Node<Key> *x, const Key &k) {
		if (x->leaf) {
			// insert key into correct sorted position in this leaf
			x->keys.insert(x->keys.begin() + lowerBound(x->keys, k), k);
			return;
		}

		// find child to descend into
		std::size_t i = childIndex(x, k);

		// if that child is full, split it first
		if (x->children[i]->keys.size() == MAX_KEYS) {
			splitChild(x, i);
			// decide whether to go left or right of the promoted median
			if (!cmp(k, x->keys[i]) && !cmp(x->keys[i], k))
				return; // equal
			else if (less(x->keys[i], k))
				i++;
		}

		// recurse
		insertNonFull(x->children[i].get(), k);
	}

	std::size_t lowerBound(const std::vector<Key> &vec, const Key &k) const {
		std::size_t lo = 0, hi = vec.size();
		while (lo < hi) {
			std::size_t mid = (lo + hi) / 2;
			if (less(vec[mid], k))
				lo = mid + 1;
			else
				hi = mid;
		}
		return lo; // position where k should go
	}

	std::size_t childIndex(const Node<Key> *x, const Key &k) const {
		return lowerBound(x->keys, k);
	}

	void inorderVisit(const Node<Key> *x, std::ostream &os) const {
		if (x->leaf) {
			for (auto &key : x->keys)
				os << key << ' ';
			return;
		}
		for (std::size_t i = 0; i < x->keys.size(); ++i) {
			inorderVisit(x->children[i].get(), os); // visit left child
			os << x->keys[i] << ' ';				// visit key
		}
		inorderVisit(x->children.back().get(), os); // visit last child
	}

	// Recursive helper to print a node with indentation
	void printNode(const Node<Key> *node, int depth, std::ostream &os) const {
		// Indent by depth (2 spaces per level)
		for (int i = 0; i < depth; i++)
			os << "  ";

		// Print this node's keys
		os << "[";
		for (std::size_t i = 0; i < node->keys.size(); ++i) {
			os << node->keys[i];
			if (i + 1 < node->keys.size())
				os << " ";
		}
		os << "]\n";

		// Recurse on children if not leaf
		if (!node->leaf) {
			for (auto &child : node->children) {
				printNode(child.get(), depth + 1, os);
			}
		}
	}
};
