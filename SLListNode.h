#pragma once

template <class T>
class SLListNode
{
private:
	T data;
	SLListNode<T>* next;

public:
	//Sets data data-member to input T and sets next data-member to input SLListNode*
	//but if the parameter is not passed then initializes next data-member to nullptr 
	SLListNode(T, SLListNode* = nullptr);

	//Sets data data-member to input T
	void setData(T);

	//Returns data data-member
	T getData();

	//Sets next data-member to input SLListNode*
	void setNext(SLListNode<T>*);

	//Returns next data-member
	SLListNode<T>* getNext();
};

template<class T>
inline SLListNode<T>::SLListNode(T inputData, SLListNode* inputNext)
{
	data = inputData;
	next = inputNext;
}

template<class T>
inline void SLListNode<T>::setData(T inputData)
{
	data = inputData;
}

template<class T>
inline T SLListNode<T>::getData()
{
	return data;
}

template<class T>
inline void SLListNode<T>::setNext(SLListNode<T>* inputNext)
{
	next = inputNext;
}

template<class T>
inline SLListNode<T>* SLListNode<T>::getNext()
{
	return next;
}