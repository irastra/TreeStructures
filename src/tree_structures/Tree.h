#pragma once
#include <assert.h>
#include <iostream>
#include <list>
#include <string>
#include "util.h"


using namespace std;
using namespace Util;
enum NodeColor {
	RED,
	BLACK
};

enum TreeType {
	NORMAL,
	BLACNE_TREE,
	RED_BLACK_TREE,
};

class Node {
public:
	NodeColor node_color;
	TreeType tree_type;
	Node* left_child;
	Node* right_child;
	Node* parent;
	bool is_leaf;
	int height;
	int order_idx;
	int width;
	int key;
	Node(int key_value);
	Node();
	void init();
	~Node();
	bool IsRoot();
	bool HasSon();
	void CreateLeaf();
	void MakeRed();
	void MakeBlack();
	void AddLeftChild(Node* node);
	void AddRightChild(Node* node);
	void RemoveFromParent();
	bool HasValueLeftChild();
	bool HasValueRightChild();
	bool ImLeftNode();
	bool ImRightNode();
	bool ImRootNode();
	bool IsBalck();
	Node* RightRotation();
	Node* LeftRotation();
	Node* UpRotationRLSon();
	Node* UpRotationLRSon();
	Node* UpRotation(bool is_left);
	Node* UncleNode();
	Node* Brother();
};

int _RBTreeCheckBlackHeight(Node* root, bool& valid);

bool RBTreeCheckFunc(Node* root);

bool BlanceTreeCheckFunc(Node* root);

bool SearchTreeCheckFunc(Node* root);

extern bool (*g_check_ruler_func)(Node*);

void RefreshNodePosition(Node* node, int** last_value);

void RefreshTree(Node* root);

void PrintTree(Node* node);

int TreeValueCnt(Node* root);

Node* TreeFind(Node* root, int val, Node** ret_parent);

Node* TreePreNode(Node* root);

Node* TreePostNode(Node* root);

int GetTreeHieght(Node* root);