//文件压缩
//首先创建一个数组，大小为256，类型为FileInfo,然后初始化这个数组中的信息，初始化字符信息（ASCII）
//将文件中的内容读取一个自定义数组中，利用该数组统计文件中所有字符出现的次数，

#include<stdio.h>
#include<string>
#include <algorithm>
#include"HuffmanTree.cpp"
#include<iostream>
using namespace std;

struct FileInfo
{
	FileInfo(long long appearCount=0)
		:_appearCount(appearCount)
	{}

	char _ch;//字符
	long long _appearCount;//字符出现的次数
	string _strCode;//字符对应的字符编码
};
bool operator<(const FileInfo& pLeft,const FileInfo& pRight)
		{
			return pLeft._appearCount < pRight._appearCount;
		}

FileInfo operator+(const FileInfo& pLeft,const FileInfo& pRight)
		{
			FileInfo temp;
			temp._appearCount = pLeft._appearCount+pRight._appearCount;//
			return temp;
		}

class FileProcess
{
public:
	FileProcess()
	{
		for(size_t idx=0; idx< 256; ++idx)//初始化字符信息
			_fileInfo[idx]._ch = idx;//将idx做为字符的ASCII存入_ch中，表示文件中(不含汉字)可能出现的256个字符,
	}                                //但是ASCII值只有126个，所以126~156是未知字符，_ch是未知的

	void CreateFileProcess(string strPathName)//文件压缩
	{
		size_t strcount = 0;

		char filebuff[1024];//?????
		FILE *fIn = fopen(strPathName.c_str(),"rb");
		if(fIn == NULL)
		{
			cout<<"文件打开失败"<<endl;
			return;
		}
		while(1)//
		{
			strcount = fread(filebuff,1,1024,fIn);//从文件中读取字符放入数组filebuff中
			if(strcount == 0)
				break;;//没有从文件中读到字符
			//根据数组filebuff中的字符来初始化_fileInfo[256]中的_appearCount(字符出现次数)的信息
			for(size_t idx=0; idx<strcount; ++idx)
			_fileInfo[filebuff[idx]]._appearCount++;
		}

		
		//根据字符出现的次数创建huffman树
		
		HuffmanTree<FileInfo> ht(_fileInfo,256,FileInfo(0));//????

		//根据huffman树得到huffman编码
		GetHuffmanCode(ht.GetRoot());
		


		//将编码替换字符
		
		char readBuff[1024];
		char writeBuff[1024];
		size_t writesize=0;
		char value=0;
		int pos = 0;

		//memset(writeBuff,0,1024);
        fseek(fIn,0,SEEK_SET);//int fseek ( FILE * stream, long int offset, int origin );设置文件指针fIn指向起始的位置
		FILE* fOut = fopen("1.hzp","wb");
		if(fOut==NULL)
			cout<<"文件打开失败"<<endl;


		size_t linecount= 0;
		string strCode;
		char szAppearCount[32];//将出现的次数整型转字符型后存储在数组中

		string FileHead = GetFilePostFix(strPathName);//获取文件后缀".txt"

		for(size_t idx=0 ; idx<256; ++idx)//保存压缩信息，即字符，出现次数的信息
		{
			if(_fileInfo[idx]._appearCount !=0 )
			{
				strCode+=_fileInfo[idx]._ch;
				strCode+=',';
				strCode+=itoa(_fileInfo[idx]._appearCount,szAppearCount,10);//char *  itoa ( int value, char * str, int base )整型转字符型,base表示10进制
				strCode+='\n';
				linecount++;
			}
		}

		FileHead+='\n';//先保存后缀信息
		FileHead+=itoa(linecount,szAppearCount,10);//将信息的个数（行数）转换为字符，保存
		FileHead+='\n';
		FileHead+=strCode;//保存压缩的叶子节点的信息

		fwrite(FileHead.c_str(),1,FileHead.length(),fOut);//将所有的信息(string类型)转化为char* 类型保存写到压缩文件中去

		//将压缩后的编码写到文件中去
		while(true)
		{
			size_t readsize = fread(readBuff,1,1024,fIn);//????
			if(readsize==0)
				break;
			for(size_t idx=0; idx<readsize; ++idx)
			{
				string& strCode = _fileInfo[readBuff[idx]]._strCode;
				for(size_t i=0; i<strCode.length(); ++i)
				{
					value = value<<1;
					if(strCode[i]=='1')//用value的二进制来表示我们将要替换的编码
						value |= 1;
					pos++;
					if(pos==8)//标志位，当8位存放完成之后，我们将value的值写入我们的1.hzp文件中
					{
						writeBuff[writesize++] = value;
						if(writesize == 1024)
						{
							fwrite(writeBuff,1,1024,fOut);
							writesize=0;
						}
						pos = 0;
						value = 0;
					}
				}
			}
		}

		if(pos)
		{
			value<<(8-pos);
			writeBuff[writesize++] = value;
			//fwrite(writeBuff,1,1024,fOut);
		}

		if(writesize)
		{
			fwrite(writeBuff,1,writesize,fOut);///????总结出现的问题
		}

		fclose(fIn);
		fclose(fOut);
	}


	void unProcessFile(string strPathName)//文件解压缩
	{
		//读取压缩文件的内容，还原huffman树
		//从根节点开始查找，根据压缩的编码，编码为1，向左子树查找，编码为0，向右子树查找，直到找到叶子节点，叶子节点对应字符即为解压缩字符

		FILE* fIn = fopen(strPathName.c_str(),"rb");
		if(fIn==NULL)
			cout<<"文件打开失败"<<endl;

		string FilePosix;//文件后缀

  		ReadLine(fIn,FilePosix);//读取压缩文件一行的信息,现在已经获取到文件的后缀信息

		string strLineCount;//叶子节点信息的行数

		ReadLine(fIn,strLineCount);//现在已经获取到了行数的信息
		size_t LineCount = atoi(strLineCount.c_str());//将字符转化为整数

		string strCode;

		for(size_t idx=0; idx<LineCount; ++idx)//还原_fileInfo[]数组
		{
			ReadLine(fIn,strCode);
			_fileInfo[strCode[0]]._appearCount = atoi(strCode.substr(2).c_str());
			strCode="";
		}
		//根据压缩文件的信息，还原二叉树//?????
		HuffmanTree<FileInfo> ht(_fileInfo,sizeof(_fileInfo)/sizeof(_fileInfo[0]),FileInfo(0));

		//根据还原的二叉树，以及压缩编码，我们可以找到对应的字符信息
		Node<FileInfo>* pRoot = ht.GetRoot();
		Node<FileInfo>* pCur = ht.GetRoot();//根节点
		string PathAndName;
		char pWriteBuff[1024];
		char pReadBuff[1024];
		size_t readsize = 0;
		size_t writesize = 0;
		int pos = 7;
		size_t filesize = pCur->_weight._appearCount;//压缩文件的大小就是根节点的权值

		//获取解压文件的路径和名字，解压文件的后缀应该是fIn文件的第一行信息，，文件的路径又分为绝对路径和相对路径
		//这是由给出的strPathName，压缩文件的内容决定；
		size_t last = strPathName.find_last_of('.');
		PathAndName+=strPathName.substr(0,last);
		PathAndName+=FilePosix;

		  FILE* fOut = fopen(PathAndName.c_str(),"wb");//获取路径和名字

		while(true)
		{
			//用不用fseek?????
			readsize = fread(pReadBuff,1,1024,fIn);//读取压缩
			if(readsize == 0)
				break;
			//cout<<readsize<<endl;
			for(size_t idx=0; idx<readsize;) 
			{
				
				if(pReadBuff[idx]&(1<<pos))//第一个字节的二进制最高位是1，查找右孩子
					pCur = pCur->_pRight;
				else
					pCur = pCur->_pLeft;//否则是0，查找左孩子

				if(pCur&&pCur->_pLeft==NULL && pCur->_pRight==NULL)
				{
					
					pWriteBuff[writesize++] = pCur->_weight._ch;
					if(writesize==1024)
					{
						fwrite(pWriteBuff,1,1024,fOut);
						writesize=0;
					}
					filesize--;
					if(filesize==0)//压缩文件的所有字符都已解压完
					break;

					pCur = pRoot;//pcur回退到根节点的位置，开始解锁下一个字符
				}
				pos--;//查找次高比特位
				if(pos < 0)//一个字节已经解压完
				{
					idx++;
					pos = 7;
				}
			}
		}
		
		if(writesize)
		{
			fwrite(pWriteBuff,1,writesize,fOut);
		}
		fclose(fIn);
		fclose(fOut);
	}

private:
	void GetHuffmanCode(Node<FileInfo>* pRoot)// 得到逆序的huffman编码并翻转
	{
		if(pRoot == NULL)
			return;
		string& strCode = _fileInfo[pRoot->_weight._ch]._strCode;//???
		Node<FileInfo>* pCur = pRoot;
		Node<FileInfo>* pParent = pRoot;
		
		if(pCur->_pLeft==NULL && pCur->_pRight==NULL)//路径只显示了一个分支
		{
			pParent = pCur->_pParent;
			while(pParent)//没有回退到根节点
			{
				if(pParent->_pLeft == pCur)
					strCode+='0';
				else
					strCode+='1';
				pCur = pParent;
				pParent = pCur->_pParent;
			}
		}
		if(pParent==NULL)//判断该叶子节点的编码是否读完（读完的条件是pParent为NULL，所以开始的时候给pParent赋值为pRoot，而不是NULL，为了让第一次不进去），若没有，则不能翻转逆序的编码
		reverse(strCode.begin(),strCode.end());//将逆序的huffman编码转换为正序

		GetHuffmanCode(pRoot->_pLeft);
		GetHuffmanCode(pRoot->_pRight);
	}

	//获取文件后缀
	string GetFilePostFix(const string& strPathName)
	{
		size_t pos = strPathName.find_last_of('.');
		return strPathName.substr(pos,strPathName.length()-pos);//string substr ( size_t pos = 0, size_t n = npos ) const;
	}

	//每次从压缩文件中读取一行的信息
	void ReadLine(FILE*& fIn,string& strCode)
	{
		if(feof(fIn))//判断文件是否到达结尾，若到达文件结尾，则返回非0
			return;
		char ch = getc(fIn);
		while(ch != '\n')
		{
			strCode+=ch;
			if(feof(fIn))
				return;
			ch = getc(fIn);
		}
	}

private:
	FileInfo _fileInfo[256];//因为字符char为一个字节，8个比特位，2^8=256；所以可能出现的字符有256个
};


void funtest()//测试压缩
{
	FileProcess fp;
	string PathName = "1.txt";
	fp.CreateFileProcess(PathName);
	
 }
void funtest1()//测试解压缩
{
	FileProcess fp;
	string strPathName = "1.hzp";
	fp.unProcessFile(strPathName);
}


int main()
{
	
	funtest();
	getchar();
	return 0;
}


