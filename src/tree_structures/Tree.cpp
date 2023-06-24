#include "Tree.h"
#include "BlanceTree.h"
#include <math.h>
using namespace std;

bool (*g_check_ruler_func)(Node*) = SearchTreeCheckFunc;

Node::Node(int key_value) {
	init();
	this->key = key_value;
}

Node::Node() {
	init();
}

void Node::init() {
	node_color = NodeColor::RED;
	left_child = right_child = parent = nullptr;
	is_leaf = false;
	height = order_idx = width = 0;
	tree_type = TreeType::NORMAL;
}

Node::~Node() {
	if (left_child != nullptr) {
		delete left_child;
	}
	if (right_child != nullptr) {
		delete right_child;
	}
	left_child = right_child = parent = nullptr;
	key = -1;
}

bool Node::IsRoot() {
	return parent == nullptr;
}

bool Node::HasSon() {
	Node* ll = left_child->left_child;
	Node* lr = left_child->right_child;
	Node* rl = right_child->left_child;
	Node* rr = right_child->right_child;
	return (ll != nullptr || lr != nullptr || rl != nullptr || rr != nullptr);
}

void Node::CreateLeaf() {
	assert(!is_leaf);
	if (left_child == nullptr) {
		Node* left = new Node(-1);
		left->node_color = NodeColor::BLACK;
		left->is_leaf = true;
		left->parent = this;
		left_child = left;
		left->tree_type = TreeType::RED_BLACK_TREE;
	}
	if (right_child == nullptr) {
		Node* right = new Node(-1);
		right->node_color = NodeColor::BLACK;
		right->is_leaf = true;
		right->parent = this;
		right_child = right;
		right->tree_type = TreeType::RED_BLACK_TREE;
	}
}

void Node::MakeRed() {
	if (is_leaf) {
		return;
	}
	if (node_color == NodeColor::RED) {
		return;
	}
	node_color = NodeColor::RED;
}

void Node::MakeBlack() {
	if (node_color == NodeColor::BLACK) {
		return;
	}
	node_color = NodeColor::BLACK;
}

void Node::AddLeftChild(Node* node) {
	assert(left_child == nullptr || left_child->is_leaf);
	assert(node->parent == nullptr);
	if (left_child != nullptr && left_child->is_leaf) {
		left_child->RemoveFromParent();
		delete left_child;
	}
	left_child = node;
	node->parent = this;
}

void Node::AddRightChild(Node* node) {
	assert(right_child == nullptr || right_child->is_leaf);
	assert(node->parent == nullptr);
	if (right_child != nullptr && right_child->is_leaf) {
		right_child->RemoveFromParent();
		delete right_child;
	}
	right_child = node;
	node->parent = this;
}

void Node::RemoveFromParent() {
	if (parent != nullptr) {
		if (parent->left_child == this) {
			parent->left_child = nullptr;
		}
		else {
			parent->right_child = nullptr;
		}
		if (tree_type == TreeType::RED_BLACK_TREE) {
			parent->CreateLeaf();
		}
		parent = nullptr;
	}
}

bool Node::HasValueLeftChild() {
	return left_child != nullptr && !left_child->is_leaf;
}

bool Node::HasValueRightChild() {
	return right_child != nullptr && !right_child->is_leaf;


}

bool Node::ImLeftNode() {
	return parent != nullptr && this == parent->left_child;
}

bool Node::ImRightNode() {
	return parent != nullptr && this == parent->right_child;
}

bool Node::ImRootNode() {
	return parent == nullptr;
}

Node* Node::UncleNode() {
	if (parent == nullptr || parent->parent == nullptr) {
		return nullptr;
	}
	return parent->ImLeftNode() ? parent->parent->right_child : parent->parent->left_child;
}

Node* Node::Brother() {
	if (parent == nullptr) {
		return nullptr;
	}
	return ImLeftNode() ? parent->right_child : parent->left_child;
}

bool Node::IsBalck() {
	return node_color == NodeColor::BLACK;
}

Node* Node::RightRotation() {
	Node* _parent = parent;
	Node* lr = left_child->right_child;
	Node* l = left_child;
	bool is_left = ImLeftNode();
	left_child->RemoveFromParent();
	RemoveFromParent();
	if (lr != nullptr) {
		lr->RemoveFromParent();
		AddLeftChild(lr);
	}
	l->AddRightChild(this);
	if (_parent != nullptr) {
		if (is_left) {
			_parent->AddLeftChild(l);
		}
		else {
			_parent->AddRightChild(l);
		}
		return l;
	}
	return l;
}

Node* Node::LeftRotation() {
	Node* _parent = parent;
	Node* rl = right_child->left_child;
	Node* r = right_child;
	bool is_left = ImLeftNode();
	right_child->RemoveFromParent();
	RemoveFromParent();
	if (rl != nullptr) {
		rl->RemoveFromParent();
		AddRightChild(rl);
	}
	r->AddLeftChild(this);
	if (_parent != nullptr) {
		if (is_left) {
			_parent->AddLeftChild(r);
		}
		else {
			_parent->AddRightChild(r);
		}
		return r;
	}
	return r;
}

Node* Node::UpRotationRLSon() {
	bool is_left = ImLeftNode();
	Node* _parent = parent;
	Node* _root = this;
	Node* r = right_child;
	Node* r_son = r->left_child;
	Node* r_son_l = r_son->left_child;
	Node* r_son_r = r_son->right_child;
	_root->RemoveFromParent();

	r_son->RemoveFromParent();
	r->RemoveFromParent();
	if (r_son_r != nullptr) {
		r_son_r->RemoveFromParent();
		r->AddLeftChild(r_son_r);
	}

	if (r_son_l != nullptr) {
		r_son_l->RemoveFromParent();
		_root->AddRightChild(r_son_l);
	}

	r_son->AddLeftChild(_root);
	r_son->AddRightChild(r);
	if (_parent != nullptr) {
		if (is_left) {
			_parent->AddLeftChild(r_son);
		}
		else {
			_parent->AddRightChild(r_son);
		}
		return r_son;
	}
	return r_son;
}

Node* Node::UpRotationLRSon() {
	bool is_left = ImLeftNode();
	Node* _parent = parent;
	Node* _root = this;
	Node* l = left_child;
	Node* l_son = l->right_child;
	Node* l_son_l = l_son->left_child;
	Node* l_son_r = l_son->right_child;
	_root->RemoveFromParent();

	l_son->RemoveFromParent();
	l->RemoveFromParent();
	if (l_son_l != nullptr) {
		l_son_l->RemoveFromParent();
		l->AddRightChild(l_son_l); // ok
	}
	if (l_son_r != nullptr) {
		l_son_r->RemoveFromParent();
		_root->AddLeftChild(l_son_r); // ok
	}
	l_son->AddRightChild(_root); // ok
	l_son->AddLeftChild(l); // ok
	if (_parent != nullptr) {
		if (is_left) {
			_parent->AddLeftChild(l_son);
		}
		else {
			_parent->AddRightChild(l_son);
		}
		return l_son;
	}
	return l_son;
}

Node* Node::UpRotation(bool is_left) {
	Node* l = left_child;
	Node* r = right_child;
	Node* r_l = r->left_child;
	Node* l_r = l->right_child;
	if (is_left) {
		UpRotationRLSon();
		return r_l;
	}
	else {
		UpRotationLRSon();
		return l_r;
	}
}

int _RBTreeCheckBlackHeight(Node* root, bool& valid) {
	if (root == nullptr) {
		return 0;
	}
	if (root->is_leaf) {
		valid = valid && root->IsBalck();
		return 1;
	}
	int l_b_h = _RBTreeCheckBlackHeight(root->left_child, valid);
	int r_b_h = _RBTreeCheckBlackHeight(root->right_child, valid);
	valid = valid && (l_b_h == r_b_h);
	if (root->IsBalck()) {
		return l_b_h + 1;
	}
	else {
		valid = valid && root->left_child->IsBalck() && root->right_child->IsBalck();
		if (root->parent == nullptr) {
			valid = false;
		}
		return l_b_h;
	}
}

bool RBTreeCheckFunc(Node* root) {
	bool ret = true;
	int black_heigt = _RBTreeCheckBlackHeight(root, ret);
	return ret;
}

void RefreshNodePosition(Node* node, int** last_value) {
	if (node == nullptr) {
		return;
	}
	node->width = 1;
	node->height = 1;
	if (node->left_child != nullptr) {
		RefreshNodePosition(node->left_child, last_value);
		node->width += node->left_child->width;
		node->height += node->left_child->height;
	}
	if (*last_value != nullptr) {
		node->order_idx = **last_value + 1;
	}
	else {
		node->order_idx = 1;
	}
	//cout << node->key << " " << node->order_idx << endl;
	*last_value = &node->order_idx;
	if (node->right_child != nullptr) {
		RefreshNodePosition(node->right_child, last_value);
		node->width += node->right_child->width;
		if (node->right_child->height + 1 > node->height) {
			node->height = node->right_child->height + 1;
		}
	}
}


void RefreshTree(Node* root) {
	int* ptr = nullptr;
	RefreshNodePosition(root, &ptr);
}

bool SearchTreeCheckFunc(Node* root) {
	return true;
}

bool BlanceTreeCheckFunc(Node* root) {
	if (root == nullptr) {
		return true;
	}
	int blance = BlanceTreeHeight(root->left_child) - BlanceTreeHeight(root->right_child);
	return fabs(blance) <= 1;
}

void PrintTree(Node* node) {
	Node* root = node;
	bool valid = g_check_ruler_func(root);
	if (!valid) {
		cout << " ============== invalid =============" << endl;
		return;
	}
	list<Node*> node_que1, node_que2;
	list<Node*>* cur_node_list_ptr = &node_que1, * back_node_list_ptr = &node_que2;
	list<Node*>* tmp = nullptr;
	int lidx = 0, ridx = 0;
	if (node == nullptr) {
		cout << " [Null] !" << endl;
		return;
	}
	RefreshTree(node);
	cur_node_list_ptr->push_back(node);
	while (!cur_node_list_ptr->empty()) {
		int cnt = 0;
		while (!cur_node_list_ptr->empty()) {
			node = cur_node_list_ptr->front();
			cur_node_list_ptr->pop_front();
			int print_cnt = node->order_idx - cnt - 1;
			for (int i = 0; i < print_cnt; i++) {
				print(" ");
				cnt += 1;
			}
			if (node->tree_type == TreeType::RED_BLACK_TREE) {
				if (node->is_leaf) {
					print("[*]");
				}
				else {
					if (node->node_color == NodeColor::RED) {
						print("<" + std::to_string(node->key) + ">");
					}
					else {
						print("[" + std::to_string(node->key) + "]");
					}
				}
			}
			else if (node->tree_type == TreeType::BLACNE_TREE) {
				print(std::to_string(node->key) + "|" + std::to_string(node->height));
			}
			else {
				print(std::to_string(node->key));
			}
			cnt += 1;
			if (node->left_child != nullptr) {
				back_node_list_ptr->push_back(node->left_child);
			}
			if (node->right_child != nullptr) {
				back_node_list_ptr->push_back(node->right_child);
			}
		}
		cout << endl;
		int section_print_cnt = 0;
		std::list<Node*>::iterator it = back_node_list_ptr->begin();
		for (; it != back_node_list_ptr->end(); it++) {
			Node* cur_node = *it;
			lidx = cur_node->order_idx;
			Node* brother = cur_node->Brother();
			ridx = brother == nullptr ? cur_node->parent->order_idx: brother->order_idx - 1;
			for (; section_print_cnt < lidx - 1; section_print_cnt++) {
				print_fill(" ");
			}
			for (; section_print_cnt <= ridx; section_print_cnt++) {
				print_fill("-");
			}
		}
		cout << endl;
		tmp = cur_node_list_ptr;
		cur_node_list_ptr = back_node_list_ptr;
		back_node_list_ptr = tmp;
	}
	for (int i = 0; i < root->width * 4; i++) {
		cout << "-";
	}
	cout << endl;
}


int TreeValueCnt(Node* root) {
	if (root == nullptr || root->is_leaf) {
		return 0;
	}
	return TreeValueCnt(root->left_child) + TreeValueCnt((root->right_child)) + 1;
}

Node* TreeFind(Node* root, int val, Node** ret_parent) {
	*ret_parent = nullptr;
	Node* find_node = root;
	while (find_node != nullptr && !find_node->is_leaf) {
		if (find_node->key == val) {
			break;
		}
		else if (find_node->key > val) {
			*ret_parent = find_node;
			find_node = find_node->left_child;
		}
		else {
			*ret_parent = find_node;
			find_node = find_node->right_child;
		}
	}
	return (find_node && find_node->key == val) ? find_node : nullptr;
}

Node* TreePreNode(Node* root) {
	if (root == nullptr || root->is_leaf) {
		return nullptr;
	}
	Node * find_node = root->left_child;
	while (find_node->HasValueRightChild()) {
		find_node = find_node->right_child;
	}
	return find_node;
}

Node* TreePostNode(Node* root) {
	if (root == nullptr || root->is_leaf) {
		return nullptr;
	}
	Node* find_node = root->right_child;
	while (find_node->HasValueLeftChild()) {
		find_node = find_node->left_child;
	}
	return find_node;
}

int GetTreeHieght(Node * root){
	if(root == nullptr || root->is_leaf){
		return 0;
	}
	int l_height = GetTreeHieght(root->left_child);
	int r_height = GetTreeHieght(root->right_child);
	return max(l_height, r_height) + 1;
}