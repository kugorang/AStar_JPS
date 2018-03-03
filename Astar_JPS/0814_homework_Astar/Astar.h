#pragma once

#include "TemplateHeap.h"
#include "TemplateLinkedList.h"

#define WIDTH 38
#define HEIGHT 19
#define BLOCK_UNIT 50

class Astar
{
private:
	COLORREF red, green, blue, gray, white, yellow, black;

	struct Node
	{
		int x, y, index, realX, realY;
		// G : 출발점으로부터의 이동 횟수
		// H : 목적지와의 거리
		// F : G + H
		double g, h, f;
		bool obstacle, openListNode, closeListNode;

		Node* parent;
	};

	Node location[HEIGHT][WIDTH];
	Node *startNode, *endNode;
	
	// 힙에서 쓰이는 우선순위 비교함수
	static double DataPriorityComp(Node* node1, Node* node2);

	TemplateHeap<Node*>* openList;
	TemplateLinkedList<Node*>* closedList;

	bool alreayFinish, searchStart;
public:
	Astar();
	~Astar();

	void Action();
	void Draw(HDC memDC);

	bool GetLocationObstacle(int xPos, int yPos);

	void SetLocationObstacle(int xPos, int yPos, bool makeObstacle);
	void SetLocationStartPos(int pos);
	void SetLocationEndPos(int pos);
	
	void DeleteList();
	void DeleteObstacle();
};