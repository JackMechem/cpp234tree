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
		Node *parent = nullptr;
		std::unique_ptr<Node> *cur = &root_;
		while (cur->get()) {
			parent = cur->get();
			if (cmp_(k, parent->key))
				cur = &parent->left;
			else if (cmp_(parent->key, k))
				cur = &parent->right;
			else
				return;
		}
		cur->reset(new Node(k, true, parent));
		fixAfterInsert(cur->get());
		if (root_)
			root_->red = false;
	}

	bool erase(const Key &k) {
		Node *z = findNode(k);
		if (!z)
			return false;
		if (z->left && z->right) {
			Node *y = minimum(z->right.get());
			z->key = y->key;
			z = y;
		}
		Node *x = z->left ? z->left.get() : z->right.get();
		Node *xParent = z->parent;
		bool zRed = z->red;
		auto &zLink = linkTo(z);
		if (z->left)
			zLink = std::move(z->left);
		else
			zLink = std::move(z->right);
		if (zLink)
			zLink->parent = xParent;
		if (!zRed)
			fixAfterDelete(x, xParent);
		if (root_)
			root_->red = false;
		return true;
	}

	void inorder(std::ostream &os = std::cout) const {
		inorderVisit(root_.get(), os);
		os << '\n';
	}

	void printTree(std::ostream &os = std::cout) const {
		printNode(root_.get(), 0, os);
	}

	void verify() const {
		if (root_ && root_->red)
			throw std::logic_error("root is red");
		checkNoRedRed(root_.get());
		int h = blackHeight(root_.get());
		if (h < 0)
			throw std::logic_error("black-height mismatch");
	}

  private:
	struct Node {
		Key key;
		bool red;
		std::unique_ptr<Node> left, right;
		Node *parent = nullptr;
		Node(const Key &k, bool r, Node *p) : key(k), red(r), parent(p) {}
	};

	std::unique_ptr<Node> root_;
	Compare cmp_;

	static bool isRed(const Node *n) { return n && n->red; }
	static bool isBlack(const Node *n) { return !isRed(n); }

	Node *findNode(const Key &k) const {
		Node *x = root_.get();
		while (x) {
			if (cmp_(k, x->key))
				x = x->left.get();
			else if (cmp_(x->key, k))
				x = x->right.get();
			else
				return x;
		}
		return nullptr;
	}

	std::unique_ptr<Node> &linkTo(Node *n) {
		if (!n->parent)
			return root_;
		if (n->parent->left.get() == n)
			return n->parent->left;
		else
			return n->parent->right;
	}

	static Node *minimum(Node *n) {
		while (n->left)
			n = n->left.get();
		return n;
	}

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

	void fixAfterInsert(Node *z) {
		while (z->parent && z->parent->red) {
			Node *p = z->parent;
			Node *g = p->parent;
			if (p == g->left.get()) {
				Node *u = g->right.get();
				if (isRed(u)) {
					p->red = false;
					u->red = false;
					g->red = true;
					z = g;
				} else {
					if (z == p->right.get()) {
						z = p;
						rotateLeft(z);
						p = z->parent;
						g = p->parent;
					}
					p->red = false;
					g->red = true;
					rotateRight(g);
				}
			} else {
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

	void fixAfterDelete(Node *x, Node *parent) {
		while ((x != root_.get()) && isBlack(x)) {
			if (x == (parent ? parent->left.get() : nullptr)) {
				Node *w = parent ? parent->right.get() : nullptr;
				if (isRed(w)) {
					w->red = false;
					parent->red = true;
					rotateLeft(parent);
					w = parent->right.get();
				}
				if (isBlack(w->left.get()) && isBlack(w->right.get())) {
					w->red = true;
					x = parent;
					parent = x ? x->parent : nullptr;
				} else {
					if (isBlack(w->right.get())) {
						if (w->left)
							w->left->red = false;
						w->red = true;
						rotateRight(w);
						w = parent->right.get();
					}
					w->red = parent->red;
					parent->red = false;
					if (w->right)
						w->right->red = false;
					rotateLeft(parent);
					x = root_.get();
					parent = nullptr;
				}
			} else {
				Node *w = parent ? parent->left.get() : nullptr;
				if (isRed(w)) {
					w->red = false;
					parent->red = true;
					rotateRight(parent);
					w = parent->left.get();
				}
				if (isBlack(w->left.get()) && isBlack(w->right.get())) {
					w->red = true;
					x = parent;
					parent = x ? x->parent : nullptr;
				} else {
					if (isBlack(w->left.get())) {
						if (w->right)
							w->right->red = false;
						w->red = true;
						rotateLeft(w);
						w = parent->left.get();
					}
					w->red = parent->red;
					parent->red = false;
					if (w->left)
						w->left->red = false;
					rotateRight(parent);
					x = root_.get();
					parent = nullptr;
				}
			}
		}
		if (x)
			x->red = false;
	}

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

	int blackHeight(const Node *n) const {
		if (!n)
			return 1;
		int lh = blackHeight(n->left.get());
		int rh = blackHeight(n->right.get());
		if (lh < 0 || rh < 0 || lh != rh)
			return -1;
		return lh + (n->red ? 0 : 1);
	}
};
