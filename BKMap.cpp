#include "BKMap.h"

void BKMap::expandBucket(int newLength)
{
	//No need to expand the bucket if new length is smaller or equal to current length of bucket
	if (newLength <= length)
	{
		return;
	}

	//To avoid dereferencing the bucket nullptr
	if (isEmpty())
	{
		bucket = new BKTreeNode* [newLength];

		//Set all of the elements in bucket to nullptr
		for (int i = 0; i < newLength; i++)
		{
			bucket[i] = nullptr;
		}

		length = newLength;

		return;
	}

	//Pointer to where bucket pointer points to
	BKTreeNode** tempBucket = bucket;

	//New bucket of a larger length
	bucket = new BKTreeNode* [newLength];

	//Copy all the data from the old bucket to the new bucket
	for (int i = 0; i < length; i++)
	{
		bucket[i] = tempBucket[i];
	}

	//Set the rest of the elements in new bucket to nullptr
	for (int i = length; i < newLength; i++)
	{
		bucket[i] = nullptr;
	}

	//Set length data-member to the new length
	length = newLength;

	//Delete the old bucket
	delete[] tempBucket;
}

BKMap::BKMap()
{
	bucket = nullptr;
	length = 0;
	currentFilledSlots = 0;
}

bool BKMap::isEmpty()
{
	return (bucket == nullptr);
}

int BKMap::getCurrentFilledSlots()
{
	return currentFilledSlots;
}

void BKMap::put(int key, BKTreeNode* value)
{
	//If key is 0 or lower then do nothing
	if (key < 1)
	{
		return;
	}

	//If key is larger than bucket length then expand the bucket
	if (key > length)
	{
		expandBucket(key);
	}

	bucket[key - 1] = value;
	
	currentFilledSlots++;
}

BKTreeNode* BKMap::get(int key)
{
	//If the BKMap is empty, then return nullptr
	if (isEmpty())
	{
		return nullptr;
	}

	//If the key is out of range, then return nullptr
	if (key < 1 || key > length)
	{
		return nullptr;
	}

	//May return nullptr if a value was not put with the specified key
	return bucket[key - 1];
}

BKMap::~BKMap()
{
	//Deletes all elements of bucket
	for (int i = 0; i < length; i++)
	{
		delete bucket[i];
	}

	//Deletes allocated memory for bucket
	delete[] bucket;
	
	//Set bucket to nullptr and length and currentFilledSlots to 0
	bucket = nullptr;

	length = 0;

	currentFilledSlots = 0;
}