#include "stdafx.h"
#include "JumpPointSearch.h"
#include <math.h>

JumpPointSearch::JumpPointSearch()
{
	red = RGB(255, 0, 0);
	green = RGB(0, 255, 0);
	blue = RGB(0, 0, 255);
	white = RGB(255, 255, 255);
	yellow = RGB(255, 255, 0);
	black = RGB(0, 0, 0);

	for (int i = 0; i < 100; i++)
	{
		do
		{
			jumpNode[i] = RGB(rand() % 120 + 100, rand() % 120 + 100, rand() % 120 + 100);
		} while (jumpNode[i] == red || jumpNode[i] == green || jumpNode[i] == blue
			|| jumpNode[i] == white || jumpNode[i] == yellow || jumpNode[i] == black);		
	}

	jumpNodeColorNum = 0;
	
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
			location[i][j].jumpNodeColorNum = -1;

			location[i][j].index = i * WIDTH + j;

			location[i][j].g = -1.0;
			location[i][j].h = -1.0;
			location[i][j].f = -1.0;

			location[i][j].obstacle = false;
			location[i][j].openListNode = false;
			location[i][j].closeListNode = false;
			location[i][j].jumpNode = false;
			location[i][j].parent = nullptr;
		}
	}

	for (int i = 0; i < 8; i++)
	{
		nodeDirection[i] = nullptr;
	}

	alreayFinish = false;
	searchStart = false;
}

JumpPointSearch::~JumpPointSearch()
{
	delete openList;
	delete closedList;

	openList = nullptr;
	closedList = nullptr;
}

void JumpPointSearch::Action()
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
	
	if (parentNode == startNode)
	{
		if (parentNode->x != 0 && parentNode->y != 0)
		{
			nodeDirection[DIRECTION_LU] = Jump(&location[parentNode->y - 1][parentNode->x - 1], DIRECTION_LU);
		}
		
		if (parentNode->x != 0)
		{
			nodeDirection[DIRECTION_LL] = Jump(&location[parentNode->y][parentNode->x - 1], DIRECTION_LL);
		}
		
		if (parentNode->x != 0 && parentNode->y != HEIGHT - 1)
		{
			nodeDirection[DIRECTION_LD] = Jump(&location[parentNode->y + 1][parentNode->x - 1], DIRECTION_LD);
		}
		
		if (parentNode->x != WIDTH - 1 && parentNode->y != 0)
		{
			nodeDirection[DIRECTION_RU] = Jump(&location[parentNode->y - 1][parentNode->x + 1], DIRECTION_RU);
		}
		
		if (parentNode->x != WIDTH - 1)
		{
			nodeDirection[DIRECTION_RR] = Jump(&location[parentNode->y][parentNode->x + 1], DIRECTION_RR);
		}
		
		if (parentNode->x != WIDTH - 1 && parentNode->y != HEIGHT - 1)
		{
			nodeDirection[DIRECTION_RD] = Jump(&location[parentNode->y + 1][parentNode->x + 1], DIRECTION_RD);
		}
		
		if (parentNode->y != 0)
		{
			nodeDirection[DIRECTION_UU] = Jump(&location[parentNode->y - 1][parentNode->x], DIRECTION_UU);
		}
		
		if (parentNode->y != HEIGHT - 1)
		{
			nodeDirection[DIRECTION_DD] = Jump(&location[parentNode->y + 1][parentNode->x], DIRECTION_DD);
		}		
	}
	else // parentNode != startNode
	{
		int posX, posY, direction;

		if (parentNode->parent->x > parentNode->x) // 자식이 부모의 왼쪽에 있다.
		{
			posX = -1;
		}
		else if (parentNode->parent->x < parentNode->x)	// 자식이 부모의 오른쪽에 있다.
		{
			posX = 1;
		}
		else // parentNode->parent->x == parentNode->x, 자식과 부모의 x 좌표가 같다.
		{
			posX = 0;
		}

		if (parentNode->parent->y > parentNode->y)	// 자식이 부모보다 위에 있다.
		{
			posY = -1;
		}
		else if (parentNode->parent->y < parentNode->y)	// 자식이 부모보다 아래에 있다.
		{
			posY = 1;
		}
		else // parentNode->parent->y == parentNode->y, 자식과 부모의 y  좌표가 같다.
		{
			posY = 0;
		}

		switch (posX + posY)
		{
		case -2:
			direction = DIRECTION_LU;
			break;
		case -1:
			direction = posY == -1 ? DIRECTION_UU : DIRECTION_LL;
			break;
		case 0:
			direction = posY == -1 ? DIRECTION_RU : DIRECTION_LD;
			break;
		case 1:
			direction = posY == 0 ? DIRECTION_RR : DIRECTION_DD;
			break;
		case 2:
			direction = DIRECTION_RD;
			break;
		}

		switch (direction)
		{
		case DIRECTION_LU:
			if (parentNode->x != 0)
			{
				nodeDirection[DIRECTION_LL] = Jump(&location[parentNode->y][parentNode->x - 1], DIRECTION_LL);
			}

			if (parentNode->y != 0)
			{
				nodeDirection[DIRECTION_UU] = Jump(&location[parentNode->y - 1][parentNode->x], DIRECTION_UU);
			}

			if (parentNode->x != 0 && parentNode->y != 0)
			{
				nodeDirection[DIRECTION_LU] = Jump(&location[parentNode->y - 1][parentNode->x - 1], DIRECTION_LU);
			}

			if (parentNode->x != 0 && parentNode->y != HEIGHT - 1 && location[parentNode->y + 1][parentNode->x].obstacle)
			{
				nodeDirection[DIRECTION_LD] = Jump(&location[parentNode->y + 1][parentNode->x - 1], DIRECTION_LD);
			}

			if (parentNode->x != WIDTH - 1 && parentNode->y != 0 && location[parentNode->y][parentNode->x + 1].obstacle)
			{
				nodeDirection[DIRECTION_RU] = Jump(&location[parentNode->y - 1][parentNode->x + 1], DIRECTION_RU);
			}
			break;
		case DIRECTION_LL:
			if (parentNode->x != 0)
			{
				nodeDirection[DIRECTION_LL] = Jump(&location[parentNode->y][parentNode->x - 1], DIRECTION_LL);
			}

			if (parentNode->x != 0 && parentNode->y != 0 && location[parentNode->y - 1][parentNode->x].obstacle)
			{
				nodeDirection[DIRECTION_LU] = Jump(&location[parentNode->y - 1][parentNode->x - 1], DIRECTION_LU);
			}

			if (parentNode->x != 0 && parentNode->y != HEIGHT - 1 && location[parentNode->y + 1][parentNode->x].obstacle)
			{
				nodeDirection[DIRECTION_LD] = Jump(&location[parentNode->y + 1][parentNode->x - 1], DIRECTION_LD);
			}
			break;
		case DIRECTION_LD:
			if (parentNode->x != 0)
			{
				nodeDirection[DIRECTION_LL] = Jump(&location[parentNode->y][parentNode->x - 1], DIRECTION_LL);
			}

			if (parentNode->y != HEIGHT - 1)
			{
				nodeDirection[DIRECTION_DD] = Jump(&location[parentNode->y + 1][parentNode->x], DIRECTION_DD);
			}

			if (parentNode->x != 0 && parentNode->y != HEIGHT - 1)
			{
				nodeDirection[DIRECTION_LD] = Jump(&location[parentNode->y + 1][parentNode->x - 1], DIRECTION_LD);
			}

			if (parentNode->x != 0 && parentNode->y != 0 && location[parentNode->y - 1][parentNode->x].obstacle)
			{
				nodeDirection[DIRECTION_LU] = Jump(&location[parentNode->y - 1][parentNode->x - 1], DIRECTION_LU);
			}

			if (parentNode->x != WIDTH - 1 && parentNode->y != HEIGHT - 1 && location[parentNode->y][parentNode->x + 1].obstacle)
			{
				nodeDirection[DIRECTION_RD] = Jump(&location[parentNode->y + 1][parentNode->x + 1], DIRECTION_RD);
			}
			break;
		case DIRECTION_RU:
			if (parentNode->x != WIDTH - 1)
			{
				nodeDirection[DIRECTION_RR] = Jump(&location[parentNode->y][parentNode->x + 1], DIRECTION_RR);
			}

			if (parentNode->y != 0)
			{
				nodeDirection[DIRECTION_UU] = Jump(&location[parentNode->y - 1][parentNode->x], DIRECTION_UU);
			}

			if (parentNode->x != WIDTH - 1 && parentNode->y != 0)
			{
				nodeDirection[DIRECTION_RU] = Jump(&location[parentNode->y - 1][parentNode->x + 1], DIRECTION_RU);
			}

			if (parentNode->x != 0 && parentNode->y != 0 && location[parentNode->y][parentNode->x - 1].obstacle)
			{
				nodeDirection[DIRECTION_LU] = Jump(&location[parentNode->y - 1][parentNode->x - 1], DIRECTION_LU);
			}

			if (parentNode->x != WIDTH - 1 && parentNode->y != HEIGHT - 1 && location[parentNode->y + 1][parentNode->x].obstacle)
			{
				nodeDirection[DIRECTION_RD] = Jump(&location[parentNode->y + 1][parentNode->x + 1], DIRECTION_RD);
			}
			break;
		case DIRECTION_RR:
			if (parentNode->x != WIDTH - 1)
			{
				nodeDirection[DIRECTION_RR] = Jump(&location[parentNode->y][parentNode->x + 1], DIRECTION_RR);
			}			

			if (parentNode->x != WIDTH - 1 && parentNode->y != 0 && location[parentNode->y - 1][parentNode->x].obstacle)
			{
				nodeDirection[DIRECTION_RU] = Jump(&location[parentNode->y - 1][parentNode->x + 1], DIRECTION_RU);
			}

			if (parentNode->x != WIDTH - 1 && parentNode->y != HEIGHT - 1 && location[parentNode->y + 1][parentNode->x].obstacle)
			{
				nodeDirection[DIRECTION_RD] = Jump(&location[parentNode->y + 1][parentNode->x + 1], DIRECTION_RD);
			}
			break;
		case DIRECTION_RD:
			if (parentNode->x != WIDTH - 1)
			{
				nodeDirection[DIRECTION_RR] = Jump(&location[parentNode->y][parentNode->x + 1], DIRECTION_RR);
			}
			
			if (parentNode->y != HEIGHT - 1)
			{
				nodeDirection[DIRECTION_DD] = Jump(&location[parentNode->y + 1][parentNode->x], DIRECTION_DD);
			}
			
			if (parentNode->x != WIDTH - 1 && parentNode->y != HEIGHT - 1)
			{
				nodeDirection[DIRECTION_RD] = Jump(&location[parentNode->y + 1][parentNode->x + 1], DIRECTION_RD);
			}			

			if (parentNode->x != WIDTH - 1 && parentNode->y != 0 && location[parentNode->y - 1][parentNode->x].obstacle)
			{
				nodeDirection[DIRECTION_RU] = Jump(&location[parentNode->y - 1][parentNode->x + 1], DIRECTION_RU);
			}

			if (parentNode->x != 0 && parentNode->y != HEIGHT - 1 && location[parentNode->y][parentNode->x - 1].obstacle)
			{
				nodeDirection[DIRECTION_LD] = Jump(&location[parentNode->y + 1][parentNode->x - 1], DIRECTION_LD);
			}
			break;
		case DIRECTION_UU:
			if (parentNode->y != 0)
			{
				nodeDirection[DIRECTION_UU] = Jump(&location[parentNode->y - 1][parentNode->x], DIRECTION_UU);
			}			

			if (parentNode->x != 0 && parentNode->y != 0 && location[parentNode->y][parentNode->x - 1].obstacle)
			{
				nodeDirection[DIRECTION_LU] = Jump(&location[parentNode->y - 1][parentNode->x - 1], DIRECTION_LU);
			}

			if (parentNode->x != WIDTH - 1 && parentNode->y != 0 && location[parentNode->y][parentNode->x + 1].obstacle)
			{
				nodeDirection[DIRECTION_RU] = Jump(&location[parentNode->y - 1][parentNode->x + 1], DIRECTION_RU);
			}
			break;
		case DIRECTION_DD:
			if (parentNode->y != HEIGHT - 1)
			{
				nodeDirection[DIRECTION_DD] = Jump(&location[parentNode->y + 1][parentNode->x], DIRECTION_DD);
			}			

			if (parentNode->x != 0 && parentNode->y != HEIGHT - 1 && location[parentNode->y][parentNode->x - 1].obstacle)
			{
				nodeDirection[DIRECTION_LD] = Jump(&location[parentNode->y + 1][parentNode->x - 1], DIRECTION_LD);
			}

			if (parentNode->x != WIDTH - 1 && parentNode->y != HEIGHT - 1 && location[parentNode->y][parentNode->x + 1].obstacle)
			{
				nodeDirection[DIRECTION_RD] = Jump(&location[parentNode->y + 1][parentNode->x + 1], DIRECTION_RD);
			}
			break;
		}
	}

	Node openListNode;

	openListNode.parent = nullptr;

	for (int i = 0; i < 8; i++)
	{
		if (nodeDirection[i] != nullptr)
		{
			bool alreadyInList = false;

			if (openList->SearchData(nodeDirection[i]))
			{
				openListNode = *nodeDirection[i];
			}
			else
			{
				closedList->MoveHead();

				while (closedList->MoveNext())
				{
					if (closedList->GetAt() == nodeDirection[i])
					{
						alreadyInList = true;
						break;
					}
				}
			}

			if (alreadyInList)
			{
				continue;
			}

			if (parentNode == startNode)
			{
				nodeDirection[i]->g = sqrt((double)(abs(nodeDirection[i]->x - startNode->x) + abs(nodeDirection[i]->y - startNode->x)));
			}
			else
			{
				nodeDirection[i]->g = parentNode->g + ((double)(abs(nodeDirection[i]->x - parentNode->x) + abs(nodeDirection[i]->y - parentNode->x)));
			}
			

			if (openListNode.parent != nullptr)
			{
				if (nodeDirection[i]->g > openListNode.g)
				{
					nodeDirection[i]->g = openListNode.g;
					nodeDirection[i]->f = openListNode.g + openListNode.h;
					nodeDirection[i]->parent = openListNode.parent;
				}
				else
				{
					nodeDirection[i]->f = nodeDirection[i]->g + nodeDirection[i]->h;
					nodeDirection[i]->parent = parentNode;
				}

				openList->SortHeap(nodeDirection[i]);
				openListNode.parent = nullptr;
			}
			else
			{
				nodeDirection[i]->h = abs(nodeDirection[i]->x - endNode->x) + abs(nodeDirection[i]->y - endNode->x);
				nodeDirection[i]->f = nodeDirection[i]->g + nodeDirection[i]->h;
				nodeDirection[i]->parent = parentNode;
				nodeDirection[i]->openListNode = true;

				openList->InsertHeap(nodeDirection[i]);
			}

			nodeDirection[i] = nullptr;
		}
	}

 	jumpNodeColorNum++;

	if (jumpNodeColorNum == 100)
	{
		jumpNodeColorNum = 0;
	}

	Sleep(100);
}

JumpPointSearch::Node* JumpPointSearch::Jump(Node* node, int direction)
{
	if (node->obstacle)
	{
		return nullptr;
	}

	if (node == endNode)
	{
		return node;
	}

	node->jumpNode = true;
	node->jumpNodeColorNum = jumpNodeColorNum;
	
	Node* returnNode = nullptr;

	switch (direction)
	{
	case DIRECTION_LU:
		if ((node->x != 0 && node->y != HEIGHT - 1 && location[node->y + 1][node->x].obstacle && !location[node->y + 1][node->x - 1].obstacle)
			|| (node->x != WIDTH - 1 && node->y != 0 && location[node->y][node->x + 1].obstacle && !location[node->y - 1][node->x + 1].obstacle))
		{
			return node;
		}

		if (node->x != 0)
		{
			returnNode = Jump(&location[node->y][node->x - 1], DIRECTION_LL);
		}

		if (returnNode != nullptr)
		{
			returnNode = node;
		}
		else if (node->y != 0)
		{
			returnNode = Jump(&location[node->y - 1][node->x], DIRECTION_UU);

			if (returnNode != nullptr)
			{
				returnNode = node;
			}
			else if (node->x != 0 && node->y != 0)
			{
				returnNode = Jump(&location[node->y - 1][node->x - 1], DIRECTION_LU);
			}
		}		

		break;
	case DIRECTION_LL:
		if ((node->x != 0 && node->y != 0 && location[node->y - 1][node->x].obstacle && !location[node->y - 1][node->x - 1].obstacle)
			|| (node->x != 0 && node->y != HEIGHT - 1 && location[node->y + 1][node->x].obstacle && !location[node->y + 1][node->x - 1].obstacle))
		{
			return node;
		}

		if (node->x != 0)
		{
			returnNode = Jump(&location[node->y][node->x - 1], DIRECTION_LL);
		}
		break;
	case DIRECTION_LD:
		if ((node->x != 0 && node->y != 0 && location[node->y - 1][node->x].obstacle && !location[node->y - 1][node->x - 1].obstacle)
			|| (node->x != WIDTH - 1 && node->y != HEIGHT - 1 && location[node->y][node->x + 1].obstacle && !location[node->y + 1][node->x + 1].obstacle))
		{
			return node;
		}

		if (node->x != 0)
		{
			returnNode = Jump(&location[node->y][node->x - 1], DIRECTION_LL);
		}

		if (returnNode != nullptr)
		{
			returnNode = node;
		}		
		else if (node->y != HEIGHT - 1)
		{
			returnNode = Jump(&location[node->y + 1][node->x], DIRECTION_DD);

			if (returnNode != nullptr)
			{
				returnNode = node;
			}
			else if (node->x != 0 && node->y != HEIGHT - 1)
			{
				returnNode = Jump(&location[node->y + 1][node->x - 1], DIRECTION_LD);
			}
		}
		break;
	case DIRECTION_RU:
		if ((node->x != 0 && node->y != 0 && location[node->y][node->x - 1].obstacle && !location[node->y - 1][node->x - 1].obstacle)
			|| (node->x != WIDTH - 1 && node->y != HEIGHT - 1 && location[node->y + 1][node->x].obstacle && !location[node->y + 1][node->x + 1].obstacle))
		{
			return node;
		}

		if (node->x != WIDTH - 1)
		{
			returnNode = Jump(&location[node->y][node->x + 1], DIRECTION_RR);
		}
		
		if (returnNode != nullptr)
		{
			returnNode = node;
		}
		else if (node->y != 0)
		{
			returnNode = Jump(&location[node->y - 1][node->x], DIRECTION_UU);

			if (returnNode != nullptr)
			{
				returnNode = node;
			}
			else if (node->x != WIDTH - 1 && node->y != 0)
			{
				returnNode = Jump(&location[node->y - 1][node->x + 1], DIRECTION_RU);
			}
		}
		break;
	case DIRECTION_RR:
		if ((node->x != WIDTH - 1 && node->y != 0 && location[node->y - 1][node->x].obstacle && !location[node->y - 1][node->x + 1].obstacle)
			|| (node->x != WIDTH - 1 && node->y != HEIGHT - 1 && location[node->y + 1][node->x].obstacle && !location[node->y + 1][node->x + 1].obstacle))
		{
			return node;
		}

		if (node->x != WIDTH - 1)
		{
			returnNode = Jump(&location[node->y][node->x + 1], DIRECTION_RR);
		}
		break;
	case DIRECTION_RD:
		if ((node->x != WIDTH - 1 && node->y != 0 && location[node->y - 1][node->x].obstacle && !location[node->y - 1][node->x + 1].obstacle)
			|| (node->x != 0 && node->y != HEIGHT - 1 && location[node->y][node->x - 1].obstacle && !location[node->y + 1][node->x - 1].obstacle))
		{
			return node;
		}

		if (node->x != WIDTH - 1)
		{
			returnNode = Jump(&location[node->y][node->x + 1], DIRECTION_RR);
		}

		if (returnNode != nullptr)
		{
			returnNode = node;
		}
		else if (node->y != HEIGHT - 1)
		{
			returnNode = Jump(&location[node->y + 1][node->x], DIRECTION_DD);

			if (returnNode != nullptr)
			{
				returnNode = node;
			}
			else if (node->x != WIDTH - 1 && node->y != HEIGHT - 1)
			{
				returnNode = Jump(&location[node->y + 1][node->x + 1], DIRECTION_RD);
			}
		}
		break;
	case DIRECTION_UU:
		if ((node->x != 0 && node->y != 0 && location[node->y][node->x - 1].obstacle && !location[node->y - 1][node->x - 1].obstacle)
			|| (node->x != WIDTH - 1 && node->y != 0 && location[node->y][node->x + 1].obstacle && !location[node->y - 1][node->x + 1].obstacle))
		{
			return node;
		}

		if (node->y != 0)
		{
			returnNode = Jump(&location[node->y - 1][node->x], DIRECTION_UU);
		}
		break;
	case DIRECTION_DD:
		if ((node->x != 0 && node->y != HEIGHT - 1 && location[node->y][node->x - 1].obstacle && !location[node->y + 1][node->x - 1].obstacle)
			|| (node->x != WIDTH - 1 && node->y != HEIGHT - 1 && location[node->y][node->x + 1].obstacle && !location[node->y + 1][node->x + 1].obstacle))
		{
			return node;
		}

		if (node->y != HEIGHT - 1)
		{
			returnNode = Jump(&location[node->y + 1][node->x], DIRECTION_DD);
		}
		break;
	}

	return returnNode;
}

void JumpPointSearch::Draw(HDC memDC)
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
				SetDCBrushColor(memDC, black);
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
			else if (location[i][j].jumpNode)
			{
				SetDCBrushColor(memDC, jumpNode[location[i][j].jumpNodeColorNum]);
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

void JumpPointSearch::DeleteList()
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
			location[i][j].jumpNode = false;
		}
	}

	for (int i = 0; i < 8; i++)
	{
		nodeDirection[i] = nullptr;
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

void JumpPointSearch::DeleteObstacle()
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
			location[i][j].jumpNode = false;
		}
	}

	for (int i = 0; i < 8; i++)
	{
		nodeDirection[i] = nullptr;
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

bool JumpPointSearch::GetLocationObstacle(int xPos, int yPos)
{
	return location[yPos][xPos].obstacle;
}

void JumpPointSearch::SetLocationObstacle(int xPos, int yPos, bool makeObstacle)
{
	if (!(alreayFinish || searchStart || &location[yPos][xPos] == startNode || &location[yPos][xPos] == endNode))
	{
		location[yPos][xPos].obstacle = makeObstacle ? true : false;
	}
}

void JumpPointSearch::SetLocationStartPos(int pos)
{
	int yPos = pos / WIDTH;
	int xPos = pos % WIDTH;

	if (!(alreayFinish || searchStart || location[yPos][xPos].obstacle || &location[yPos][xPos] == endNode))
	{
		startNode = &location[yPos][xPos];
	}
}

void JumpPointSearch::SetLocationEndPos(int pos)
{
	int yPos = pos / WIDTH;
	int xPos = pos % WIDTH;

	if (!(alreayFinish || searchStart || location[yPos][xPos].obstacle || &location[yPos][xPos] == startNode))
	{
		endNode = &location[yPos][xPos];
	}
}

// 힙에서 쓰이는 우선순위 비교함수
double JumpPointSearch::DataPriorityComp(Node* f1, Node* f2)
{
	return (f2->f - f1->f);
}