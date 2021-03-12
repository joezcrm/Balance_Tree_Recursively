#include "balance_tree.h"
#include <iostream>
#include <algorithm>

template <typename T>
TreeNode<T>::TreeNode()
{
	// n_Counter is used to access data
	n_Counter = 0;
	// Current size of the array
	n_Size = 0;
	n_Values = new T[NODE_SIZE];
	n_Nodes = new TreeNode<T> * [NODE_SIZE + 1];
	for (UInt i = 0; i < NODE_SIZE + 1; i++)
	{
		n_Nodes[i] = nullptr;
	}
	n_NextListNode = nullptr;
	n_PreNode = nullptr;
}

template <typename T>
TreeNode<T>::~TreeNode()
{
	delete[] n_Values;
	delete[] n_Nodes;
}

template <typename T>
void reset(TreeNode<T>* tNode)
{
	if (tNode)
	{
		tNode->n_Counter = 0;
		tNode->n_Size = 0;
		tNode->n_PreNode = nullptr;
		tNode->n_NextListNode = nullptr;
		for (UInt i = 0; i < NODE_SIZE + 1; i++)
		{
			tNode->n_Nodes[i] = nullptr;
		}
	}
}

// Insert value and node pointers on both sides to the node
template <typename T>
bool insertValueToNode(TreeNode<T>* tNode, T value, TreeNode<T>* pLeft, TreeNode<T>* pRight)
{
	// If maximum size has been reached
	if (tNode->n_Size >= NODE_SIZE)
	{
		return false;
	}
	// Insert and connect nodes
	for (UInt i = 0; i < tNode->n_Size; i++)
	{
		if (value < tNode->n_Values[i])
		{
			for (UInt j = tNode->n_Size; j > i; j--)
			{
				tNode->n_Values[j] = tNode->n_Values[j - 1];
				tNode->n_Nodes[j + 1] = tNode->n_Nodes[j];
			}
			tNode->n_Values[i] = value;
			tNode->n_Nodes[i] = pLeft;
			if (tNode->n_Nodes[i])
			{
				tNode->n_Nodes[i]->n_PreNode = tNode;
			}
			tNode->n_Nodes[i + 1] = pRight;
			if (tNode->n_Nodes[i + 1])
			{
				tNode->n_Nodes[i + 1]->n_PreNode = tNode;
			}
			tNode->n_Size++;
			return true;
		}
	}
	tNode->n_Values[tNode->n_Size] = value;
	tNode->n_Nodes[tNode->n_Size] = pLeft;
	if (tNode->n_Nodes[tNode->n_Size])
	{
		tNode->n_Nodes[tNode->n_Size]->n_PreNode = tNode;
	}
	tNode->n_Nodes[tNode->n_Size + 1] = pRight;
	if (tNode->n_Nodes[tNode->n_Size + 1])
	{
		tNode->n_Nodes[tNode->n_Size + 1]->n_PreNode = tNode;
	}
	tNode->n_Size++;
	return true;
}

// Delete the value and the node pointer on the right
template <typename T>
bool deleteValueRightNode(TreeNode<T>* tNode, UInt location)
{
	if (location >= tNode->n_Size)
	{
		return false;
	}
	for (UInt i = location; i < tNode->n_Size - 1; i++)
	{
		tNode->n_Values[i] = tNode->n_Values[i + 1];
		tNode->n_Nodes[i + 1] = tNode->n_Nodes[i + 2];
	}
	tNode->n_Size--;
	return true;

}

// Delete the value and the node pointer on the left
template <typename T>
bool deleteValueLeftNode(TreeNode<T>* tNode, UInt location)
{
	if (location >= tNode->n_Size)
	{
		return false;
	}
	for (UInt i = location; i < tNode->n_Size - 1; i++)
	{
		tNode->n_Values[i] = tNode->n_Values[i + 1];
		tNode->n_Nodes[i] = tNode->n_Nodes[i + 1];
	}
	tNode->n_Nodes[tNode->n_Size - 1] = tNode->n_Nodes[tNode->n_Size];
	tNode->n_Size--;
	return true;
}

// Return true if all node pointers under the current node are nullptr
template <typename T>
bool allNodePtrsNull(TreeNode<T>* tNode)
{
	bool allNull = true;
	for (UInt i = 0; i < tNode->n_Size + 1; i++)
	{
		allNull = allNull && (tNode->n_Nodes[i] == nullptr);
	}
	return allNull;
}

// Return true if all the node pointers under the current node are not nullptr
template <typename T>
bool allNodesNotNull(TreeNode<T> * tNode)
{
	bool allNotNull = true;
	for (UInt i = 0; i < tNode->n_Size + 1; i++)
	{
		allNotNull = allNotNull && (tNode->n_Nodes[i] != nullptr);
	}
	return allNotNull;
}

// Find the exact location of the value in the node 
template <typename T>
bool findValueLocation(TreeNode<T>* tNode, T value, UInt* pLocation)
{
	for (UInt i = 0; i < tNode->n_Size; i++)
	{
		if (value == tNode->n_Values[i])
		{
			*pLocation = i;
			return true;
		}
	}
	return false;
}

// Find location of a node pointer in the current node
template <typename T>
bool findNodeLocation(TreeNode<T>* tNode, TreeNode<T>* pNode, UInt* pLocation)
{
	for (UInt i = 0; i < tNode->n_Size + 1; i++)
	{
		if (pNode == tNode->n_Nodes[i])
		{
			*pLocation = i;
			return true;
		}
	}
	return false;
}

// Find the targeted leaf node to insert value, return nullptr for invalid pointer argument
template <typename T>
TreeNode<T>* BTree<T>::findLeafNode(TreeNode<T>* pCurrent, T value)
{
	if (!pCurrent)
	{
		return nullptr;
	}
	if (allNodePtrsNull<T>(pCurrent))
	{
		return pCurrent;
	}
	else if (allNodesNotNull<T>(pCurrent))
	{
		for (UInt i = 0; i < pCurrent->n_Size; i++)
		{
			if (value < pCurrent->n_Values[i])
			{
				return findLeafNode(pCurrent->n_Nodes[i], value);
			}
		}
		return findLeafNode(pCurrent->n_Nodes[pCurrent->n_Size], value);
	}
	else
	{
		return nullptr;
	}
}

// Find the excact node that containts the value, return nullptr if not found
template <typename T>
TreeNode<T>* BTree<T>::findExactNode(TreeNode<T>* pCurrent, T value)
{
	UInt location;
	if (!pCurrent)
	{
		return nullptr;
	}
	if (findValueLocation<T>(pCurrent, value, &location))
	{
		return pCurrent;
	}
	else
	{
		for (UInt i = 0; i < pCurrent->n_Size; i++)
		{
			if (value < pCurrent->n_Values[i])
			{
				return findExactNode(pCurrent->n_Nodes[i], value);
			}
		}
		return findExactNode(pCurrent->n_Nodes[pCurrent->n_Size], value);
	}
}

template <typename T>
TreeNode<T>* BTree<T>::searchNode(T value)
{
	if (t_ListHeads.empty())
	{
		return nullptr;
	}
	return findExactNode(t_ListHeads[t_ListHeads.size() -1], value);
}

template <typename T>
BTree<T>::BTree()
{
	t_MaxBufferSize = MEMORY_SIZE / sizeof(TreeNode<T>);
	t_BufferSize = t_MaxBufferSize;
}

template <typename T>
BTree<T>::~BTree()
{
	for (UInt i = 0; i < t_Buffer.size(); i++)
	{
		delete[] t_Buffer[i];
	}
}

// Get or create a node
template <typename T>
TreeNode<T>* BTree<T>::getEmptyNode()
{
	TreeNode<T>* tnPtr;
	if (!t_DeletedBuffer.empty())
	{
		tnPtr = t_DeletedBuffer.back();
		t_DeletedBuffer.pop_back();
	}
	else
	{
		
		if (t_BufferSize >= t_MaxBufferSize)
		{
			tnPtr = new TreeNode<T>[t_MaxBufferSize];
			for (UInt i = 0; i < NODE_SIZE; i++)
			{
				if (tnPtr)
				{
					break;
				}
				else
				{
					tnPtr = new TreeNode<T>[t_MaxBufferSize];
				}
			}
			if (!tnPtr)
			{
				throw std::bad_alloc();
			}
			t_Buffer.push_back(tnPtr);
			tnPtr = &(tnPtr[0]);
			t_BufferSize = 1;
		}
		else
		{
			tnPtr = t_Buffer.back();
			tnPtr = &(tnPtr[t_BufferSize]);
			t_BufferSize++;
		}
	}
	reset<T>(tnPtr);
	return tnPtr;
}

template <typename T>
bool BTree<T>::insertValue(T value)
{
	if (t_ListHeads.empty())
	{
		TreeNode<T>* temp;
		try
		{
			temp = getEmptyNode();
		}
		catch (std::bad_alloc e)
		{
			throw e;
		}
		t_ListHeads.push_back(temp);
		insertValueToNode<T>(t_ListHeads[t_ListHeads.size() -1], value, nullptr, nullptr);
		t_ListAccessor.push_back(temp);
		return true;
	}

	TreeNode<T>* pLeftInsert = nullptr;
	TreeNode<T>* pRightInsert = nullptr;
	TreeNode<T>* pNode = findLeafNode(t_ListHeads[t_ListHeads.size() -1], value);
	if (insertValueToNode<T>(pNode, value, pLeftInsert, pRightInsert))
	{
		return true;
	}

	while (true)
	{
		TreeNode<T>* nextLNode = pNode->n_NextListNode;
		TreeNode<T>* preNode = pNode->n_PreNode;
		TreeNode<T>* pRightTemp;
		try
		{
			pRightTemp = getEmptyNode();
		}
		catch (std::bad_alloc e)
		{
			throw e;
		}
		UInt midPoint = NODE_SIZE / 2;
		T tempValue = pNode->n_Values[midPoint];
		for (UInt i = 0; i < NODE_SIZE - midPoint - 1; i++)
		{
			pRightTemp->n_Values[i] = pNode->n_Values[midPoint + 1 + i];
			pRightTemp->n_Nodes[i] = pNode->n_Nodes[midPoint + 1 + i];
			if (pRightTemp->n_Nodes[i])
			{
				pRightTemp->n_Nodes[i]->n_PreNode = pRightTemp;
			}
		}
		pRightTemp->n_Nodes[NODE_SIZE - midPoint - 1] = pNode->n_Nodes[NODE_SIZE];
		if (pRightTemp->n_Nodes[NODE_SIZE - midPoint - 1])
		{
			pRightTemp->n_Nodes[NODE_SIZE - midPoint - 1]->n_PreNode = pRightTemp;
		}
		pRightTemp->n_Size = NODE_SIZE - midPoint - 1;
		pNode->n_Size = midPoint;
		if (value < tempValue)
		{
			insertValueToNode<T>(pNode, value, pLeftInsert, pRightInsert);
		}
		else
		{
			insertValueToNode<T>(pRightTemp, value, pLeftInsert, pRightInsert);
		}
		pRightTemp->n_NextListNode = pNode->n_NextListNode;
		pNode->n_NextListNode = pRightTemp;
		value = tempValue;
		pLeftInsert = pNode;
		pRightInsert = pRightTemp;
		pNode = preNode;
		if (!pNode)
		{
			TreeNode<T>* temp;
			try
			{
				temp = getEmptyNode();
			}
			catch (std::bad_alloc e)
			{
				throw e;
			}
			t_ListHeads.push_back(temp);
			insertValueToNode<T>(t_ListHeads[t_ListHeads.size() -1], value, pLeftInsert, pRightInsert);
			t_ListAccessor.push_back(temp);
			return true;
		}
		else if (insertValueToNode(pNode, value, pLeftInsert, pRightInsert))
		{
			return true;
		}
	}
}

template <typename T>
bool BTree<T>::deleteValue(T value)
{
	TreeNode<T>* pNode = searchNode(value);
	if (!pNode)
	{
		return false;
	}
	UInt location;
	if (!findValueLocation<T>(pNode, value, &location))
	{
		return false;
	}
	
	TreeNode<T>* pNext = pNode->n_Nodes[location];
	if (pNext)
	{
		while (pNext->n_Nodes[0])
		{
			pNext = pNext->n_Nodes[pNext->n_Size];
		}
		pNode->n_Values[location] = pNext->n_Values[pNext->n_Size - 1];
		pNext->n_Size--;
		pNode = pNext;
	}
	else
	{
		deleteValueRightNode<T>(pNode, location);
	}

	TreeNode<T>* preNode = pNode->n_PreNode;
	if (!preNode && pNode->n_Size == 0)
	{
		t_DeletedBuffer.push_back(t_ListHeads[t_ListHeads.size() -1]);
		t_ListHeads.pop_back();
		t_ListAccessor.pop_back();
	}
	UInt leftLength, rightLength;
	TreeNode<T>* leftNode,  *rightNode;
	while (preNode)
	{
		if (!findNodeLocation<T>(preNode, pNode, &location))
		{
		}
		if (location == preNode->n_Size || 
			(location != 0 && preNode->n_Nodes[location-1]->n_Size 
				+ preNode->n_Nodes[location]->n_Size < NODE_SIZE))
		{
			leftNode = preNode->n_Nodes[location - 1];
			rightNode = preNode->n_Nodes[location];
			location--;
		}
		else
		{
			leftNode = preNode->n_Nodes[location];
			rightNode = preNode->n_Nodes[location + 1];
		}
		leftLength = leftNode->n_Size;
		rightLength = rightNode->n_Size;

		if (leftLength + rightLength < NODE_SIZE)
		{
			leftNode->n_Values[leftLength] = preNode->n_Values[location];
			for (UInt i = 0; i < rightLength; i++)
			{
				leftNode->n_Values[leftLength + 1 + i] = rightNode->n_Values[i];
				leftNode->n_Nodes[leftLength + 1 + i] = rightNode->n_Nodes[i];
				if (leftNode->n_Nodes[leftLength + 1 + i])
				{
					leftNode->n_Nodes[leftLength + 1 + i]->n_PreNode = leftNode;
				}
			}
			leftLength = leftLength + rightLength + 1;
			leftNode->n_Nodes[leftLength] = rightNode->n_Nodes[rightLength];
			if (leftNode->n_Nodes[leftLength])
			{
				leftNode->n_Nodes[leftLength]->n_PreNode = leftNode;
			}
			leftNode->n_Size = leftLength;
			leftNode->n_NextListNode = rightNode->n_NextListNode;
			t_DeletedBuffer.push_back(rightNode);
			deleteValueRightNode<T>(preNode, location);
			if (!(preNode->n_PreNode) && preNode->n_Size == 0)
			{
				leftNode->n_PreNode = nullptr;
				t_DeletedBuffer.push_back(t_ListHeads[t_ListHeads.size() -1]);
				t_ListHeads.pop_back();
				t_ListAccessor.pop_back();
				return true;
			}
		}
		else if (leftLength == 0)
		{
			TreeNode<T>* temp = leftNode->n_Nodes[leftLength];
			insertValueToNode<T>(leftNode, preNode->n_Values[location], 
				temp, rightNode->n_Nodes[0]);
			preNode->n_Values[location] = rightNode->n_Values[0];
			deleteValueLeftNode<T>(rightNode, (UInt)0);
			return true;
		}
		else if(rightLength == 0)
		{
			TreeNode<T>* temp = rightNode->n_Nodes[rightLength];
			insertValueToNode<T>(rightNode, preNode->n_Values[location],
				leftNode->n_Nodes[leftLength], temp);
			preNode->n_Values[location] = leftNode->n_Values[leftLength - 1];
			deleteValueRightNode<T>(leftNode, leftLength - 1);
			return true;
		}
		else
		{
			return true;
		}
		pNode = preNode;
		preNode = preNode->n_PreNode;
	}
	return true;
}

template <typename T>
UInt BTree<T>::getAllSortedValue(T* pValues, UInt maxSize)
{
	UInt counter = 0;
	UInt listPosition;
	if (t_ListAccessor.empty())
	{
		return 0;
	}
	while (t_ListAccessor[0] && counter < maxSize)
	{
		for (UInt i = 0; i < t_ListAccessor[0]->n_Size; i++)
		{
			if (counter < maxSize)
			{
				pValues[counter] = t_ListAccessor[0]->n_Values[i];
				counter++;
			}
			else
			{
				break;
			}
		}
		listPosition = 1;
		while (listPosition < t_ListAccessor.size() && t_ListAccessor[listPosition] &&
			t_ListAccessor[listPosition]->n_Counter >= t_ListAccessor[listPosition]->n_Size)
		{
			t_ListAccessor[listPosition]->n_Counter = 0;
			t_ListAccessor[listPosition] = t_ListAccessor[listPosition]->n_NextListNode;
			listPosition++;
		}
		if (listPosition < t_ListAccessor.size() && t_ListAccessor[listPosition])
		{
			if (counter < maxSize)
			{
				pValues[counter] = t_ListAccessor[listPosition]->n_Values[
					t_ListAccessor[listPosition]->n_Counter];
				counter++;
				t_ListAccessor[listPosition]->n_Counter++;
			}
			else
			{
				t_ListAccessor[listPosition]->n_Counter = 0;
				break;
			}
		}
		t_ListAccessor[0] = t_ListAccessor[0]->n_NextListNode;
	}
	if (!resetAccessor())
	{
		// Check for error
	}
	return counter;
}

template <typename T>
UInt BTree<T>::getTreeSize()
{
	return (t_Buffer.size() - 1)*t_MaxBufferSize + 
		t_BufferSize - t_DeletedBuffer.size();
}

template <typename T>
bool BTree<T>::resetAccessor()
{
	if (t_ListAccessor.size() != t_ListHeads.size())
	{
		return false;
	}
	for (UInt i = 0; i < t_ListAccessor.size(); i++)
	{
		t_ListAccessor[i] = t_ListHeads[i];
	}
	return true;
}
