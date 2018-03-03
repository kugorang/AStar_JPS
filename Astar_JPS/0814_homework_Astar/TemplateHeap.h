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
	int index = numOfData + 1;	// 새 노드가 저장될 인덱스 값을 index에 저장

	// 새 노드가 저장될 위치가 루트 노드의 위치가 아니라면 while문 반복
	while (index != 1)
	{
		// 새 노드와 부모 노드의 우선순위 비교
		int parentIndex = GetParentIndex(index);

		if (priorityComp(data, heap[parentIndex]) > 0)	// 새 노드의 우선순위 높다면
		{
			// 부모 노드를 한 레벨 내림, 실제로 내림
			heap[index] = heap[parentIndex];

			// 새 노드를 한 레벨 올림, 실제로 올리지는 않고 인덱스 값만 갱신
			index = parentIndex;
		}
		else	// 새 노드의 우선순위가 높지 않다면
		{
			break;
		}
	}

	heap[index] = data;	// 새 노드를 배열에 저장
	numOfData++;
}

template <typename T>
T TemplateHeap<T>::DeleteHeap()
{
	T returnData = heap[1];				// 반환을 위해서 삭제할 데이터 저장	
	T lastElement = heap[numOfData];	// 힙의 마지막 노드 저장

	// 아래의 변수 parentIndex에는 마지막 노드가 저장될 위치정보가 담긴다.
	int parentIndex = 1;	// 루트 노드가 위치해야 할 인덱스 값 저장
	int childIndex;

	// 루트 노드의 우선순위가 높은 자식 노드를 시작으로 반복문 시작
	while (childIndex = GetHighPriorityChildIndex(parentIndex))
	{
		// 마지막 노드와 우선순위 비교
		if (priorityComp(lastElement, heap[childIndex]) >= 0)
		{
			break;	// 마지막 노드의 우선순위가 높으면 반복문 탈출!
		}

		// 마지막 노드보다 우선순위 높으니, 비교대상 노드의 위치를 한 레벨 올림
		heap[parentIndex] = heap[childIndex];
		parentIndex = childIndex;	// 마지막 노드가 저장될 위치정보를 한 레벨 내림
	}	// 반복문 탈출하면 parentIndex에는 마지막 노드의 위치정보가 저장됨

	heap[parentIndex] = lastElement;	// 마지막 노드 최종 저장
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

	// 자식 노드가 존재하지 않는다면,
	if (leftChildIndex > numOfData)
	{
		return 0;
	}
	// 자식 노드가 왼쪽 자식 노드 하나만 존재한다면,
	else if (leftChildIndex == numOfData)
	{
		return leftChildIndex;
	}
	// 자식 노드가 둘 다 존재한다면,
	else
	{
		// 왼쪽 자식 노드의 우선순위가 높다면,
		if (priorityComp(heap[leftChildIndex], heap[rightChildIndex]) > 0)
		{
			return leftChildIndex;	// 왼쪽 자식 노드의 인덱스 값 반환
		}
		// 오른쪽 자식 노드의 우선순위가 높다면,
		else
		{
			return rightChildIndex;	// 오른쪽 자식 노드의 인덱스 값 반환
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