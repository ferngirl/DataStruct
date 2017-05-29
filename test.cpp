
#include"RBTree.cpp"
#include<string>
#include"map.cpp"
#include"set.cpp"

void TestRBTreeNode()
{
	RBTree<int,int> rbt;
    rbt.Insert(1,1);
	rbt.Insert(2,2);
	rbt.Insert(3,3);
	rbt.Insert(4,4);
	rbt.Insert(5,5);

	rbt.InOrder();
	RBTree<int,int>::Iterator it  = rbt.Begin();
	while(it != rbt.End())
	{
		cout<<*it<<" ";
		++it;
	}
	cout<<endl;
	--it;
	cout<<*it<<endl;
	
}

void TestMap()
{
	Map<string,int> m;
	m.Insert("1",1);
	m.Insert("2",2);
	m.Insert("3",3);
	m.Insert("4",4);
	Map<string,int>::Iterator it = m.Begin();
	while(it != m.End())
	{
		cout<<*it<<" ";
		++it;
	}
	cout<<endl;

	--it;
	cout<<*it<<endl;

	cout<<m["1"]<<endl;

}

void TestSet()
{
	Set<string> s;
	s.Insert("111");
	s.Insert("222");
	s.Insert("333");
	s.Insert("444");

	Set<string>::Iterator it = s.Begin();
	while(it!=s.End())
	{
		cout<<*it<<" ";
		++it;
	}
	cout<<endl;
	--it;
	cout<<*it<<endl;
}

int main()
{
	//TestRBTreeNode();
	//TestMap();
	TestSet();
	getchar();
	return 0;
}