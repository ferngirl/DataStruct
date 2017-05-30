
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
			cout<<"��ɫ�ڵ����ڣ�Υ��������3"<<endl;
			return false;
		}
		if(pRoot->_pLeft == NULL && pRoot->_pRight == NULL)
		{
			if(k != blackcount)
			{
				cout<<"ÿ��·���ĺ�ɫ�ڵ������ͬ��Υ��������4"<<endl;
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
		//�ҵ�Ҫ�ڵ�ɾ����λ��
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
				return false;//�ҵ�keyֵ�ڵ㣬���ò��룬ֱ���˳�
			}
		}

		//�Ѿ��ҵ�
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

		//�Ժ�������е���

		
		while(pParent && pParent->_color == RED && pNode->_color == RED)
		{
			Node* grandFather = pParent->_pParent;
			Node* pUncle = NULL;

			////�ҵ�pUncle�ڵ�
			//if(grandFather->_pLeft == pParent)
			//	pUncle = grandFather->_pRight;
			//else
			//	pUncle = grandFather->_pLeft;

			if(grandFather->_pLeft == pParent)//�ڵ����������
			{
				pUncle = grandFather->_pRight;
				if(pUncle && pUncle->_color == RED)//���һ
				{
					pUncle->_color = BLACK;
					pParent->_color = BLACK;
					grandFather->_color = RED;

					pNode = grandFather;
					pParent = grandFather->_pParent;

				}
				
				else//�ڴ���������Ĺ�����˳�㴦�������
				{
					if(pParent && pParent->_pRight == pNode)
					{
						//������������
						_RotateL(pParent);
						swap(pNode,pParent);
					}
					grandFather->_color = RED;
					pParent->_color = BLACK;
					_RotateR(grandFather);
				}
			}

				////�����
				//if(pNode->_pLeft == NULL && pNode->_pRight == NULL && pUncle == NULL)//pUncle������
				//{
				//	return true;
				//}
				//else
				//{
				//	if(pUncle && pUncle->_color == BLACK)
				//	{
				//		//�����ҵ���
				//		_RotateR(grandFather);
				//		grandFather->_color = RED;
				//		pParent->_color = BLACK;
				//	}
				//}

				////�����
				//if(pNode->_pLeft == NULL && pNode->_pRight == NULL && pUncle == NULL)//pUncle������
				//{
				//	return true;
				//}
				//else
				//{
				//	if(pUncle && pUncle->_color == BLACK && pParent->_pRight == pNode)
				//	{
				//		//������������
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
				if(pUncle && pUncle->_color == RED)//���һ
				{
					pUncle->_color = BLACK;
					pParent->_color = BLACK;
					grandFather->_color = RED;

					pNode = grandFather;
					pParent = grandFather->_pParent;

					
				}
				//�ڴ���������Ĺ�����˳�㴦�������
				
				else
				{
					if(pUncle && pUncle->_color == BLACK && pParent->_pLeft == pNode)//�����
					{
						//������������
						_RotateR(pParent);
						swap(pNode,pParent);
					}
					grandFather->_color = RED;//�����
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
		cout<<"�����Ǻ����"<<endl;
	}
	else
	{
		cout<<"�������Ǻ����"<<endl;
	}

}

int main()
{
	funtest();
	getchar();
	return 0;
}

