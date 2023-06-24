#pragma once
#include "Tree.h"

int BlanceTreeHeight(Node* node);

int RefreshBlanceTreeHeight(Node* root, int height_step);

Node* RepairBlanceTree(Node* node);

Node* BlanceTreeInsert(Node* root, int val);

Node* BlanceTreeRemove(Node* root, int val);