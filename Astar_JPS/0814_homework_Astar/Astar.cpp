#include "stdafx.h"
#include "Astar.h"

Astar::Astar()
{
	red = RGB(255, 0, 0);
	green = RGB(0, 255, 0);
	blue = RGB(0, 0, 255);
	gray = RGB(30, 30, 30);
	white = RGB(255, 255, 255);
	yellow = RGB(255, 255, 0);
	black = RGB(0, 0, 0);

	startNode = nullptr;
	endNode = nullptr;

	openList = new TemplateHeap<Node*>(WIDTH * HEIGHT, DataPriorityComp);
	closedList = new TemplateLinkedList<Node*>();

	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			location[i][j].x = j;
			location[i][j].y = i;
			location[i][j].realX = j * BLOCK_UNIT + BLOCK_UNIT / 2;
			location[i][j].realY = i * BLOCK_UNIT + BLOCK_UNIT / 2;			
			location[i][j].index = i * WIDTH + j;

			location[i][j].g = -1.0;
			location[i][j].h = -1.0;
			location[i][j].f = -1.0;
			
			location[i][j].obstacle = false;
			location[i][j].openListNode = false;
			location[i][j].closeListNode = false;
			location[i][j].parent = nullptr;
		}
	}

	alreayFinish = false;
	searchStart = false;
}

Astar::~Astar()
{
	delete openList;
	delete closedList;

	openList = nullptr;
	closedList = nullptr;
}

void Astar::Action()
{
	if (startNode == nullptr || endNode == nullptr || alreayFinish)
	{
		return;
	}

	if (openList->IsEmpty())
	{
		startNode->g = 0;
		startNode->h = abs(endNode->x - startNode->x) + abs(endNode->y - startNode->y);
		startNode->f = startNode->g + startNode->h;
		startNode->openListNode = true;

		openList->InsertHeap(startNode);
		searchStart = true;
	}
	Node* parentNode = openList->DeleteHeap();

	if (parentNode == endNode)
	{
		alreayFinish = true;
		return;
	}

	if (parentNode == nullptr)
	{
		return;
	}

	closedList->InsertHead(parentNode);
	parentNode->openListNode = false;
	parentNode->closeListNode = true;

	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			bool alreadyInList = false;
			int parentX = parentNode->x;
			int parentY = parentNode->y;

			if ((i == 0 && j == 0) || parentX + j < 0 || parentY + i < 0 ||
				parentX + j > WIDTH - 1 || parentY + i > HEIGHT - 1)
			{
				continue;
			}

			Node* childNode = &location[parentY + i][parentX + j];
			Node openListNode;

			openListNode.parent = nullptr;

			if (openList->SearchData(childNode))
			{
				openListNode = *childNode;
			}
			else
			{
				closedList->MoveHead();

				while (closedList->MoveNext())
				{
					if (closedList->GetAt() == childNode)
					{
						alreadyInList = true;
						break;
					}
				}
			}

			if (childNode->obstacle || alreadyInList)
			{
				continue;
			}

			if ((i == -1 && j == -1) || (i == -1 && j == 1)
				|| (i == 1 && j == -1) || (i == 1 && j == 1))
			{
				childNode->g = parentNode->g + 1.4;
			}
			else // 대각선이 아니면
			{
				childNode->g = parentNode->g + 1;
			}

			if (openListNode.parent != nullptr)
			{
				if (childNode->g > openListNode.g)
				{
					childNode->g = openListNode.g;
					childNode->f = openListNode.g + openListNode.h;
					childNode->parent = openListNode.parent;
				}
				else
				{
					childNode->f = childNode->g + childNode->h;
					childNode->parent = parentNode;
				}

				openList->SortHeap(childNode);
			}
			else // openListNode == nullptr
			{
				childNode->h = abs(endNode->x - childNode->x) + abs(endNode->y - childNode->y);
				childNode->f = childNode->g + childNode->h;
				childNode->parent = parentNode;
				childNode->openListNode = true;

				openList->InsertHeap(childNode);
			}
		}
	}
}

void Astar::Draw(HDC memDC)
{
	WCHAR index[4] = { 0, };
	WCHAR parentIndex[4] = { 0, };

	WCHAR f[8] = { 0, };
	WCHAR g[8] = { 0, };
	WCHAR h[8] = { 0, };
	
	SelectObject(memDC, GetStockObject(DC_BRUSH));
	SelectObject(memDC, GetStockObject(DC_PEN));

	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			if (location[i][j].obstacle)
			{
				SetDCBrushColor(memDC, gray);
			}
			else if (&location[i][j] == startNode)
			{
				SetDCBrushColor(memDC, green);
			}
			else if (&location[i][j] == endNode)
			{
				SetDCBrushColor(memDC, red);
			}
			else if (location[i][j].closeListNode)
			{
				SetDCBrushColor(memDC, blue);
			}
			else if (location[i][j].openListNode)
			{
				SetDCBrushColor(memDC, yellow);
			}
			
			Rectangle(memDC, j * BLOCK_UNIT, i * BLOCK_UNIT, (j + 1) * BLOCK_UNIT - 1, (i + 1) * BLOCK_UNIT - 1);
			
			if (!location[i][j].obstacle
				&& ((&location[i][j] == startNode && location[i][j].openListNode)
				|| (&location[i][j] == endNode && location[i][j].openListNode)
				|| location[i][j].openListNode || location[i][j].closeListNode))
			{
				wsprintf(index, L"%d", location[i][j].index);
				TextOut(memDC, j * BLOCK_UNIT + BLOCK_UNIT / 2, i * BLOCK_UNIT + BLOCK_UNIT / 10 * 3, index, lstrlen(index));

				swprintf_s(f, L"%.1lf", location[i][j].f);
				TextOut(memDC, j * BLOCK_UNIT + BLOCK_UNIT / 5, i * BLOCK_UNIT + BLOCK_UNIT / 40, f, lstrlen(f));

				swprintf_s(g, L"%.1lf", location[i][j].g);
				TextOut(memDC, j * BLOCK_UNIT + BLOCK_UNIT / 5, i * BLOCK_UNIT + BLOCK_UNIT / 5 * 3, g, lstrlen(g));

				swprintf_s(h, L"%.1lf", location[i][j].h);
				TextOut(memDC, j * BLOCK_UNIT + BLOCK_UNIT / 5 * 4, i * BLOCK_UNIT + BLOCK_UNIT / 5 * 3, h, lstrlen(h));

				if (location[i][j].parent != nullptr)
				{
					wsprintf(parentIndex, L"%d", location[i][j].parent->index);
					TextOut(memDC, j * BLOCK_UNIT + BLOCK_UNIT / 5 * 4, i * BLOCK_UNIT + BLOCK_UNIT / 40, parentIndex, lstrlen(parentIndex));
				}
			}
			
			SetDCBrushColor(memDC, white);
		}
	}
	
	if (alreayFinish)
	{
		Node* tmpNode = endNode;

		SetDCPenColor(memDC, red);

		do
		{
			MoveToEx(memDC, tmpNode->realX, tmpNode->realY, NULL);
			LineTo(memDC, tmpNode->parent->realX, tmpNode->parent->realY);

			tmpNode = tmpNode->parent;
		} while (tmpNode != startNode);

	}
}

void Astar::DeleteList()
{
	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			location[i][j].g = -1;
			location[i][j].h = -1;
			location[i][j].f = -1;

			location[i][j].openListNode = false;
			location[i][j].closeListNode = false;
			location[i][j].parent = nullptr;
		}
	}

	openList->ClearHeap();

	closedList->MoveHead();
	closedList->MoveNext();

	Node* tmpNode = nullptr;

	do
	{
		tmpNode = closedList->RemoveAt();
	} while (tmpNode != nullptr);

	alreayFinish = false;
	searchStart = false;
}

void Astar::DeleteObstacle()
{
	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			location[i][j].g = -1;
			location[i][j].h = -1;
			location[i][j].f = -1;

			location[i][j].obstacle = false;
			location[i][j].openListNode = false;
			location[i][j].closeListNode = false;
			location[i][j].parent = nullptr;
		}
	}

	openList->ClearHeap();

	closedList->MoveHead();
	closedList->MoveNext();

	startNode = nullptr;
	endNode = nullptr;

	Node* tmpNode = nullptr;

	do
	{
		tmpNode = closedList->RemoveAt();
	} while (tmpNode != nullptr);

	alreayFinish = false;
	searchStart = false;
}

bool Astar::GetLocationObstacle(int xPos, int yPos)
{
	return location[yPos][xPos].obstacle;
}

void Astar::SetLocationObstacle(int xPos, int yPos, bool makeObstacle)
{
	if (!(alreayFinish || searchStart || &location[yPos][xPos] == startNode || &location[yPos][xPos] == endNode))
	{
		location[yPos][xPos].obstacle = makeObstacle ? true : false;
	}
}

void Astar::SetLocationStartPos(int pos)
{
	int yPos = pos / WIDTH;
	int xPos = pos % WIDTH;

	if (!(alreayFinish || searchStart || location[yPos][xPos].obstacle || &location[yPos][xPos] == endNode))
	{
		startNode = &location[yPos][xPos];
	}
}

void Astar::SetLocationEndPos(int pos)
{
	int yPos = pos / WIDTH;
	int xPos = pos % WIDTH;

	if (!(alreayFinish || searchStart || location[yPos][xPos].obstacle || &location[yPos][xPos] == startNode))
	{
		endNode = &location[yPos][xPos];
	}
}

// 힙에서 쓰이는 우선순위 비교함수
double Astar::DataPriorityComp(Node* f1, Node* f2)
{
	return (f2->f - f1->f);
}