#pragma once

#include<stdio.h>
#include<iostream>
using namespace std;


enum COLOR{RED,BLACK,};


template<class k,class v>
struct RBTreeNode
{
	RBTreeNode(const k& key=k(),const v& value = v(),COLOR color = RED)
		:_pLeft(NULL)
		,_pRight(NULL)
		,_pParent(NULL)
		,_key(key)
		,_value(value)
		,_color(color)
	{}

	k _key;
	v _value;
	COLOR _color;
	RBTreeNode<k,v>* _pLeft;
	RBTreeNode<k,v>* _pRight;
	RBTreeNode<k,v>* _pParent;
};

template<class k,class v,class Ref,class Ptr>
class RBTreeIterator
{
public:
	typedef RBTreeNode<k,v> Node;
	typedef RBTreeIterator<k,v,Ref,Ptr> Self;

	RBTreeIterator()
		:_pNode(NULL)
	{}
	RBTreeIterator(Node* pNode1)
		:pNode(pNode1)
	{}

	RBTreeIterator(const Self& s)
		:pNode(s.pNode)
	{}

	Self& operator++()
	{
		_Increase();
		return *this;
	}

	Self operator++(int)
	{
		Self temp(*this);
		_Increase();
		return temp;
	}

	Self& operator--()
	{
		_Decrease();
		return *this;
	}

	Self operator--(int)
	{
		Self temp(*this);
		_Decrease();
		rteurn *this;
	}

	Ref operator*()
	{
		return pNode->_key;
	}
	Ptr operator->()
	{
		return &(operator*());
	}
	bool operator!=(const Self& s)
	{
		return pNode!=s.pNode;
	}

	bool operator==(const Self& s)
	{
		return pNode==s.pNode;
	}

private:
	void _Increase()
	{
		if(pNode->_pRight)
		{
			pNode = pNode->_pRight;
			while(pNode->_pLeft)
				pNode = pNode->_pLeft;
		}
		else
		{
			Node* pParent = pNode->_pParent;
			while(pParent->_pRight == pNode)
			{
				pNode = pParent;
				pParent = pNode->_pParent;
			}
			if(pNode->_pRight != pParent)
				 pNode = pParent;
		}
	}

	void _Decrease()
	{
		if(pNode->_pParent->_pParent == pNode && pNode->_color == RED)
		{
			pNode = pNode->_pRight;
		}
		else if(pNode->_pLeft)
		{
			pNode = pNode->_pLeft;
			while(pNode->_pRight)
				pNode = pNode->_pRight;
		}
		else
		{
			Node* pParent = pNode->_pParent;
			while(pParent->_pLeft == pNode)
			{
				pNode = pParent;
				pParent = pNode->_pParent;
			}
			pNode = pParent;
		}
	}

//private:
	public:
	Node* pNode;
};

template<class k,class v>
class RBTree
{
public:
	typedef RBTreeNode<k,v> Node;
	typedef RBTreeIterator<k,v,k&,k*> Iterator;
	RBTree()
	{
		_pHead = new Node;
		_pHead->_pLeft = _pHead;
		_pHead->_pRight = _pHead;
		_pHead->_pParent = NULL;
	}

	bool Insert(const k& key,const v& value)
	{
		return _Insert(key,value);
	}

	Iterator Begin()
	{
		return Iterator(_pHead->_pLeft);
	}

	Iterator End()
	{
		return Iterator(_pHead);
	}

	bool Empty()
	{
		if(_pHead->_pParent == NULL)
			return true;
		return false;
	}

	Iterator Find(const k& key)
	{
		Node* pCur = _pHead->_pParent;
		while(pCur)
		{
			if(pCur->_key > key)
				pCur = pCur->_pLeft;
			else if(pCur->_key < key)
				pCur = pCur->_pRight;
			else
				return Iterator(pCur);
		}
		return Iterator(NULL);
	}


	void InOrder()
	{
		cout<<"InOrder: ";
		_InOrder(_pHead->_pParent);
		cout<<endl;
	}

	bool CheckRBTree()
	{
		Node*& _pRoot = _pHead->_pParent;
		if(_pRoot == NULL)
			return true;
		if(_pRoot->_pLeft == NULL && _pRoot->_pRight == NULL)
		{
			if(_pRoot->_color == BLACK)
				return true;
			else
				return false;
		}

		int blackcount = 0;
		Node* pCur = _pRoot;
		while(pCur)
		{
			if(pCur->_color == BLACK)
				blackcount++;
			pCur= pCur->_pLeft;
		}

		int k = 0;
		return _CheckRBTree(_pRoot,blackcount,k);
	}
private:
	bool _Insert(const k& key,const v& value)
	{
		Node* &pRoot = _pHead->_pParent;
		if(pRoot == NULL)
		{
			pRoot = new Node(key,value,BLACK);
			pRoot->_pParent = _pHead;
			_pHead->_pParent = pRoot;
		}

		//查找要插入节点的位置
		Node* pNode = pRoot;
		Node* pParent = NULL;
		while(pNode)
		{
			if(pNode->_key > key)
			{
				pParent= pNode;
				pNode = pNode->_pLeft;
			}
			else if(pNode->_key < key)
			{
				pParent = pNode;
				pNode = pNode->_pRight;
			}
			else
				return false;//insert_unique
		}

		pNode = new Node(key,value);
		if(pParent->_key > key)
		{
			pParent->_pLeft = pNode;
			pNode->_pParent = pParent;
		}
		else
		{
			pParent->_pRight = pNode;
			pNode->_pParent = pParent;
		}

		//插入后根据情况的不同需要进行调整

		while(pParent  && pParent->_color == RED && (pNode != pRoot))
		{
			Node* grandfather = pParent->_pParent;
			if(grandfather && grandfather->_pLeft == pParent)
			{
				Node* pUncle = grandfather->_pRight;
				if(pUncle && pUncle->_color == RED)
				{
					pUncle->_color = BLACK;
					pParent->_color = BLACK;
					pNode->_color = RED;

					pNode = grandfather;
					pParent = pNode->_pParent;
				}
				else
				{
					if(pUncle && pUncle->_color == BLACK && pParent->_pRight == pNode)
					{
						_RotateL(pParent);
						swap(pNode,pParent);
					}
					pParent->_color = BLACK;
					grandfather->_color = RED;
					_RotateR(grandfather);
				}
			}
			else
			{
				Node* pUncle = grandfather->_pLeft;
				if(pUncle && pUncle->_color == RED)
				{
					pUncle->_color = BLACK;
					pParent->_color = BLACK;
					grandfather->_color = RED;

					pNode = grandfather;
					pParent = pNode->_pParent;
				}
				else
				{
					if(pUncle && pUncle->_color == BLACK && pParent->_pLeft == pNode)
					{
						_RotateR(pParent);
						swap(pNode,pParent);
					}
					pParent->_color = BLACK;
					grandfather->_color = RED;
					_RotateL(grandfather);
				}
			}
		}
		pRoot->_color = BLACK;//情况一
		_pHead->_pLeft = GetMinNode();
		_pHead->_pRight = GetMaxNode();
		return true;
	}

	Node* GetMinNode()
	{
		if(_pHead->_pParent == NULL)
			return NULL;
		Node* pCur = _pHead->_pParent;
		while(pCur->_pLeft)
			pCur = pCur->_pLeft;

		return pCur;
	}

	Node* GetMaxNode()
	{
		if(_pHead->_pParent == NULL)
			return NULL;
		Node* pCur = _pHead->_pParent;
		while(pCur->_pRight)
			pCur = pCur->_pRight;
		return pCur;
	}



	void _RotateL(Node*& pParent)
	{
		Node* pSubR = pParent->_pRight;
		Node* pSubRL = pSubR->_pLeft;

		pParent->_pRight = pSubRL;
		if(pSubRL)
			pSubRL->_pParent = pParent;
		pSubR->_pLeft = pParent;
		Node* pPParent = pParent->_pParent;
		pParent->_pParent = pSubR;

		pSubR->_pParent = pPParent;
		if(pPParent == _pHead)
		{
			_pHead->_pParent = pSubR;

		}
		else if(pPParent->_pLeft == pParent)
			pPParent->_pLeft = pSubR;
		else 
			pPParent->_pRight = pSubR;
	}

	void _RotateR(Node*& pParent)
	{
		Node* pSubL = pParent->_pLeft;
		Node* pSubLR = pSubL->_pRight;

		pParent->_pLeft = pSubLR;
		if(pSubLR)
			pSubLR->_pParent = pParent;
		pSubL->_pRight = pParent;
		Node* pPParent = pParent->_pParent;
		pParent->_pParent = pSubL;

		pSubL->_pParent = pPParent;
		if(pPParent==NULL)
			_pHead->_pParent = pSubL;
		else if(pPParent->_pLeft == pParent)
			pPParent->_pLeft = pSubL;
		else
			pPParent->_pRight = pSubL;
	}

	bool _CheckRBTree(Node* pRoot,int blackcount,int k)
	{
		if(pRoot == NULL)
			return true;
		if(pRoot->_color == BLACK)
			++k;
		Node* pParent = pRoot->_pParent;
		if(pParent && pParent->_color == RED && pRoot->_color == RED)
		{
			cout<<"红色节点相邻，违反了性质3"<<endl;
			return false;
		}
		if(pRoot->_pLeft == NULL && pRoot->_pRight == NULL)
		{
			if(k != blackcount)
			{
				cout<<"每条路径的黑色节点个数不同，违反了性质4"<<endl;
				return false;
			}
		}
		return _CheckRBTree(pRoot->_pLeft,blackcount,k)&&_CheckRBTree(pRoot->_pRight,blackcount,k);
	}

	void _InOrder(Node* pRoot)
	{
		if(pRoot == NULL)
			return;
		_InOrder(pRoot->_pLeft);
		cout<<pRoot->_key<<" ";
		_InOrder(pRoot->_pRight);
	}

private:
	Node* _pHead;
};

