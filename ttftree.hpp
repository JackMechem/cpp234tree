#pragma once
#include <iostream>
#include <memory>
#include <stdexcept>
#include <utility>

template <class Key, class Compare = std::less<Key>> class RBTree {
  public:
	RBTree() = default;

	bool contains(const Key &k) const {
		const Node *x = root_.get();
		while (x) {
			if (cmp_(k, x->key))
				x = x->left.get();
			else if (cmp_(x->key, k))
				x = x->right.get();
			else
				return true;
		}
		return false;
	}

	void insert(const Key &k) {
		// BST insert using unique_ptr ownership
		Node *parent = nullptr;
		std::unique_ptr<Node> *cur = &root_;
		while (cur->get()) {
			parent = cur->get();
			if (cmp_(k, parent->key))
				cur = &parent->left;
			else if (cmp_(parent->key, k))
				cur = &parent->right;
			else
				return; // no duplicates
		}
		cur->reset(new Node(k, /*red=*/true, parent));
		fixAfterInsert(cur->get());
		root_->red = false; // property 2: root is black
	}

	void inorder(std::ostream &os = std::cout) const {
		inorderVisit(root_.get(), os);
		os << '\n';
	}

	// Pretty print: one node per line, indentation by depth, color tag
	void printTree(std::ostream &os = std::cout) const {
		printNode(root_.get(), 0, os);
	}

	// Check core RB properties (throws if violated)
	void verify() const {
		if (root_ && root_->red)
			throw std::logic_error("root is not black");
		checkNoRedRed(root_.get());
		int h = blackHeight(root_.get());
		if (h < 0)
			throw std::logic_error("black-height mismatch");
	}

  private:
	struct Node {
		Key key;
		bool red; // true = red, false = black
		std::unique_ptr<Node> left, right;
		Node *parent = nullptr;
		Node(const Key &k, bool r, Node *p) : key(k), red(r), parent(p) {}
	};

	std::unique_ptr<Node> root_;
	Compare cmp_;

	static bool isRed(const Node *n) { return n && n->red; }

	// Return the owning unique_ptr that points to n (either parent's left/right
	// or root_)
	std::unique_ptr<Node> &linkTo(Node *n) {
		if (!n->parent)
			return root_;
		if (n->parent->left.get() == n)
			return n->parent->left;
		else
			return n->parent->right;
	}

	// Left rotate around x:
	//    x                 y
	//     \      ==>      / \
    //      y             x  (γ)
	//     /
	//   (β)
	void rotateLeft(Node *x) {
		Node *p = x->parent;
		auto &xLink = linkTo(x);
		std::unique_ptr<Node> y = std::move(x->right);
		Node *yRaw = y.get();

		x->right = std::move(yRaw->left);
		if (x->right)
			x->right->parent = x;

		yRaw->left = std::move(xLink);
		yRaw->left->parent = yRaw;
		yRaw->parent = p;

		xLink = std::move(y);
	}

	// Right rotate around x:
	//        x            y
	//       /    ==>     / \
    //      y            (α) x
	//       \               /
	//       (β)           (β)
	void rotateRight(Node *x) {
		Node *p = x->parent;
		auto &xLink = linkTo(x);
		std::unique_ptr<Node> y = std::move(x->left);
		Node *yRaw = y.get();

		x->left = std::move(yRaw->right);
		if (x->left)
			x->left->parent = x;

		yRaw->right = std::move(xLink);
		yRaw->right->parent = yRaw;
		yRaw->parent = p;

		xLink = std::move(y);
	}

	// Rebalance after inserting z (z is red)
	void fixAfterInsert(Node *z) {
		while (z->parent && z->parent->red) {
			Node *p = z->parent;
			Node *g = p->parent; // grandparent exists because parent is red and
								 // root is black
			if (p == g->left.get()) {
				Node *u = g->right.get(); // uncle
				if (isRed(u)) {
					// Case 1: parent & uncle red -> recolor and continue up
					p->red = false;
					u->red = false;
					g->red = true;
					z = g;
				} else {
					if (z == p->right.get()) {
						// Case 2: triangle -> rotate to line
						z = p;
						rotateLeft(z);
						p = z->parent;
						g = p->parent;
					}
					// Case 3: line -> rotate opposite and recolor
					p->red = false;
					g->red = true;
					rotateRight(g);
				}
			} else { // symmetric: parent is right child
				Node *u = g->left.get();
				if (isRed(u)) {
					p->red = false;
					u->red = false;
					g->red = true;
					z = g;
				} else {
					if (z == p->left.get()) {
						z = p;
						rotateRight(z);
						p = z->parent;
						g = p->parent;
					}
					p->red = false;
					g->red = true;
					rotateLeft(g);
				}
			}
		}
	}

	// Inorder traversal
	void inorderVisit(const Node *x, std::ostream &os) const {
		if (!x)
			return;
		inorderVisit(x->left.get(), os);
		os << x->key << ' ';
		inorderVisit(x->right.get(), os);
	}

	void printNode(const Node *n, int depth, std::ostream &os) const {
		if (!n)
			return;
		for (int i = 0; i < depth; ++i)
			os << "  ";
		os << (n->red ? "(R) " : "(B) ") << n->key << "\n";
		printNode(n->left.get(), depth + 1, os);
		printNode(n->right.get(), depth + 1, os);
	}

	// --- verification helpers ---
	void checkNoRedRed(const Node *n) const {
		if (!n)
			return;
		if (n->red) {
			if (isRed(n->left.get()) || isRed(n->right.get()))
				throw std::logic_error("red node has red child");
		}
		checkNoRedRed(n->left.get());
		checkNoRedRed(n->right.get());
	}

	// Returns black-height if consistent, else -1
	int blackHeight(const Node *n) const {
		if (!n)
			return 1; // null leaves are black
		int lh = blackHeight(n->left.get());
		int rh = blackHeight(n->right.get());
		if (lh < 0 || rh < 0 || lh != rh)
			return -1;
		return lh + (n->red ? 0 : 1);
	}
};
