#pragma once

#include "resource.h"

#define ASTAR 0
#define JUMPPOINTSEARCH 1

// �� �ڵ� ��⿡ ��� �ִ� �Լ��� ������ �����Դϴ�.
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK DialogProc(HWND, UINT, WPARAM, LPARAM);

void Update();
void KeyProcess();
void Draw();