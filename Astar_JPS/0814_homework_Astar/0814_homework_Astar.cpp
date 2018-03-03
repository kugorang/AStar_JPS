// 0814_homework_Astar.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "0814_homework_Astar.h"
#include "Astar.h"
#include "JumpPointSearch.h"

#pragma comment(lib, "User32.lib")

// ���� ����:
HINSTANCE hInst;								// ���� �ν��Ͻ��Դϴ�.
HWND hWnd;
RECT windowRect;

WCHAR localeNum[8] = { 0, };

Astar* aStar = new Astar();
JumpPointSearch* jumpPointSearch = new JumpPointSearch();

static int searchMode;

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: ���⿡ �ڵ带 �Է��մϴ�.
	MSG msg;
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY0814_HOMEWORK_ASTAR));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_MY0814_HOMEWORK_ASTAR);
	wcex.lpszClassName = L"0814_homework_Astar";
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	RegisterClassEx(&wcex);

	hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

	hWnd = CreateWindow(L"0814_homework_Astar", L"0814_homework_Astar", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	windowRect.top = 0;
	windowRect.left = 0;
	windowRect.right = 1900;
	windowRect.bottom = 950;

	AdjustWindowRect(&windowRect, GetWindowStyle(hWnd), GetMenu(hWnd) != NULL);

	MoveWindow(hWnd, (GetSystemMetrics(SM_CXSCREEN) - 1920) / 2,
		(GetSystemMetrics(SM_CYSCREEN) - 1050) / 2, windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top, TRUE);
	
	// �⺻ �޽��� �����Դϴ�.
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			Update();
		}
	}

	return (int) msg.wParam;
}

//
//  �Լ�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ����:  �� â�� �޽����� ó���մϴ�.
//
//  WM_COMMAND	- ���� ���α׷� �޴��� ó���մϴ�.
//  WM_PAINT	- �� â�� �׸��ϴ�.
//  WM_DESTROY	- ���� �޽����� �Խ��ϰ� ��ȯ�մϴ�.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HMENU hmenu;         // handle to main menu

	PAINTSTRUCT ps;
	HDC hdc;
	static bool makeObstacle;
	int xPos, yPos;

	switch (message)
	{
	case WM_CREATE:
		hmenu = GetMenu(hWnd);

		searchMode = JUMPPOINTSEARCH;

		CheckMenuItem(hmenu, ID_AStar, MF_UNCHECKED);
		CheckMenuItem(hmenu, ID_JumpPointSearch, MF_CHECKED);
		break;
	case WM_COMMAND:
		// �޴� ������ ���� �м��մϴ�.
		switch (LOWORD(wParam))
		{
		case ID_DELETELIST:
			if (searchMode == ASTAR)
			{
				aStar->DeleteList();
			}
			else if (searchMode == JUMPPOINTSEARCH)
			{
				jumpPointSearch->DeleteList();
			}			
			break;
		case ID_DELETEOBSTACLE:
			if (searchMode == ASTAR)
			{
				aStar->DeleteObstacle();
			}
			else if (searchMode == JUMPPOINTSEARCH)
			{
				jumpPointSearch->DeleteObstacle();
			}
			break;
		case ID_AStar:
			searchMode = ASTAR;
			CheckMenuItem(hmenu, ID_AStar, MF_CHECKED);
			CheckMenuItem(hmenu, ID_JumpPointSearch, MF_UNCHECKED);
			break;
		case ID_JumpPointSearch:
			searchMode = JUMPPOINTSEARCH;
			CheckMenuItem(hmenu, ID_AStar, MF_UNCHECKED);
			CheckMenuItem(hmenu, ID_JumpPointSearch, MF_CHECKED);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_LBUTTONDOWN:
		if (GetAsyncKeyState(VK_CONTROL) & 0x8000)		// ��Ʈ�� Ű
		{
			xPos = GET_X_LPARAM(lParam) / BLOCK_UNIT;
			yPos = GET_Y_LPARAM(lParam) / BLOCK_UNIT;

			if (searchMode == ASTAR)
			{
				makeObstacle = aStar->GetLocationObstacle(xPos, yPos);
			}
			else if (searchMode == JUMPPOINTSEARCH)
			{
				makeObstacle = jumpPointSearch->GetLocationObstacle(xPos, yPos);
			}
			
			makeObstacle = makeObstacle == true ? false : true;
		}
		else
		{
			xPos = GET_X_LPARAM(lParam) / BLOCK_UNIT;
			yPos = GET_Y_LPARAM(lParam) / BLOCK_UNIT;
			
			if (searchMode == ASTAR)
			{
				aStar->SetLocationStartPos(xPos + yPos * WIDTH);
			}
			else if (searchMode == JUMPPOINTSEARCH)
			{
				jumpPointSearch->SetLocationStartPos(xPos + yPos * WIDTH);
			}			
		}
		break;
	case WM_RBUTTONDOWN:
		xPos = GET_X_LPARAM(lParam) / BLOCK_UNIT;
		yPos = GET_Y_LPARAM(lParam) / BLOCK_UNIT;

		if (searchMode == ASTAR)
		{
			aStar->SetLocationEndPos(xPos + yPos * WIDTH);
		}
		else if (searchMode == JUMPPOINTSEARCH)
		{
			jumpPointSearch->SetLocationEndPos(xPos + yPos * WIDTH);
		}		
		break;
	case WM_MOUSEMOVE:
		if (IsLButtonDown() && GetAsyncKeyState(VK_CONTROL) & 0x8000)
		{
			xPos = GET_X_LPARAM(lParam) / BLOCK_UNIT;
			yPos = GET_Y_LPARAM(lParam) / BLOCK_UNIT;

			if (searchMode == ASTAR)
			{
				aStar->SetLocationObstacle(xPos, yPos, makeObstacle);
			}
			else if (searchMode == JUMPPOINTSEARCH)
			{
				jumpPointSearch->SetLocationObstacle(xPos, yPos, makeObstacle);
			}			
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		delete aStar;
		delete jumpPointSearch;
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void Update()
{
	KeyProcess();

	Draw();

	Sleep(1000 / 60);
}

void KeyProcess()
{
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)		// �����̽� ��
	{
		if (searchMode == ASTAR)
		{
			aStar->Action();
		}
		else if (searchMode == JUMPPOINTSEARCH)
		{
			jumpPointSearch->Action();
		}		
	}
}

void Draw()
{
	HDC hdc = GetDC(hWnd);
	HDC memDC = CreateCompatibleDC(hdc);
	HFONT newFont, oldFont;

	HBITMAP memBitmap = CreateCompatibleBitmap(hdc, windowRect.right, windowRect.bottom);
	HBITMAP oldMemBitmap = SelectBitmap(memDC, memBitmap);

	PatBlt(memDC, 0, 0, windowRect.right, windowRect.bottom, WHITENESS);

	SetTextAlign(memDC, TA_CENTER);

	newFont = CreateFont(12, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, 0, L"���� ���");
	oldFont = SelectFont(memDC, newFont);

	if (searchMode == ASTAR)
	{
		aStar->Draw(memDC);
	}
	else if (searchMode == JUMPPOINTSEARCH)
	{
		jumpPointSearch->Draw(memDC);
	}
	
	BitBlt(hdc, 0, 0, windowRect.right, windowRect.bottom, memDC, 0, 0, SRCCOPY);
	
	DeleteFont(SelectFont(memDC, oldFont));

	DeleteBitmap(SelectBitmap(memDC, oldMemBitmap));
	ReleaseDC(hWnd, memDC);
	DeleteDC(memDC);

	ReleaseDC(hWnd, hdc);
}