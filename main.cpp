#include <windows.h>
#include "resource.h"

#define TITLE TEXT("Kitty on your lap")
#define MDI_FRAME TEXT("FRAMEWINDOW")
#define MDI_CHILD TEXT("MDICHILD")

#define ID_CHILDWND 0x100

HINSTANCE hIns;
HWND hClient;

LRESULT CALLBACK FrameProc(HWND hWnd , UINT msg , WPARAM wp , LPARAM lp) {
	HDC hdc;
	PAINTSTRUCT ps;
	CLIENTCREATESTRUCT ccsClient;
	HWND hActive;

	switch (msg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_CREATE:
		ccsClient.hWindowMenu = GetSubMenu(GetMenu(hWnd) , 1);
		ccsClient.idFirstChild = ID_CHILDWND;

		hClient = CreateWindow(TEXT("MDICLIENT") , NULL ,
			WS_CHILD | WS_CLIPCHILDREN | WS_VISIBLE ,
			0 , 0 , 0 , 0 , hWnd , (HMENU)1 , hIns , &ccsClient
		);
		return 0;
	case WM_COMMAND:
		hActive = (HWND)SendMessage(hClient , WM_MDIGETACTIVE , 0 , 0);
		switch(LOWORD(wp)) {
		case IDM_EXIT:
			SendMessage(hWnd , WM_CLOSE , 0 , 0);
			return 0;
		case IDM_NEW:
			CreateMDIWindow(MDI_CHILD , TITLE , 0 ,
				CW_USEDEFAULT , CW_USEDEFAULT ,
				CW_USEDEFAULT , CW_USEDEFAULT ,
				hClient , hIns , 0
			);
			return 0;
		case IDM_CAS:
			CascadeWindows(hClient , 0 , NULL , 0 , NULL);
			return 0;
		case IDM_ARR:
			ArrangeIconicWindows(hClient);
			return 0;
		case IDM_MAX:
			SendMessage(hClient ,
				WM_MDIMAXIMIZE , (WPARAM)hActive , 0);
			return 0;
		case IDM_NEXT:
			SendMessage(hClient ,
				WM_MDINEXT , (WPARAM)hActive , 0);
			return 0;
		case IDM_RES:
			SendMessage(hClient ,
				WM_MDIRESTORE , (WPARAM)hActive , 0);
			return 0;
		case IDM_TILH:
			TileWindows(hClient , 
				MDITILE_HORIZONTAL , NULL , 0 , NULL);
			return 0;
		case IDM_TILV:
			TileWindows(hClient , 
				MDITILE_VERTICAL , NULL , 0 , NULL);
			return 0;
		case IDM_DEL:
			SendMessage(hClient ,
				WM_MDIDESTROY ,(WPARAM)hActive , 0);
		}
	}
	return DefFrameProc(hWnd , hClient , msg , wp , lp);
}

int WINAPI WinMain(HINSTANCE hInstance , HINSTANCE hPrevInstance ,
			PSTR lpCmdLine , int nCmdShow) {
	HWND hWnd;
	MSG msg;
	WNDCLASS winc;

	hIns = hInstance;

	winc.style 		= CS_HREDRAW | CS_VREDRAW;
	winc.lpfnWndProc	= FrameProc;
	winc.cbClsExtra	= winc.cbWndExtra	= 0;
	winc.hInstance		= hInstance;
	winc.hIcon		= LoadIcon(NULL , IDI_APPLICATION);
	winc.hCursor		= LoadCursor(NULL , IDC_ARROW);
	winc.hbrBackground	= (HBRUSH)(COLOR_APPWORKSPACE + 1);
	winc.lpszMenuName	= MAKEINTRESOURCE(IDR_MENU1);
	winc.lpszClassName	= MDI_FRAME;

	if (!RegisterClass(&winc)) return 1;

	winc.lpfnWndProc		= DefMDIChildProc;
	winc.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH);
	winc.lpszClassName	= MDI_CHILD;

	if (!RegisterClass(&winc)) return 1;

	hWnd = CreateWindow(
			MDI_FRAME , TITLE ,
			WS_OVERLAPPEDWINDOW | WS_VISIBLE ,
			CW_USEDEFAULT , CW_USEDEFAULT ,
			CW_USEDEFAULT , CW_USEDEFAULT ,
			NULL , NULL , hInstance , NULL
	);

	if (hWnd == NULL) return 1;

	while (GetMessage(&msg , NULL , 0 , 0 )) {
		if (!TranslateMDISysAccel(hClient , &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return msg.wParam;
}