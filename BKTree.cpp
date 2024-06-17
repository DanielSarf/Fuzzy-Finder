#include "BKTree.h"

int BKTree::stringDistance(std::string s1, std::string s2)
{
	//Store lengths of strings
	int l1 = s1.length();
	int l2 = s2.length();

	//Make 2D array of ints of l1xl2 size
	int** distanceTable = new int* [l1 + 1];
	for (int i = 0; i <= l1; i++)
	{
		distanceTable[i] = new int[l2 + 1];
	}

	//Fill first row with numbers 0 - l1
	for (int i = 0; i <= l1; i++)
	{
		distanceTable[i][0] = i;
	}

	//Fill first column with numbers 0 - l2
	for (int i = 0; i <= l2; i++)
	{
		distanceTable[0][i] = i;
	}

	//Render 2D array
	int track = 0;
	int temp = 0;
	for (int i = 1; i <= l1; i++)
	{
		for (int j = 1; j <= l2; j++)
		{
			if (s1[i - 1] == s2[j - 1])
			{
				track = 0;
			}
			else
			{
				track = 1;
			}

			temp = (distanceTable[i - 1][j] > distanceTable[i][j - 1]) ? distanceTable[i][j - 1] + 1 : distanceTable[i - 1][j] + 1;

			distanceTable[i][j] = (temp > distanceTable[i - 1][j - 1] + track) ? distanceTable[i - 1][j - 1] + track : temp;
		}
	}

	//Save result
	int distance = distanceTable[l1][l2];

	//Deletes allocated memory for 2D array
	for (int i = 0; i <= l1; i++)
	{
		delete[] distanceTable[i];
	}
	delete[] distanceTable;

	return distance;
}

BKTree::BKTree()
{
	root = nullptr;
}

bool BKTree::isEmpty()
{
	return (root == nullptr);
}

void BKTree::insertWord(std::string inputWord)
{
	//If BK Tree, then insert first word as the root
	if (isEmpty())
	{
		root = new BKTreeNode(inputWord, -1);

		return;
	}

	//Set current node to root
	BKTreeNode* currentNode = root;

	//Calculate the levenshtein distance between root's word and inputWord
	int currentDistance = stringDistance(currentNode->getWord(), inputWord);

	//If currentDistance is 0, then nullptr is also returned
	BKTreeNode* childAtCurrentDistance = currentNode->getChildAtDistance(currentDistance);

	//Loop till a child with the same levenshtein distance as parent node's word and inputWord is not found
	//and if it is found, then this child node is the new parent node
	while (childAtCurrentDistance != nullptr)
	{
		currentNode = childAtCurrentDistance;

		currentDistance = stringDistance(currentNode->getWord(), inputWord);

		childAtCurrentDistance = currentNode->getChildAtDistance(currentDistance);
	}

	//If the current node's word is the same as the input word and the current node is set to deleted, then set it to not deleted
	if (currentDistance == 0)
	{
		if (currentNode->isDeleted())
		{
			currentNode->setIsDeleted(false);
		}

		//A child node's word with a levenshtein distance of 0 from its parent node's word should not be added, thus exit function
		return;
	}

	//A new node is made with the inputWord and the levenshtein distance between its parent node's word and inputWord as parameters
	BKTreeNode* newNode = new BKTreeNode(inputWord, currentDistance);

	//Add the new node as a child of the current node
	currentNode->addChild(newNode);
}

std::shared_ptr<SLList<std::string>> BKTree::fuzzySearch(std::string inputWord, int tolerance)
{
	//A new empty list is created
	std::shared_ptr<SLList<std::string>> listOfWords (new SLList<std::string>);

	//If the tree is empty, then return an empty list
	if (isEmpty())
	{
		return listOfWords;
	}

	//A list of nodes to be searched is made
	SLList<BKTreeNode*> listOfCandidateNodes;

	//First node to search is the root node
	listOfCandidateNodes.insertBeg(root);

	BKTreeNode* currentNode, * currentChildNode;

	int currentDistance, tolerance1, tolerance2;

	//Loop till there are no nodes remaining that are to be searched
	while (!listOfCandidateNodes.isEmpty())
	{
		//Get first node in listOfCandidatesNodes list
		currentNode = listOfCandidateNodes.getHead()->getData();

		//Calculate the levenshtein distance between current node's word and inputWord
		currentDistance = stringDistance(currentNode->getWord(), inputWord);
		tolerance1 = currentDistance - tolerance;
		//If tolderance1 is negative then set it to 0
		tolerance1 = tolerance1 > 0 ? tolerance1 : 0;
		tolerance2 = currentDistance + tolerance;

		//Find every child with a distance from the parent node within tolerance1 - tolerance2 (inclusive)
		for (int i = tolerance1; i <= tolerance2; i++)
		{
			currentChildNode = currentNode->getChildAtDistance(i);

			//If a child is found, insert it into the listOfCandidateNodes list
			if (currentChildNode != nullptr)
			{
				listOfCandidateNodes.insertEnd(currentChildNode);
			}
		}

		//Insert into the listOfWords list if the levenshtein distance between the current word and inputWord is below or equal to the tolerance 
		if (currentDistance <= tolerance && !currentNode->isDeleted())
		{
			listOfWords->insertEnd(currentNode->getWord());
		}
		
		//Delete current node from the listOfCandidateNodes list
		listOfCandidateNodes.deleteBeg();
	}

	//Return the search results
	return listOfWords;
}

void BKTree::deleteWord(std::string inputWord)
{
	//Start search of the inputWord from the root node
	BKTreeNode* currentNode = root;

	//Calculate the levenshtein distance between root's word and inputWord
	int currentDistance = stringDistance(currentNode->getWord(), inputWord);

	//Find a child with the same distance from the root to start the loop
	BKTreeNode* childAtCurrentDistance = currentNode->getChildAtDistance(currentDistance);

	//Loop till no more nodes left to search or node is found
	while (childAtCurrentDistance != nullptr && currentDistance != 0)
	{
		//The child node is the current node now
		currentNode = childAtCurrentDistance;

		currentDistance = stringDistance(currentNode->getWord(), inputWord);

		childAtCurrentDistance = currentNode->getChildAtDistance(currentDistance);
	}

	//If inputWord is found in the tree, then set it to deleted
	if (currentDistance == 0)
	{
		currentNode->setIsDeleted(true);

		return;
	}
}

BKTree::~BKTree()
{
	delete root;
}