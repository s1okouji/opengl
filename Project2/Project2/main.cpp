#pragma comment(lib, "glew32.lib")

#include <Windows.h>
#include <GL/glew.h>
#include <stdio.h>

LRESULT CALLBACK MyWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (uMsg == WM_DESTROY || uMsg == WM_CLOSE) { PostQuitMessage(0); return 0; }
	return DefWindowProcA(hwnd, uMsg, wParam, lParam);
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow) {

	// アプリケーションの初期化
	MSG msg; HWND hWnd;
	WNDCLASSEX wcex = { sizeof(WNDCLASSEX) , CS_OWNDC | CS_HREDRAW | CS_VREDRAW, MyWndProc, 0, 0, hInstance, NULL, NULL, (HBRUSH)(COLOR_WINDOW + 1), NULL, "TestWindow", NULL };
	if (!RegisterClassEx(&wcex))
		return 0;
	RECT R = { 0, 0, 640, 480 };
	AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, FALSE);
	if (!(hWnd = CreateWindow("TestWindow", "TestWindow", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, R.right - R.left, R.bottom - R.top, NULL, NULL, hInstance, NULL)))
		return 0;

	// glewの初期化
	GLuint VertexArrayID;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);


	ShowWindow(hWnd, nCmdShow);
	do {
		if (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}
	} while (msg.message != WM_QUIT);

	return 0;
}