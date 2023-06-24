#pragma once
#include <vector>
#include <iostream>
#include <algorithm>
#include "Tree.h"
#include "time.h"
#include "RedBlackTree.h"
#include "BlanceTree.h"

using namespace std;

void FullBiaryTree(Node* root, int depth, int* idx) {
	if (root == nullptr) {
		return;
	}
	if (depth == 0) {
		return;
	}
	*idx += 1;
	Node* l = new Node(*idx);
	*idx += 1;
	Node* r = new Node(*idx);
	root->AddLeftChild(l);
	root->AddRightChild(r);
	FullBiaryTree(l, depth - 1, idx);
	FullBiaryTree(r, depth - 1, idx);
}

Node* CreateSimple7Tree(const int* value_list, int n) {
	vector<Node*> vec;
	for (int i = 0; i < n; i++) {
		int key_value = value_list[i];
		if (key_value < 0) {
			vec.push_back(nullptr);
			continue;
		}
		int idx = i + 1;
		Node* node = new Node(idx);
		node->CreateLeaf();
		vec.push_back(node);
		if (idx > 1) {
			bool is_left = idx / 2 * 2 == idx;
			int parent_idx = idx / 2 - 1;
			Node* parent = vec[parent_idx];
			if (parent == nullptr) {
				continue;
			}
			if (!parent->is_leaf) {
				if (is_left) {
					parent->AddLeftChild(node);
				}
				else {
					parent->AddRightChild(node);
				}
			}
		}
	}
	return vec.size() > 0 ? vec[0] : nullptr;
}

void UpRotationTest() {
	int valuse_list[7] = { 1, 2, 3, -1, 5, -1, -1 };
	Node* root = CreateSimple7Tree(valuse_list, 7);
	Node* rr = new Node(0);
	rr->AddLeftChild(root);
	PrintTree(rr);
	Node* n_root = root->UpRotation(false);
	if (n_root != nullptr) {
		rr = n_root;
	}
	PrintTree(rr);
}

void FullLUpRotationTest() {
	int valuse_list[17] = { 1, 2, 3, -1, -1, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17 };
	Node* root = CreateSimple7Tree(valuse_list, 15);
	PrintTree(root);
	root = root->UpRotationRLSon();
	PrintTree(root);
}

void FullRUpRotationTest() {
	int valuse_list[17] = { 1, 2, 3, 4, 5, -1, -1, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17 };
	Node* root = CreateSimple7Tree(valuse_list, 15);
	PrintTree(root);
	root = root->UpRotationLRSon();
	PrintTree(root);
}

void FullRUpRotationTest2() {
	int valuse_list[17] = { 1, -1, 3, 4, 5, 6, -1, 8, 9, 10, -1, -1, -1, 14, 15, 16, 17 };
	Node* root = CreateSimple7Tree(valuse_list, 15);
	PrintTree(root);
	root = root->UpRotationRLSon();
	PrintTree(root);
}

void FullLUpRotationTest2() {
	int valuse_list[17] = { 1, 2, -1, -1, 5, -1, -1, -1, -1, -1, -1, -1, -1, 14, 15, 16, 17 };
	Node* root = CreateSimple7Tree(valuse_list, 15);
	PrintTree(root);
	root = root->UpRotationLRSon();
	PrintTree(root);
}

int RandomInt(int a, int b) {
	return a + rand() % (b - a);
}

void PrintCmd(vector<int> cmd) {
	cout << "{";
	for (int i = 0; i < cmd.size(); i++) {
		cout << cmd[i];
		if (i != cmd.size() - 1) {
			cout << ",";
		}
	}
	cout << "}" << endl;
}

Node* MonkeyTestCmd(Node* root, int opt, int key_value, bool& res, vector<int>& cmd_vec, Node* TreeInsert(Node*, int), Node* TreeRemove(Node*, int), bool debug = true) {
	if (opt == 0) {
		Node* find_value = TreeFind(root, key_value, &find_value);
		if (find_value != nullptr) {
			res = false;
			return root;
		}
		if (debug) {
			cmd_vec.push_back(opt);
			cmd_vec.push_back(key_value);
			cout << "insert : " << key_value << endl;
			PrintCmd(cmd_vec);
		}
		root = TreeInsert(root, key_value);
	}
	else {
		Node* find_value = TreeFind(root, key_value, &find_value);
		if (find_value == nullptr) {
			res = false;
			return root;
		}
		if (debug) {
			cmd_vec.push_back(opt);
			cmd_vec.push_back(key_value);
			cout << " --- begin --- " << endl;
			PrintCmd(cmd_vec);
		}
		root = TreeRemove(root, key_value);
	}
	if (debug) {
		PrintTree(root);
	}
	return root;
}

Node* MonkeyTestCmdTranslator(const int* cmd_list, int len, Node* BRTreeInsert(Node*, int), Node* BRTreeRemove(Node*, int)) {
	Node* root = nullptr;
	vector<int> vec;
	for (int i = 0; i < len / 2; i++) {
		int opt = cmd_list[i * 2];
		int key_value = cmd_list[i * 2 + 1];
		bool opt_res;
		cout << opt << " " << key_value << endl;
		root = MonkeyTestCmd(root, opt, key_value, opt_res, vec, BRTreeInsert, BRTreeRemove);
		if (!opt_res) {
			break;
		}
	}
	return root;
}

void GenCmdList(int test_cnt, vector<int> & res){
	srand(time(nullptr));
	res.clear();	
	for (int i = 0; i < test_cnt; i++) {
		res.push_back(i);
	}
	std::random_shuffle(res.begin(), res.end());
}

void MokeyTest(int test_cnt, int max_value, Node* TreeInsert(Node*, int), Node* TreeRemove(Node*, int)) {
	vector<int> cmd;
	Node* root = nullptr;
	vector<int> value_list;
	GenCmdList(max_value, value_list);
	bool res = 1;
	cout << "GenTestFinish!" << endl;
	for(int test_idx = 0; test_idx < test_cnt; test_idx++){
		for(int opt_type = 0; opt_type < 2; opt_type ++){
			for (int idx = 0; idx < max_value; idx++) {
				int key_value = value_list[(idx + (test_idx + opt_type)) % max_value];
				bool opt_res = true;
				root = MonkeyTestCmd(root, opt_type, key_value, opt_res, cmd, TreeInsert, TreeRemove, false);
				// cost too many time
				//res = res && g_check_ruler_func(root);
				if (!res) {
					PrintTree(root);
					PrintCmd(cmd);
					break;
				}
			}
			cout << "Tree height:" << GetTreeHieght(root) << endl;
		}
	}
	cout << "finish:" << res << endl;
}