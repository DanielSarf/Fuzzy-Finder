#include "BKTreeNode.h"

BKTreeNode::BKTreeNode(std::string inputWord, int inputDistanceFromParent)
{
	word.assign(inputWord);
	children = new BKMap();
	distanceFromParent = inputDistanceFromParent;
	isDeletedBool = false;
}

std::string BKTreeNode::getWord()
{
	return word;
}

void BKTreeNode::addChild(BKTreeNode* inputChild)
{
	//Puts BKTreeNode* into children data-member, with the key being its distance from its parent node
	children->put(inputChild->distanceFromParent, inputChild);
}

BKTreeNode* BKTreeNode::getChildAtDistance(int inputDistance)
{
	return children->get(inputDistance);
}

int BKTreeNode::getDistanceFromParent()
{
	return distanceFromParent;
}

bool BKTreeNode::isDeleted()
{
	return isDeletedBool;
}

void BKTreeNode::setIsDeleted(bool inputIsDeletedBool)
{
	isDeletedBool = inputIsDeletedBool;
}

BKTreeNode::~BKTreeNode()
{
	delete children;
}