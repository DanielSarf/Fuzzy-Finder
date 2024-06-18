#pragma once
#include <string>

#include "BKMap.h"

class BKMap;

class BKTreeNode
{
private:
	//The word represented by the BKTreeNode
	std::string word;
	//Point to BKMap which stores vector of children with unique levenshtein distances
	BKMap* children;
	//Stores levenshtein distance between word data-member and parent node's word data-member
	int distanceFromParent;
	//If this is set to true, then it is not returned in fuzzy search in BKTree data structure
	bool isDeletedBool;

public:
	//Sets word to input string, distanceFromParent to input int, sets children pointer to a new empty BKMap and initializes isDeleted to false
	BKTreeNode(std::string, int);

	//Returns word data-member
	std::string getWord();

	//Puts BKTreeNode* into children data-member
	void addChild(BKTreeNode*);

	//Returns a child BKTreeNode* that has a distanceFromParent data-member with the same value as input int
	//If such child does not exist (including if input int is 0), then returns nullptr
	BKTreeNode* getChildAtDistance(int);

	//Returns distanceFromParent data-member
	int getDistanceFromParent();

	//Checks if the word is set to deleted 
	bool isDeleted();

	//Sets word to deleted or not deleted by setting isDeleted to true or false respectively
	void setIsDeleted(bool);

	//Deletes children and thus deleting its compositional units recursively
	~BKTreeNode();
};