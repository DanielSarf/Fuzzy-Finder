#pragma once
#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <algorithm>

#include "BKTreeNode.h"

class BKTree
{
private:
	BKTreeNode* root;

	//Returns the levenshtein distance between two strings
	int stringDistance(std::string, std::string);

public:
	//Initializes root to nullptr
	BKTree();

	//Checks if BK Tree is empty
	bool isEmpty();

	//Inserts word into BK Tree based on the levenshtein distance metric
	void insertWord(std::string);

	//Searches for words within BK Tree which are in 0 - tolerance (inclusive) levenshtein distance of search query string 
	std::vector<std::string> fuzzySearch(std::string, int);

	//Looks for the word to be deleted in the tree and sets isDeleted bool of that word's node to true
	void deleteWord(std::string);

	//Deletes tree and thus deleting its compositional units recursively
	~BKTree();
};