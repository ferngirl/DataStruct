
#include<stdio.h>
#include<iostream>
using namespace std;

enum COLOR{RED,BLACK,};

template<class k,class v>
struct RBTreeNode
{
	RBTreeNode(const k& key,const v& value,COLOR color = RED)
		:_pLeft(NULL)
		,_pRight(NULL)
		,_pParent(NULL)
		,_key(key)
		,_value(value)
		,_color(color)
	{}

	RBTreeNode<k,v>* _pLeft;
	RBTreeNode<k,v>* _pRight;
	RBTreeNode<k,v>* _pParent;
	k _key;
	v _value;
	COLOR _color;
};
template<class k,class v>
class RBTree
{
public:
	typedef RBTreeNode<k,v> Node;
	RBTree()
		:_pRoot(NULL)
	{}

	bool Insert(const k& key,const v& value)
	{
		return _Insert(key,value);
	}

	void InOrder()
	{
		cout<<"InOrder: ";
		_InOrder(_pRoot);
		cout<<endl;
	}

	bool CheckRBTree()
	{
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
	bool _Insert(const k& key,const v& value)
	{
		if(_pRoot == NULL)
		{
			_pRoot = new Node(key,value,BLACK);
			return true;
		}
		//找到要节点删除的位置
		Node* pNode = _pRoot;
		Node* pParent = NULL;
		while(pNode)
		{
			if(pNode->_key > key)
			{
				pParent = pNode;
				pNode = pNode->_pLeft;
			}
			else if(pNode->_key < key)
			{
				pParent = pNode;
				pNode = pNode->_pRight;
			}
			else 
			{
				return false;//找到key值节点，不用插入，直接退出
			}
		}

		//已经找到
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

		//对红黑树进行调整

		
		while(pParent && pParent->_color == RED && pNode->_color == RED)
		{
			Node* grandFather = pParent->_pParent;
			Node* pUncle = NULL;

			////找到pUncle节点
			//if(grandFather->_pLeft == pParent)
			//	pUncle = grandFather->_pRight;
			//else
			//	pUncle = grandFather->_pLeft;

			if(grandFather->_pLeft == pParent)//节点插在左子树
			{
				pUncle = grandFather->_pRight;
				if(pUncle && pUncle->_color == RED)//情况一
				{
					pUncle->_color = BLACK;
					pParent->_color = BLACK;
					grandFather->_color = RED;

					pNode = grandFather;
					pParent = grandFather->_pParent;

				}
				
				else//在处理情况三的过程中顺便处理情况二
				{
					if(pParent && pParent->_pRight == pNode)
					{
						//先左旋再右旋
						_RotateL(pParent);
						swap(pNode,pParent);
					}
					grandFather->_color = RED;
					pParent->_color = BLACK;
					_RotateR(grandFather);
				}
			}

				////情况二
				//if(pNode->_pLeft == NULL && pNode->_pRight == NULL && pUncle == NULL)//pUncle不存在
				//{
				//	return true;
				//}
				//else
				//{
				//	if(pUncle && pUncle->_color == BLACK)
				//	{
				//		//进行右单旋
				//		_RotateR(grandFather);
				//		grandFather->_color = RED;
				//		pParent->_color = BLACK;
				//	}
				//}

				////情况三
				//if(pNode->_pLeft == NULL && pNode->_pRight == NULL && pUncle == NULL)//pUncle不存在
				//{
				//	return true;
				//}
				//else
				//{
				//	if(pUncle && pUncle->_color == BLACK && pParent->_pRight == pNode)
				//	{
				//		//先左旋再右旋
				//		_RotateL(pParent);
				//		swap(pCur,pParent);
				//		grandFather->_color = RED;
				//		pParent->_color = BLACK;
				//		_RotateR(grandfather);
				//	}
				//}

			else
			{
				pUncle = grandFather->_pLeft;
				if(pUncle && pUncle->_color == RED)//情况一
				{
					pUncle->_color = BLACK;
					pParent->_color = BLACK;
					grandFather->_color = RED;

					pNode = grandFather;
					pParent = grandFather->_pParent;

					
				}
				//在处理情况三的过程中顺便处理情况二
				
				else
				{
					if(pUncle && pUncle->_color == BLACK && pParent->_pLeft == pNode)//情况三
					{
						//先左旋再右旋
						_RotateR(pParent);
						swap(pNode,pParent);
					}
					grandFather->_color = RED;//情况二
					pParent->_color = BLACK;
					_RotateL(grandFather);
				}
			}
		}
		_pRoot->_color = BLACK;
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
		if(pPParent == NULL)
			_pRoot = pSubR;
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
			_pRoot = pSubL;
		else if(pPParent->_pLeft == pParent)
			pPParent->_pLeft = pSubL;
		else
			pPParent->_pRight = pSubL;
	}
private:
	Node* _pRoot;
};

void funtest()
{
	RBTree<int ,int> rbt;
	int array[] = {40,30,60,10,50,80,20,70};
	for(size_t idx=0; idx<sizeof(array)/sizeof(array[0]); ++idx)
		rbt.Insert(array[idx],array[idx]);
	rbt.InOrder();
	if(rbt.CheckRBTree())
	{
		cout<<"该树是红黑树"<<endl;
	}
	else
	{
		cout<<"该树不是红黑树"<<endl;
	}

}

int main()
{
	funtest();
	getchar();
	return 0;
}

