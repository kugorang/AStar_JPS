#pragma once

#include "TemplateHeap.h"
#include "TemplateLinkedList.h"

#define WIDTH 38
#define HEIGHT 19
#define BLOCK_UNIT 50

#define DIRECTION_LU 0
#define DIRECTION_LL 1
#define DIRECTION_LD 2
#define DIRECTION_RU 3
#define DIRECTION_RR 4
#define DIRECTION_RD 5
#define DIRECTION_UU 6
#define DIRECTION_DD 7

class JumpPointSearch
{
private:
	COLORREF red, green, blue, white, yellow, black;
	COLORREF jumpNode[100];

	int jumpNodeColorNum;

	struct Node
	{
		int x, y, index, realX, realY, jumpNodeColorNum;
		// G : 출발점으로부터의 이동 횟수
		// H : 목적지와의 거리
		// F : G + H
		double g, h, f;
		bool obstacle, openListNode, closeListNode, jumpNode;

		Node* parent;
	};

	Node location[HEIGHT][WIDTH];
	Node *startNode, *endNode;
	Node *nodeDirection[8];

	// 힙에서 쓰이는 우선순위 비교함수
	static double DataPriorityComp(Node* node1, Node* node2);

	TemplateHeap<Node*>* openList;
	TemplateLinkedList<Node*>* closedList;

	bool alreayFinish, searchStart;
public:
	JumpPointSearch();
	~JumpPointSearch();

	void Action();
	Node* Jump(Node* node, int direction);
	void Draw(HDC memDC);

	bool GetLocationObstacle(int xPos, int yPos);

	void SetLocationObstacle(int xPos, int yPos, bool makeObstacle);
	void SetLocationStartPos(int pos);
	void SetLocationEndPos(int pos);

	void DeleteList();
	void DeleteObstacle();
};