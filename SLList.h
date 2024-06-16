#pragma once
#include <iostream>

#include "SLListNode.h"

template <class T>
class SLList
{
private:
	SLListNode<T>* head;
	SLListNode<T>* tail;
	int length;

public:
	//Initializes head and tail to nullptr and length to 0
	SLList();

	//Checks if list is empty
	bool isEmpty();

	//Returns head pointer (may return nullptr)
	SLListNode<T>* getHead();

	//Returns tail pointer (may return nullptr)
	SLListNode<T>* getTail();

	//Returns length of the list
	int getLength();

	//Returns the node pointer to the node at the specified position in the list if it exists, else returns nullptr
	SLListNode<T>* findNodeAtPos(int pos);

	//Inserts a node at the beginning of the list and increments length by 1
	void insertBeg(T inputData);
	
	//Inserts a node at the specified position in the list if it is in within a valid range and increments length by 1, else does nothing
	void insertAtPos(T inputData, int pos);

	//Inserts a node at the end of the list and increments length by 1
	void insertEnd(T inputData);

	//Returns data data-member of the first node, if the list is empty then returns 0 value
	T getBeg();

	//Returns data data-member of the node at the specified position in the list if it exists, else returns 0 value
	T getAtPos(int pos);

	//Returns data data-member of the last node, if the list is empty then returns 0 value
	T getEnd();
	
	//Deletes the node at the beginning of the list and decrements length by 1
	void deleteBeg();

	//Deletes the node at the specified position in the list if it is in within a valid range and decrements length by 1, else does nothing
	void deleteAtPos(int pos);

	//Deletes the node at the end of the list and decrements length by 1
	void deleteEnd();

	//Deletes every node in the list and sets head and tail to nullptr and sets length to 0
	void emptyList();

	//Calls emptyList()
	~SLList();
};

template<class T>
inline SLList<T>::SLList()
{
	head = nullptr;
	tail = nullptr;
	length = 0;
}

template<class T>
inline bool SLList<T>::isEmpty()
{
	return (length == 0);
}

template<class T>
inline SLListNode<T>* SLList<T>::getHead()
{
	return head;
}

template<class T>
inline SLListNode<T>* SLList<T>::getTail()
{
	return tail;
}

template<class T>
inline int SLList<T>::getLength()
{
	return length;
}

template<class T>
inline SLListNode<T>* SLList<T>::findNodeAtPos(int pos)
{
	//If pos is out of range
	if (pos < 0 || pos >= length)
	{
		return nullptr;
	}

	//Fast retrieval of first/last node
	if (pos == 0)
	{
		return head;
	}
	else if (pos == length - 1)
	{
		return tail;
	}

	//Set currentNode to head
	SLListNode<T>* currentNode = head;

	//Traverse list by setting currentNode to its next node till specified position
	for (int i = 0; i < pos; i++)
	{
		currentNode = currentNode->getNext();
	}

	return currentNode;
}

template<class T>
inline void SLList<T>::insertBeg(T inputData)
{
	//Make new node with data data-memeber set to inputData and next data-memeber set to head (which may be nullptr if list is empty)
	SLListNode<T>* newNode = new SLListNode<T>(inputData, head);

	//head is set to the new node
	head = newNode;

	//Increment length by 1
	length++;

	//When there is one element in the list, the tail is set to head which is the first node
	if (length == 1)
	{
		tail = head;
	}
}

template<class T>
inline void SLList<T>::insertAtPos(T inputData, int pos)
{
	//If pos is out of range
	if (pos < 0 || pos > length)
	{
		return;
	}
	
	//Fast insertion of first/last node
	if (pos == 0)
	{
		insertBeg(inputData);

		return;
	}
	else if (pos == length)
	{
		insertEnd(inputData);

		return;
	}
	
	//Find the node before poisition, set the new node's next data-memeber to previous node's next data-memeber,
	//set the previous node's next data-memeber to the new node 
	SLListNode<T>* previousNode = findNodeAtPos(pos - 1);
	SLListNode<T>* newNode = new SLListNode<T>(inputData, previousNode->getNext());
	previousNode->setNext(newNode);

	//Increment length by 1
	length++;
}

template<class T>
inline void SLList<T>::insertEnd(T inputData)
{
	//Make new node with data data-memeber set to inputData and next data-memeber set to nullptr
	SLListNode<T>* newNode = new SLListNode<T>(inputData, nullptr);

	//If list is not empty, then set existing tail node's next data-member to new node
	if (!isEmpty())
	{
		tail->setNext(newNode);
	}

	//Tail is now the new node
	tail = newNode;

	//Increment length by 1
	length++;

	//When there is one element in the list, the tail is set to head which is the first node
	if (length == 1)
	{
		head = tail;
	}
}

template<class T>
inline T SLList<T>::getBeg()
{
	//Return 0 value if list is empty to avoid nullptr exception
	if (isEmpty())
	{
		return { 0 };
	}

	return head->getData();
}

template<class T>
inline T SLList<T>::getAtPos(int pos)
{
	//If pos is out of range return 0 value to avoid nullptr exception
	if (pos < 0 || pos >= length)
	{
		return { 0 };
	}

	return findNodeAtPos(pos)->getData();
}

template<class T>
inline T SLList<T>::getEnd()
{
	//Return 0 value if list is empty to avoid nullptr exception
	if (isEmpty())
	{
		return { 0 };
	}

	return tail->getData();
}

template<class T>
inline void SLList<T>::deleteBeg()
{
	//If list is empty, there is not any node to delete
	if (isEmpty())
	{
		return;
	}

	//Set tempNode to head
	SLListNode<T>* tempNode = head;

	//Set head to the second node (which may be nullptr if the list has one node)
	head = tempNode->getNext();

	//Decrement length by 1
	length--;

	//When there is one element in the list, the tail is set to head which is the first node
	//When there are no nodes, tail is set to head which is nullptr
	if (length == 1 || isEmpty())
	{
		tail = head;
	}

	//Delete tempNode which used to be the first node
	delete tempNode;
}

template<class T>
inline void SLList<T>::deleteAtPos(int pos)
{
	//If pos is out of range
	if (pos < 0 || pos >= length)
	{
		return;
	}

	//Fast deletion of first/last node
	if (pos == 0)
	{
		deleteBeg();

		return;
	}
	else if (pos == length - 1)
	{
		deleteEnd();

		return;
	}

	//Find the node before poisition, set a temp node to previous node's next data-memeber,
	//set the previous node's next data-memeber to the node after the next node 
	SLListNode<T>* previousNode = findNodeAtPos(pos - 1);
	SLListNode<T>* tempNode = previousNode->getNext();
	previousNode->setNext(tempNode->getNext());

	//Decrement length by 1
	length--;

	//Delete tempNode which used to be the at the specified position
	delete tempNode;
}

template<class T>
inline void SLList<T>::deleteEnd()
{
	//If list is empty, there is not any node to delete
	if (isEmpty())
	{
		return;
	}

	//If there is only one node in the list
	if (length == 1)
	{
		//Delete tail which used to be the only node
		delete tail;

		//Set tail and head to nullptr and length to 0
		tail = nullptr;

		head = nullptr;

		length = 0;

		return;
	}

	//Set a temp node to tail
	SLListNode<T>* tempNode = tail;

	//Set tail to the second last node
	tail = findNodeAtPos(length - 2);

	//Set the next data-member of the last node to nullptr
	tail->setNext(nullptr);

	//Decrement length by 1
	length--;

	//Delete tempNode which used to be the last node
	delete tempNode;
}

template<class T>
inline void SLList<T>::emptyList()
{
	//If list is empty, there is not any node to delete
	if (isEmpty())
	{
		return;
	}
	//If there is only one node in the list
	else if (length == 1)
	{
		delete head;
	}
	else
	{
		SLListNode<T>* currentNode = head->getNext();
		SLListNode<T>* previousNode = head;

		//Traverse list setting current node to its next node while deleting the previous node until the end of the list
		while (currentNode != nullptr)
		{
			delete previousNode;

			previousNode = currentNode;

			currentNode = currentNode->getNext();
		}

		//Last pending delete
		delete previousNode;
	}

	//Set tail and head to nullptr and length to 0
	head = nullptr;
	tail = nullptr;

	length = 0;
}

template<class T>
inline SLList<T>::~SLList()
{
	emptyList();
}