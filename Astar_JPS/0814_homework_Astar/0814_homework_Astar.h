#pragma once

#include "resource.h"

#define ASTAR 0
#define JUMPPOINTSEARCH 1

// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK DialogProc(HWND, UINT, WPARAM, LPARAM);

void Update();
void KeyProcess();
void Draw();