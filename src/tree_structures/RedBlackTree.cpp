﻿#include "RedBlackTree.h"

Node* RBRepairInsertTree(Node* node) {
	// escape r, r
	assert(!node->IsBalck());
	Node* parent = node->parent;
	Node* ori_root = node;
	Node* uncle = node->UncleNode();
	Node* grand_parent = parent->parent;
	//cout << node->key << " " << parent->key << " " << uncle->key<< " " << grand_parent->key<< endl;
	if (parent->IsBalck()) {
		return node;
	}
	// r, r, b
	if (uncle->IsBalck()) {
		// r, r, b, b
		Node* tmp;
		if (node->ImRightNode() && parent->ImLeftNode()) {
			parent->LeftRotation();
			tmp = parent;
			parent = node;
			node = tmp;
		}
		else if (node->ImLeftNode() && parent->ImRightNode()) {
			parent->RightRotation();
			tmp = parent;
			parent = node;
			node = tmp;
		}
		bool is_left = node->ImLeftNode();
		Node * new_root = nullptr;
		if ((node->ImLeftNode() && parent->ImLeftNode()) || (node->ImRightNode() && parent->ImRightNode()))
		{
			parent->MakeBlack();
			grand_parent->MakeRed();
			if (is_left) {
				new_root = grand_parent->RightRotation();
			}
			else {
				new_root = grand_parent->LeftRotation();;
			}
			return (new_root != nullptr && new_root->IsRoot()) ? new_root : ori_root; 
		}
		assert(true);
		return nullptr;
	}
	else {
		// r r b r
		parent->MakeBlack();
		uncle->MakeBlack();
		if (grand_parent->IsRoot()) {
			return grand_parent;
		}
		grand_parent->MakeRed();
		return RBRepairInsertTree(grand_parent);
	}
}

Node* RBTreeInsert(Node* root, int val) {
	Node* node = new Node(val);
	node->MakeRed();
	node->CreateLeaf();
	node->tree_type = TreeType::RED_BLACK_TREE;
	if (root == nullptr) {
		node->MakeBlack();
		return node;
	}
	Node* find_node = root;
	TreeFind(root, val, &find_node);
	if (find_node->key > val) {
		find_node->AddLeftChild(node);
	}
	else if (find_node->key < val) {
		find_node->AddRightChild(node);
	}
	else {
		find_node->key = val;
		return root;
	}
	if (find_node->IsBalck()) {
		// insert to balck, include inser to root
		return root;
	}
	Node* parent = node->parent;
	Node* grand_parent = node->parent->parent;
	Node* uncle = node->UncleNode();
	Node * new_root = nullptr;
	if (uncle->is_leaf) {
		//cout << "----" << endl;
		grand_parent->MakeRed();
		if (node->ImLeftNode() && parent->ImLeftNode()) {
			// gp -> right rot 
			parent->MakeBlack();
			new_root = grand_parent->RightRotation();
		}
		else if (node->ImRightNode() && parent->ImLeftNode()) {
			// gp -> l up rot
			node->MakeBlack();
			new_root = grand_parent->UpRotationLRSon();
		}
		else if (node->ImRightNode() && parent->ImRightNode()) {
			// gp -> left rot
			parent->MakeBlack();
			new_root = grand_parent->LeftRotation();
		}
		else {
			// gp -> r up rot
			node->MakeBlack();
			new_root = grand_parent->UpRotationRLSon();
		}
		return (new_root != nullptr && new_root->IsRoot()) ? new_root : root;
	}
	if (!uncle->IsBalck()) {
		parent->MakeBlack();
		uncle->MakeBlack();
		if (grand_parent->IsRoot()) {
			return grand_parent;
		}
		grand_parent->MakeRed();
		Node * new_root = RBRepairInsertTree(grand_parent);
		return (new_root != nullptr && new_root->IsRoot()) ? new_root : root;
	}
	assert(true);
	return nullptr;
}

Node* RBRepairRemoveTree(Node* node) {
	if (node->IsRoot()) {
		return node;
	}
	Node* brother = node->Brother();
	if (node->parent->IsBalck()) {
		if (node->Brother()->IsBalck()) {
			Node* local_root = nullptr;
			// b(-1) b b, ���Һڸ���ͬ,�����ƺڽڵ�(n-1)
			if (brother->left_child->IsBalck() && brother->right_child->IsBalck()) {
				// b(-1) b b (b, b)
				node->Brother()->MakeRed();
				return RBRepairRemoveTree(node->parent);
			}
			else if (brother->left_child->IsBalck() && node->ImLeftNode()) {
				// b(-1) b b (b, r)
				local_root = node->parent->LeftRotation();
				brother->right_child->MakeBlack();
				return local_root;
			}
			else if (brother->right_child->IsBalck() && node->ImRightNode()) {
				local_root = node->parent->RightRotation();
				brother->left_child->MakeBlack();
				return local_root;
			}
			else {
				//b(-1) b b (r, r/b)
				Node* local_root = nullptr;
				if (node->ImLeftNode()) {
					local_root = node->Brother()->left_child;
					node->parent->UpRotationRLSon();
				}
				else {
					local_root = node->Brother()->right_child;
					node->parent->UpRotationLRSon();
				}
				local_root->MakeBlack();
				return local_root;
			}
		}
		else {
			// b(-1) b r
			node->Brother()->MakeBlack();
			Node* b_l = brother->left_child;
			Node* b_r = brother->right_child;
			Node* b_l_l = brother->left_child->left_child;
			Node* b_l_r = brother->left_child->right_child;
			Node* b_r_l = brother->right_child->left_child;
			Node* b_r_r = brother->right_child->right_child;
			Node* local_parent = brother;
			Node* new_target = nullptr;
			Node* n_l = nullptr;
			Node* n_r = nullptr;
			if (node->ImLeftNode()) {
				node->parent->LeftRotation();
				if (b_l_l->IsBalck() && b_l_r->IsBalck()) {
					b_l->MakeRed();
				}
				else {
					new_target = b_l;
					n_l = new_target->left_child;
					n_r = new_target->right_child;
					if (n_r->IsBalck()) {
						new_target->RightRotation();
						new_target = n_l;
						n_l = new_target->left_child;
						n_r = new_target->right_child;
						n_r->MakeRed();
					}
					local_parent->left_child->LeftRotation();
					new_target->MakeRed();
					n_r->MakeBlack();
				}
			}
			else {
				node->parent->RightRotation();
				if (b_r_l->IsBalck() && b_r_r->IsBalck()) {
					b_r->MakeRed();
				}
				else {
					new_target = b_r;
					n_l = new_target->left_child;
					n_r = new_target->right_child;
					if (n_l->IsBalck()) {
						new_target->LeftRotation();
						new_target = n_r;
						n_l = new_target->left_child;
						n_r = new_target->right_child;
						n_l->MakeRed();
					}
					local_parent->right_child->RightRotation();
					new_target->MakeRed();
					n_l->MakeBlack();

				}
			}
			return local_parent;
		}
	}
	else {
		Node* local_root = nullptr;
		// b(-1) r b
		Node* b_l = brother->left_child;
		Node* b_l_l = b_l->left_child;
		Node* b_l_r = b_l->right_child;
		Node* b_r = brother->right_child;
		Node* b_r_l = b_r->left_child;
		Node* b_r_r = b_r->right_child;
		Node* parent = node->parent;
		if (node->ImLeftNode()) {
			if (!b_l->IsBalck() && b_r->IsBalck()) {
				local_root = brother->RightRotation();
				brother->MakeRed();
				b_l->MakeBlack();
				parent->LeftRotation();
			}
			else {
				local_root = parent->LeftRotation();
				if ((!b_l->IsBalck() && !b_r->IsBalck()) || !b_r->IsBalck()) {
					b_r->MakeBlack();
					brother->MakeRed();
					parent->MakeBlack();
				}
			}
		}
		else {
			if (!b_r->IsBalck() && b_l->IsBalck()) {
				local_root = brother->LeftRotation();
				brother->MakeRed();
				b_r->MakeBlack();
				parent->RightRotation();
			}
			else {
				local_root = parent->RightRotation();
				if ((!b_l->IsBalck() && !b_r->IsBalck()) || !b_l->IsBalck()) {
					b_l->MakeBlack();
					brother->MakeRed();
					parent->MakeBlack();
				}
			}
		}
		return local_root;
	}
}

Node* RBTreeRemove(Node* root, int val) {
	if (root == nullptr) {
		return root;
	}
	Node* find_node_parent;
	Node* find_node = TreeFind(root, val, &find_node_parent);
	if (!find_node->left_child->is_leaf && !find_node->right_child->is_leaf) {
		Node* del_node = find_node;
		// # pre link node
		find_node = TreePreNode(del_node);
		//cout << "find node key_value : " << find_node->key << endl;
		del_node->key = find_node->key;
	}
	// can make delete node just has one child
	Node* parent = find_node->parent;
	if (find_node->IsRoot()) {
		// just has one child, delete root , make child be root
		if (!find_node->left_child->is_leaf) {
			find_node->key = find_node->left_child->key;
			Node* l_c = find_node->left_child;
			l_c->RemoveFromParent();
			delete l_c;
			return root;
		}
		else if (!find_node->right_child->is_leaf) {
			find_node->key = find_node->right_child->key;
			Node* r_c = find_node->right_child;
			r_c->RemoveFromParent();
			delete r_c;
			return root;
		}
		// delete root and not has any child
		delete find_node;
		return nullptr;
	}
	if (!find_node->IsBalck()) {
		// find_node is red, just delete and make it's child replace this
		bool is_left_child = find_node->ImLeftNode();
		if (find_node->left_child->is_leaf && find_node->right_child->is_leaf) {
			find_node->RemoveFromParent();
			delete find_node;
			return root;
		}
		else {
			// find not is red and it just has one child, not exist (r b, not blance), (r, r not regular) 
			assert(true);
		}
		assert(true);
	}
	else {
		// find node is balck and just has one red child or not has child 
		Node* l = find_node->left_child;
		Node* r = find_node->right_child;
		bool has_two_child = bool(!l->is_leaf && !r->is_leaf);
		assert(!has_two_child);
		if (!(l->is_leaf && r->is_leaf)) {
			if (!l->is_leaf) {
				find_node->key = l->key;
				l->RemoveFromParent();
				delete l;
				return root;
			}
			else {
				find_node->key = r->key;
				r->RemoveFromParent();
				delete r;
				return root;
			}
		}
		else {
			// find not is black and , find not not has child
			Node* brother = find_node->Brother();
			bool is_left = find_node->ImLeftNode();
			find_node->RemoveFromParent();
			delete find_node;
			Node* local_root = nullptr;
			// b - r - ?
			bool l_l = is_left && !brother->right_child->is_leaf;
			bool r_r = !is_left && !brother->left_child->is_leaf;
			if (!parent->IsBalck()) { // parent is red b - r  (black height 1)
				if (brother->left_child->is_leaf && brother->right_child->is_leaf) {
					// b - r - b
					parent->MakeBlack();
					brother->MakeRed();
					return root;
				}
				else if (l_l || r_r || (!brother->left_child->is_leaf && !brother->right_child->is_leaf)) {
					// b - r <<r> b <r>>   / b - r <b<r>>  
					local_root = brother;
					if (is_left) {
						parent->LeftRotation();
					}
					else {
						parent->RightRotation();
					}
					local_root->left_child->MakeBlack();
					local_root->right_child->MakeBlack();
					if (!local_root->IsRoot()) {
						// keep black height 1
						local_root->MakeRed();
						return root;
					}
					return local_root;
				}
				else {
					local_root = parent->UpRotation(is_left);
					local_root->left_child->MakeBlack();
					local_root->right_child->MakeBlack();
					if (local_root->IsRoot()) {
						local_root->MakeBlack();
						return local_root;
					}
					return root;
				}
			}
			else {
				bool l_l = is_left && brother->left_child->is_leaf;
				bool r_r = !is_left && brother->right_child->is_leaf;
				// b - b - r  
				if (!brother->IsBalck()) {
					// b - b - r (b, b)
					Node* b_l_l = brother->left_child->left_child;
					Node* b_l_r = brother->left_child->right_child;
					Node* b_r_l = brother->right_child->left_child;
					Node* b_r_r = brother->right_child->right_child;
					Node* ori_parent = parent;
					local_root = brother;
					local_root->MakeBlack();
					if (is_left) {
						parent->LeftRotation();
					}
					else {
						parent->RightRotation();
					}
					if (is_left && (!b_l_l->is_leaf || !b_l_r->is_leaf)) {
						local_root->left_child->LeftRotation();
						// b < <<r>b<r>> r <<r>b><r>>
						if (b_l_l->is_leaf) {
							local_root->left_child->left_child->MakeRed();
						}
						else {
							b_l_l->parent->parent->UpRotation(b_l_l->ImLeftNode());
						}
					}
					else if (!is_left && (!b_r_l->is_leaf || !b_r_r->is_leaf)) {
						// b < <<r>b<r>> r <<r>b><r>>
						local_root->right_child->RightRotation();
						if (b_r_r->is_leaf) {
							local_root->right_child->right_child->MakeRed();
						}
						else {
							b_r_r->parent->parent->UpRotation(b_r_r->ImLeftNode());
						}
					}
					else {
						// <b> b <<b>r<b>>
						if (is_left) {
							local_root->left_child->right_child->MakeRed();
						}
						else {
							local_root->right_child->left_child->MakeRed();
						}
					}
					//assert(true);
				}
				else if (brother->left_child->is_leaf && brother->right_child->is_leaf) {
					// <b> b <b>
					brother->MakeRed();
					local_root = RBRepairRemoveTree(parent);
				}
				else if (l_l || r_r || (!brother->left_child->is_leaf && !brother->right_child->is_leaf)) {
					// <b> b <r> b <r>
					local_root = brother;
					if (is_left) {
						parent->LeftRotation();
					}
					else {
						parent->RightRotation();
					}
					local_root->left_child->MakeBlack();
					local_root->right_child->MakeBlack();
				}
				else {
					// <b> b <r> b <r>
					local_root = parent->UpRotation(is_left);
					local_root->MakeBlack();
				}
				if (local_root != nullptr && local_root->parent == nullptr) {
					return local_root;
				}
				return root;
			}
		}
	}
	return root;
}
