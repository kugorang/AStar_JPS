#pragma once

template <typename T>
class TemplateHeap
{
private:
	typedef double PriorityComp(T d1, T d2);
	
	T* heap;
	PriorityComp* priorityComp;

	int numOfData, heapLen;

	int GetParentIndex(int index);
	int GetLeftChildIndex(int index);
	int GetRightChildIndex(int index);
	int GetHighPriorityChildIndex(int index);	
public:
	TemplateHeap(int heapLen, PriorityComp priorityComp);
	~TemplateHeap();
	
	bool IsEmpty();

	void InsertHeap(T data);
	T DeleteHeap();
	bool SearchData(T data);
	void SortHeap(T data);
	void ClearHeap();

	int GetNumOfData();
};

template <typename T>
TemplateHeap<T>::TemplateHeap(int heapLen, PriorityComp priorityComp)
{
	heap = new T[heapLen];
	memset(heap, 0, heapLen);
	numOfData = 0;
	this->heapLen = heapLen;
	this->priorityComp = priorityComp;
}

template <typename T>
TemplateHeap<T>::~TemplateHeap()
{
	delete heap;
	heap = nullptr;
}

template <typename T>
bool TemplateHeap<T>::IsEmpty()
{
	if (numOfData == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

template <typename T>
void TemplateHeap<T>::InsertHeap(T data)
{
	int index = numOfData + 1;	// �� ��尡 ����� �ε��� ���� index�� ����

	// �� ��尡 ����� ��ġ�� ��Ʈ ����� ��ġ�� �ƴ϶�� while�� �ݺ�
	while (index != 1)
	{
		// �� ���� �θ� ����� �켱���� ��
		int parentIndex = GetParentIndex(index);

		if (priorityComp(data, heap[parentIndex]) > 0)	// �� ����� �켱���� ���ٸ�
		{
			// �θ� ��带 �� ���� ����, ������ ����
			heap[index] = heap[parentIndex];

			// �� ��带 �� ���� �ø�, ������ �ø����� �ʰ� �ε��� ���� ����
			index = parentIndex;
		}
		else	// �� ����� �켱������ ���� �ʴٸ�
		{
			break;
		}
	}

	heap[index] = data;	// �� ��带 �迭�� ����
	numOfData++;
}

template <typename T>
T TemplateHeap<T>::DeleteHeap()
{
	T returnData = heap[1];				// ��ȯ�� ���ؼ� ������ ������ ����	
	T lastElement = heap[numOfData];	// ���� ������ ��� ����

	// �Ʒ��� ���� parentIndex���� ������ ��尡 ����� ��ġ������ ����.
	int parentIndex = 1;	// ��Ʈ ��尡 ��ġ�ؾ� �� �ε��� �� ����
	int childIndex;

	// ��Ʈ ����� �켱������ ���� �ڽ� ��带 �������� �ݺ��� ����
	while (childIndex = GetHighPriorityChildIndex(parentIndex))
	{
		// ������ ���� �켱���� ��
		if (priorityComp(lastElement, heap[childIndex]) >= 0)
		{
			break;	// ������ ����� �켱������ ������ �ݺ��� Ż��!
		}

		// ������ ��庸�� �켱���� ������, �񱳴�� ����� ��ġ�� �� ���� �ø�
		heap[parentIndex] = heap[childIndex];
		parentIndex = childIndex;	// ������ ��尡 ����� ��ġ������ �� ���� ����
	}	// �ݺ��� Ż���ϸ� parentIndex���� ������ ����� ��ġ������ �����

	heap[parentIndex] = lastElement;	// ������ ��� ���� ����
	heap[numOfData] = 0;
	numOfData--;

	return returnData;
}

template <typename T>
bool TemplateHeap<T>::SearchData(T data)
{
	for (int i = 1; i <= numOfData; i++)
	{
		if (data == heap[i])
		{
			return true;
		}
	}

	return false;
}

template <typename T>
void TemplateHeap<T>::SortHeap(T data)
{
	int index = 0;

	for (int i = 1; i <= numOfData; i++)
	{
		if (data == heap[i])
		{
			index = i;
			break;
		}
	}
	
	int parentIndex = GetParentIndex(index);
	int childIndex = GetHighPriorityChildIndex(index);

	if (parentIndex != 0 && priorityComp(data, heap[parentIndex]) > 0)
	{
		while (index != 1)
		{
			parentIndex = GetParentIndex(index);

			if (priorityComp(data, heap[parentIndex]) > 0)
			{
				heap[index] = heap[parentIndex];
				index = parentIndex;
			}
			else
			{
				break;
			}
		}
	}

	if (childIndex != 0 && priorityComp(data, heap[childIndex]) < 0)
	{
		while (childIndex = GetHighPriorityChildIndex(index))
		{
			if (priorityComp(data, heap[childIndex]) >= 0)
			{
				break;
			}

			heap[index] = heap[childIndex];
			index = childIndex;
		}
	}	

	heap[index] = data;
}

template <typename T>
void TemplateHeap<T>::ClearHeap()
{
	memset(heap, 0, heapLen);
	numOfData = 0;
}

template <typename T>
int TemplateHeap<T>::GetHighPriorityChildIndex(int index)
{
	int leftChildIndex = GetLeftChildIndex(index);
	int rightChildIndex = GetRightChildIndex(index);

	// �ڽ� ��尡 �������� �ʴ´ٸ�,
	if (leftChildIndex > numOfData)
	{
		return 0;
	}
	// �ڽ� ��尡 ���� �ڽ� ��� �ϳ��� �����Ѵٸ�,
	else if (leftChildIndex == numOfData)
	{
		return leftChildIndex;
	}
	// �ڽ� ��尡 �� �� �����Ѵٸ�,
	else
	{
		// ���� �ڽ� ����� �켱������ ���ٸ�,
		if (priorityComp(heap[leftChildIndex], heap[rightChildIndex]) > 0)
		{
			return leftChildIndex;	// ���� �ڽ� ����� �ε��� �� ��ȯ
		}
		// ������ �ڽ� ����� �켱������ ���ٸ�,
		else
		{
			return rightChildIndex;	// ������ �ڽ� ����� �ε��� �� ��ȯ
		}
	}
}

template <typename T>
int TemplateHeap<T>::GetParentIndex(int index)
{
	return index / 2;
}

template <typename T>
int TemplateHeap<T>::GetLeftChildIndex(int index)
{
	return index * 2;
}

template <typename T>
int TemplateHeap<T>::GetRightChildIndex(int index)
{
	return GetLeftChildIndex(index) + 1;
}

template <typename T>
int TemplateHeap<T>::GetNumOfData()
{
	return numOfData;
}