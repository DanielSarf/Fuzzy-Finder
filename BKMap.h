#pragma once
#include <iostream>

#include "BKTreeNode.h"

class BKTreeNode;

class BKMap
{
//In a BK Tree, nodes are not deleted and every child has a unique edit distance from it's parent.
//The edit distances also do not get unreasonably large, thus a hash function is not needed.
//Therefore, a Map that only increases in size, does not have a hash function and does not have a delete functionality is the requirement.
private:
	//Dynamic array of pointers to BKTreeNodes
	BKTreeNode** bucket;
	int length;
	int currentFilledSlots;

	//Sets bucket to a new array with increased length with data copied from previous array
	//and also sets length data-member to the new length
	void expandBucket(int);

public:
	//Initializes bucket to nullptr and other data-members 0
	BKMap();

	//Checks if BKMap is empty
	bool isEmpty();

	//Returns number of filled slots in bucket
	int getCurrentFilledSlots();

	//Takes key-value pair and insets data into bucket. If bucket is smaller than key then expands bucket first.
	void put(int, BKTreeNode*);

	//Returns value using key
	BKTreeNode* get(int);

	//Deletes all allocated memory for bucket and thus deleting its compositional units recursively
	~BKMap();
};