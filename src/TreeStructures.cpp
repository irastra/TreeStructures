#include <iostream>
#include <algorithm>
#include "tree_structures/Tree.h"
#include "tree_structures/RedBlackTree.h"
#include "tree_structures/SearchTree.h"
#include "tree_structures/BlanceTree.h"
#include "tree_structures/Test.h"

using namespace std;

int main() {
	// int cmd_list[] = { 0, 1, 0, 2, 0, 3, 0, 4, 0, 5};
	//Node* root = MonkeyTestCmdTranslator(cmd_list, sizeof(cmd_list) / sizeof(int), InsertValueToSearchTree, DelNodeFromSearchTree);
	//cout << ceil(log2(1000000)) << endl;
	g_check_ruler_func = SearchTreeCheckFunc;
	MokeyTest(1, 10000, InsertValueToSearchTree, DelNodeFromSearchTree);
	g_check_ruler_func = BlanceTreeCheckFunc;
	MokeyTest(1, 1000000, BlanceTreeInsert, BlanceTreeRemove);
	g_check_ruler_func = RBTreeCheckFunc;
	MokeyTest(1, 1000000, RBTreeInsert, RBTreeRemove);
	cout << "finish " << endl;
	return 0;
}

