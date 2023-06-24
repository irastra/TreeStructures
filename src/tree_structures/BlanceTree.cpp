#include <algorithm>
#include <math.h>
#include "Tree.h"
#include "BlanceTree.h"
using namespace std;

int BlanceTreeHeight(Node* node) {
	if (node == nullptr) {
		return 0;
	}
	return node->height;
}

int RefreshBlanceTreeHeight(Node* root, int height_step) {
	if (height_step <= 0 || root == nullptr) {
		return BlanceTreeHeight(root);
	}
	int l = RefreshBlanceTreeHeight(root->left_child, height_step - 1);
	int r = RefreshBlanceTreeHeight(root->right_child, height_step - 1);
	root->height = max(l, r) + 1;
	return root->height;
}

Node* RepairBlanceTree(Node* node) {
	if (node == nullptr || node->parent == nullptr) {
		return node;
	}
	Node* parent = node->parent;
	int old_parent_height = parent->height;
	RefreshBlanceTreeHeight(parent, 3);
	bool is_left = node->ImLeftNode();
	int node_height = BlanceTreeHeight(node);
	int brother_height = BlanceTreeHeight(node->Brother());
	int blance_factor = node_height - brother_height;
	int parent_hieght = 1 + max(node_height, brother_height);
	if (fabs(blance_factor) <= 1) {
		if (parent_hieght == old_parent_height) {
			return !node->IsRoot() ? nullptr : node;
		}
		else {
			parent->height = parent_hieght;
			return RepairBlanceTree(parent);
		}
	}
	if (blance_factor < 0) {
		return RepairBlanceTree(node->Brother());
	}
	Node* new_parent = nullptr;
	int m_blance_factor = BlanceTreeHeight(node->left_child) - BlanceTreeHeight(node->right_child);
	if (!is_left) {
		if (m_blance_factor <= 0) {
			new_parent = parent->LeftRotation();
			RefreshBlanceTreeHeight(new_parent, 3);
			if (m_blance_factor < 0) {
				new_parent = RepairBlanceTree(new_parent);
			}
		}
		else {
			new_parent = parent->UpRotationRLSon();
			RefreshBlanceTreeHeight(new_parent, 3);
			new_parent = RepairBlanceTree(new_parent);
		}
	}
	else {
		if (m_blance_factor >= 0) {
			new_parent = parent->RightRotation();
			RefreshBlanceTreeHeight(new_parent, 3);
			if (m_blance_factor > 0) {
				new_parent = RepairBlanceTree(new_parent);
			}
		}
		else {
			new_parent = parent->UpRotationLRSon();
			RefreshBlanceTreeHeight(new_parent, 3);
			new_parent = RepairBlanceTree(new_parent);
		}
	}
	return (new_parent != nullptr && new_parent->IsRoot()) ? new_parent : node;
}

Node* BlanceTreeInsert(Node* root, int val) {
	Node* new_node = new Node(val);
	new_node->height = 1;
	new_node->tree_type = TreeType::BLACNE_TREE;
	if (root == nullptr) {
		return new_node;
	}
	Node* find_node;
	Node* insert_parent = nullptr;
	find_node = TreeFind(root, val, &insert_parent);
	assert(find_node == nullptr);
	find_node = insert_parent;
	if (val < find_node->key) {
		find_node->AddLeftChild(new_node);
	}
	else {
		find_node->AddRightChild(new_node);
	}
	Node* new_root = RepairBlanceTree(new_node);
	return (new_root != nullptr && new_root->IsRoot()) ? new_root : root;
}

Node* BlanceTreeRemove(Node* root, int val) {
	if (root == nullptr) {
		return nullptr;
	}
	Node* del_node = nullptr;
	Node* insert_parent = nullptr;
	Node* find_node = nullptr;
	del_node = TreeFind(root, val, &insert_parent);
	if (del_node == nullptr) {
		return root;
	}
	if (del_node->left_child != nullptr && del_node->right_child != nullptr) {
		find_node = TreePreNode(del_node);
		del_node->key = find_node->key;
	}
	else {
		find_node = del_node;
	}
	bool is_left = find_node->ImLeftNode();
	Node* parent = find_node->parent;
	Node* l = find_node->left_child;
	Node* r = find_node->right_child;
	bool is_root = find_node->IsRoot();
	find_node->RemoveFromParent();
	if (is_root) {
		if (l != nullptr) {
			l->RemoveFromParent();
			delete find_node;
			return l;
		}
		else if (r != nullptr) {
			r->RemoveFromParent();
			delete find_node;
			return r;
		}
		delete find_node;
		return nullptr;
	}
	else if (l != nullptr) {
		l->RemoveFromParent();
		is_left ? parent->AddLeftChild(l) : parent->AddRightChild(l);
	}
	else if (r != nullptr) {
		r->RemoveFromParent();
		is_left ? parent->AddLeftChild(r) : parent->AddRightChild(r);
	}
	delete find_node;
	Node* new_root = nullptr;
	if (parent->left_child != nullptr) {
		new_root = RepairBlanceTree(parent->left_child);
	}
	else if (parent->right_child != nullptr) {
		new_root = RepairBlanceTree(parent->right_child);
	}
	else {
		new_root = RepairBlanceTree(parent);
	}
	return (new_root != nullptr && new_root->IsRoot()) ? new_root : root;
}