#include <iostream>
#include <cstdlib>
#include <chrono>
#include "balance_tree.h"
#include "balance_tree.cpp"

void getAllChar()
{
	char letter;
	do
	{
		std::cin.get(letter);
	} while (letter != '\n');
}

// Bubble sort for testing
void bubbleSort(int* pInput, int* pOutput, UInt size)
{
	UInt position;
	UInt outSize = size;
	int minNum;
	for (UInt i = 0; i < size; i++)
	{
		position = 0;
		minNum = pInput[position];
		for (UInt j = 0; j < outSize; j++)
		{
			if (pInput[j] < minNum)
			{
				minNum = pInput[j];
				position = j;
			}
		}
		pOutput[i] = pInput[position];
		pInput[position] = pInput[outSize - 1];
		outSize--;
	}
}

int main()
{
	BTree<int> tree;
	srand((unsigned)time(nullptr));
	int randNum;
	UInt size = 500000;
	UInt position;
	int *intArray = new int [size];
	int* intSorted = new int[size];
	int *pResult = new int[size];
	UInt resultSize;
	UInt counter = 0;
	for (UInt i = 0; i < size; i++)
	{
		randNum = (int)(rand() % 10);
		if (randNum < 6)
		{
			intArray[counter] = (int)(rand() % 30000);
			try
			{
				tree.insertValue(intArray[counter]);
			}
			catch (std::bad_alloc e)
			{
				std::cout << "Memory erro" << std::endl;
				system("pause");
				return 0;
			}
			counter++;
		}
		else
		{
			if (counter < 1)
			{
				randNum = (int)(rand() % 30000);
				tree.deleteValue(randNum);
			}
			else
			{
				position = (UInt)(rand() % counter);
				tree.deleteValue(intArray[position]);
				randNum = intArray[position];
				if (position >= counter - 1)
				{

				}
				else
				{
					intArray[position] = intArray[counter - 1];
				}
				counter--;
			}
		}
	}
	resultSize = tree.getAllSortedValue(pResult, counter);
	bubbleSort(intArray, intSorted, counter);
	if (counter != resultSize)
	{
		std::cout << "Tree implementation error." << std::endl;
	}
	bool succeeded = true;
	for (UInt i = 0; i < counter; i++)
	{
		succeeded = succeeded && (pResult[i] == intSorted[i]);
		
		if (!succeeded)
		{
			std::cout << "Sizes returned are the same, but tree error." << std::endl;
			break;
		}
	}
	if (succeeded)
	{
		std::cout << "Succeeded!" << std::endl;
	}
	delete[] intArray;
	delete[] intSorted;
	delete[] pResult;
	return 0;
}
