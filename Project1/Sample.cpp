#pragma comment(lib, "OpenGL32.lib")

// glGenBuffers�Ƃ����g�p���邽�� 
//#define GL_GLEXT_PROTOTYPES true

#include <stdio.h>
#include <windows.h>
#include <tchar.h>
#include <KHR/khrplatform.h>
#include <gl/GL.h>
#include <GL/glext.h>
//#include <GL/glcorearb.h>

PFNGLGENBUFFERSPROC glGenBuffers;
PFNGLISBUFFERPROC glIsBuffer;
PFNGLBINDBUFFERPROC glBindBuffer;
PFNGLBUFFERDATAPROC glBufferData;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
PFNGLLINKPROGRAMPROC glLinkProgram;
PFNGLGETPROGRAMIVPROC glGetProgramiv;
PFNGLMAPBUFFERPROC glMapBuffer;
PFNGLUNMAPBUFFERPROC glUnmapBuffer;
PFNGLUSEPROGRAMPROC glUseProgram;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;


const char* gName = "OpenGL�e�X�g";

static GLuint buffer;
static GLuint vertShader;
static GLuint fragShader;
static GLuint gl2Program;

// �E�B���h�E�v���V�[�W��
LRESULT CALLBACK WndProc(HWND hWnd, UINT mes, WPARAM wParam, LPARAM lParam) {
    if (mes == WM_DESTROY || mes == WM_CLOSE) { PostQuitMessage(0); return 0; }
    return DefWindowProc(hWnd, mes, wParam, lParam);
}

void init() {

    glGenBuffers =
        (PFNGLGENBUFFERSPROC)wglGetProcAddress("glGenBuffers");
    glIsBuffer =
        (PFNGLISBUFFERPROC)wglGetProcAddress("glIsBuffer");
    glBindBuffer =
        (PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer");
    glBufferData =
        (PFNGLBUFFERDATAPROC)wglGetProcAddress("glBufferData");
    glVertexAttribPointer =
        (PFNGLVERTEXATTRIBPOINTERPROC)wglGetProcAddress("glVertexAttribPointer");
    glLinkProgram =
        (PFNGLLINKPROGRAMPROC)wglGetProcAddress("glLinkProgram");
    glGetProgramiv =
        (PFNGLGETPROGRAMIVPROC)wglGetProcAddress("glGetProgramiv");
    glMapBuffer =
        (PFNGLMAPBUFFERPROC)wglGetProcAddress("glMapBuffer");
    glUnmapBuffer =
        (PFNGLUNMAPBUFFERPROC)wglGetProcAddress("glUnmapBuffer");
    glUseProgram =
        (PFNGLUSEPROGRAMPROC)wglGetProcAddress("glUseProgram");
    glEnableVertexAttribArray =
        (PFNGLENABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glEnableVertexAttribArray");
    glDisableVertexAttribArray =
        (PFNGLDISABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glDisableVertexAttribArray");

    GLint compiled, linked;

    /* ���_�o�b�t�@�I�u�W�F�N�g�̃��������Q�Ƃ���|�C���^*/
    typedef GLfloat Position[2];
    Position* position;

    /* �V�F�[�_�[�v���O�����̃����N*/
    glLinkProgram(gl2Program);
    glGetProgramiv(gl2Program, GL_LINK_STATUS, &linked);
    //printProgramInfoLog(gl2Program);
    if (linked == GL_FALSE) {
        fprintf(stderr, "Link error.\n");
        exit(1);
    }

    /* ���_�o�b�t�@�I�u�W�F�N�g������ */
    glGenBuffers(1, &buffer);
    /* ���_�o�b�t�@�I�u�W�F�N�g��4���X���̃������̈���m�ۂ��� */
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Position) * 4, NULL, GL_STATIC_DRAW);

    /* ���_�o�b�t�@�I�u�W�F�N�g�̃��������v���O�����̃�������ԂɃ}�b�v���� */
    position = (Position*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

    /* ���_�o�b�t�@�I�u�W�F�N�g�̃������Ƀf�[�^���������� */
    position[0][0] = 0.9;
    position[0][1] = 0.9;
    position[1][0] = -0.9;
    position[1][1] = 0.9;
    position[2][0] = -0.9;
    position[2][1] = -0.9;
    position[3][0] = 0.9;
    position[3][1] = -0.9;

    /* ���_�o�b�t�@�I�u�W�F�N�g�̃��������v���O�����̃�������Ԃ���؂藣�� */
    glUnmapBuffer(GL_ARRAY_BUFFER);
    /* ���_�o�b�t�@�I�u�W�F�N�g��������� */
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

static void display() {
    /* ��ʂ��N���A */
    glClear(GL_COLOR_BUFFER_BIT);

    /* �V�F�[�_�[�v���O������K�p���� */
    glUseProgram(gl2Program);

    /* index��0��attribute�ϐ��ɒ��_����Ή��t���� */
    glEnableVertexAttribArray(0);

    /* ���_�o�b�t�@�����ƉF�s�Ƃ��� buffer ���w�肷�� */
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    /* ���_���̊i�[�ꏊ�Ə������w�肷�� */
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

    /* �}�`��`�悷�� */
    glDrawArrays(GL_LINE_LOOP, 0, 4);
    /* ���_�o�b�t�@�I�u�W�F�N�g���J������ */
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    /* index��0��attribute�ϐ��̒��_���Ƃ̑Ή��t������������ */
    glDisableVertexAttribArray(0);
    glFlush();
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
    // �o�b�t�@�[�Ȃǂ��m��
    init();
    display();
    SwapBuffers(dc);
    // ���b�Z�[�W ���[�v
    do {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    } while (msg.message != WM_QUIT);

    return 0;


    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    return 0;
}