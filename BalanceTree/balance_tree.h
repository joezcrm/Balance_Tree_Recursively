#ifndef BALANCE_TREE_H
#define BALANCE_TREE_H
#include <vector>
#include <fstream>

typedef unsigned int UInt;
#define NODE_SIZE 32
#define DEFAULT_TREE_SIZE 16
#define MEMORY_SIZE 4096

template <typename T>
class TreeNode
{
public:
	UInt n_Counter;
	UInt n_Size;
	T* n_Values;
	TreeNode** n_Nodes;
	TreeNode* n_NextListNode;
	TreeNode* n_PreNode;
	TreeNode();
	~TreeNode();
};


template <typename T>
class BTree
{
private:
	UInt t_MaxBufferSize;
	UInt t_BufferSize;
	std::vector<TreeNode<T>*> t_Buffer;
	std::vector<TreeNode<T>*> t_DeletedBuffer;
	std::vector<TreeNode<T>*> t_ListHeads;
	std::vector<TreeNode<T>*> t_ListAccessor;
	TreeNode<T>* findLeafNode(TreeNode<T>* pCurrent, T value);
	TreeNode<T>* findExactNode(TreeNode<T>* pCurrent, T value);
	TreeNode<T>* searchNode(T value); 
	TreeNode<T>* getEmptyNode();
public:
	BTree();
	~BTree();
	bool insertValue(T value);
	bool deleteValue(T value);
	UInt getAllSortedValue(T* pValues, UInt maxSize);
	UInt getTreeSize();
	bool resetAccessor();
};
#endif