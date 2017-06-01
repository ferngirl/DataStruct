#include<stdio.h>
#include<utility>
#include<iostream>
using namespace std;

template<class k,size_t M>
struct BTreeNode
{

	BTreeNode()
		:_size(0)
		,_pParent(NULL)
	{
		for(size_t idx=0; idx<M+1; ++idx)
			_pSub[idx] = NULL;
	}

	k _keys[M];  //�洢��ֵ������
	BTreeNode* _pSub[M+1];  //�洢����ָ�����ָ��
	BTreeNode* _pParent;
	size_t _size;  //��Ч��ֵ�ĸ���
};


template<class k,size_t M>
class BTree
{
	typedef BTreeNode<k,M> Node;
public:
	BTree()
		:_pRoot(NULL)
	{}

	

	bool Insert(const k& key)
	{
		if(_pRoot == NULL)
		{
			_pRoot = new Node;
			_pRoot->_keys[0] = key;
			_pRoot->_size = 1;
			return true;
		}

		//���Ҳ���ڵ��λ��
		pair<Node*,int> ret = Find(key);
		if(ret.second > -1)
			return false;
		Node* pNode = ret.first;
		Node* pSub = NULL;
		k valuek = key;

		//��keyֵ���������Ѿ��ҵ��Ľڵ�
		while(true)
		{
			_Insert(pNode,valuek,pSub);

			if(pNode->_size < M)
				return true;
			//��Ҫ�Խڵ���з���
			Node* pNewNode = new Node;
			size_t mid = M/2;
			size_t index = 0;
			size_t idx = 0;

			//����Ԫ�غͺ���ָ�뵽pNewNode
			for(idx=mid+1; idx<pNode->_size; ++idx)
			{
				pNewNode->_keys[index] = pNode->_keys[idx];
				pNewNode->_pSub[index] = pNode->_pSub[idx];
				if(pNode->_pSub[idx])
				{
					pNode->_pSub[idx]->_pParent = pNewNode->_pSub[index];
					pNode->_pSub[idx] = NULL;
				}
				pNewNode->_size++;
				pNode->_size--;
				index++;
			}
			
			pNewNode->_pSub[index] = pNode->_pSub[idx];
			if(pNode->_pSub[idx])
			{
				pNode->_pSub[idx]->_pParent = pNewNode->_pSub[index];
				pNode->_pSub[idx] = NULL;
			}
			pNode->_size = pNode->_size - pNewNode->_size;

			//�жϸýڵ��Ƿ�Ϊ���ڵ�
			if(pNode->_pParent == NULL)
			{
				_pRoot = new Node;
				_pRoot->_keys[0] = pNode->_keys[mid];
				
				_pRoot->_pSub[0] = pNode;
				pNode->_pParent = _pRoot;
				_pRoot->_pSub[1] = pNewNode;
				pNewNode->_pParent = _pRoot;
				_pRoot->_size++;
				return true;
			}
			else
			{
				valuek = pNode->_keys[mid];
				pNode = pNode->_pParent;
				pSub = pNewNode;
			}
		}
	}

	pair<Node*,int> Find(const k& key)
	{
		Node* pCur = _pRoot;
		Node* pParent = NULL;
		while(pCur)
		{
			int index = 0;
			while(index < pCur->_size)
			{
				if(pCur->_keys[index] > key)
				{
					//pCur = pCur->_pSub[index];
					break;
				}
				else if(pCur->_keys[index] < key)
					index++;
				else
					return pair<Node*,int>(pCur,index);
			}

			//if(key > pCur->_keys[index-1])
			pParent = pCur;
			pCur = pCur->_pSub[index];//��key���������һ��keyֵ����ʱ
		}
		return pair<Node*,int>(pParent,-1);
	}
private:
	void _Insert(Node*& pNode,const k& key,Node* pSub)
	{
		int end = pNode->_size-1;
		while(end > -1)
		{
			if(pNode->_keys[end] > key)
			{
				pNode->_keys[end+1] = pNode->_keys[end];
				pNode->_pSub[end+2] = pNode->_pSub[end+1];
			}
			else
				break;
			end--;
		}
		pNode->_keys[end+1] = key;
		pNode->_pSub[end+2] = pSub;
		if(pSub)
			pSub->_pParent = pNode;
		pNode->_size++;
	}

private:
	Node* _pRoot;
};


