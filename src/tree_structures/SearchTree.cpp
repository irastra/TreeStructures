#include "SearchTree.h"

Node* InsertValueToSearchTree(Node* root, int key_value) {
	Node* node = new Node(key_value);
	if (root == nullptr) {
		return node;
	}
	Node* insert_parent;
	Node * find_node = TreeFind(root, key_value, &insert_parent);
	if (find_node != nullptr) {
		return root;
	}
	if (insert_parent->key > key_value) {
		insert_parent->AddLeftChild(node);
	}
	else {
		insert_parent->AddRightChild(node);
	}
	return root;
}

Node* DelNodeFromSearchTree(Node* root, int key_value) {
	if (root == nullptr) {
		return nullptr;
	}
	if (key_value == root->key) {
		if (root->left_child == nullptr && root->right_child == nullptr) {
			root->RemoveFromParent();
			delete root;
			return nullptr;
		}
		else if (root->left_child != nullptr) {
			Node* find_node = TreePreNode(root);
			root->key = find_node->key;
			Node* parent = find_node->parent;
			bool is_left_child = parent->left_child == find_node ? true : false;
			find_node->RemoveFromParent();
			Node* f_l = find_node->left_child;
			if (f_l != nullptr) {
				f_l->RemoveFromParent();
				if (is_left_child) {
					parent->AddLeftChild(f_l);
				}
				else {
					parent->AddRightChild(f_l);
				}
			}
			delete find_node;
		}
		else {
			Node* find_node = TreePostNode(root);
			root->key = find_node->key;
			Node* parent = find_node->parent;
			bool is_left_child = parent->left_child == find_node ? true : false;
			find_node->RemoveFromParent();
			Node* f_r = find_node->right_child;
			if (f_r != nullptr) {
				f_r->RemoveFromParent();
				if (is_left_child) {
					parent->AddLeftChild(f_r);
				}
				else {
					parent->AddRightChild(f_r);
				}
			}
			delete find_node;
		}
		return root;
	}
	else if (key_value < root->key) {
		return DelNodeFromSearchTree(root->left_child, key_value);
	}
	else {
		return DelNodeFromSearchTree(root->right_child, key_value);
	}
}
