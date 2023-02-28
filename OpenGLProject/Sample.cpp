#pragma comment(lib, "OpenGL32.lib")

#include <windows.h>
#include <tchar.h>

const char* gName = "OpenGL�e�X�g";

// �E�B���h�E�v���V�[�W��
LRESULT CALLBACK WndProc(HWND hWnd, UINT mes, WPARAM wParam, LPARAM lParam) {
    if (mes == WM_DESTROY || mes == WM_CLOSE) { PostQuitMessage(0); return 0; }
    return DefWindowProc(hWnd, mes, wParam, lParam);
}

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
    // �A�v���P�[�V�����̏�����
    MSG msg; HWND hWnd;
    WNDCLASSEX wcex = { sizeof(WNDCLASSEX), CS_OWNDC | CS_HREDRAW | CS_VREDRAW, WndProc, 0, 0, hInstance, NULL, NULL, (HBRUSH)(COLOR_WINDOW + 1), NULL, (TCHAR*)gName, NULL };
    if (!RegisterClassEx(&wcex))
        return 0;

    RECT R = { 0, 0, 640, 480 };
    AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, FALSE);
    if (!(hWnd = CreateWindow(gName, gName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, R.right - R.left, R.bottom - R.top, NULL, NULL, hInstance, NULL)))
        return 0;

    // OpenGL������
    // �s�N�Z���t�H�[�}�b�g������
    PIXELFORMATDESCRIPTOR pfd =
    {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, //Flags
        PFD_TYPE_RGBA, //The kind of framebuffer. RGBA or palette.
        32, //Colordepth of the framebuffer.
        0, 0, 0, 0, 0, 0,
        0,
        0,
        0,
        0, 0, 0, 0,
        24, //Number of bits for the depthbuffer
        8, //Number of bits for the stencilbuffer
        0, //Number of Aux buffers in the framebuffer.
        PFD_MAIN_PLANE,
        0,
        0, 0, 0
    };
    HDC dc = GetDC(hWnd);
    int format = ChoosePixelFormat(dc, &pfd);
    if (format == 0)
        return 0; // �Y������s�N�Z���t�H�[�}�b�g������

    // OpenGL���g���f�o�C�X�R���e�L�X�g�Ɏw��̃s�N�Z���t�H�[�}�b�g���Z�b�g
    if (!SetPixelFormat(dc, format, &pfd))
        return 0; // DC�փt�H�[�}�b�g��ݒ肷��̂Ɏ��s

    // OpenGL context���쐬
    HGLRC glRC = wglCreateContext(dc);

    // �쐬���ꂽ�R���e�L�X�g���J�����g�i���ݎg�p���̃R���e�L�X�g�j���H
    if (!wglMakeCurrent(dc, glRC))
        return 0; // �����������Ȃ��݂����c


    ShowWindow(hWnd, nCmdShow);

    // ���b�Z�[�W ���[�v
    do {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    } while (msg.message != WM_QUIT);

    // �㏈��
    // �J�����g�R���e�L�X�g�𖳌��ɂ���
    wglMakeCurrent(NULL, NULL);

    // �J�����g�R���e�L�X�g���폜
    wglDeleteContext(glRC);

    // �f�o�C�X�R���e�L�X�g���
    ReleaseDC(hWnd, dc);

    return 0;
}