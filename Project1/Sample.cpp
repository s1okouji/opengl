#pragma comment(lib, "OpenGL32.lib")

// glGenBuffersとかを使用するため 
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


const char* gName = "OpenGLテスト";

static GLuint buffer;
static GLuint vertShader;
static GLuint fragShader;
static GLuint gl2Program;

// ウィンドウプロシージャ
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

    /* 頂点バッファオブジェクトのメモリを参照するポインタ*/
    typedef GLfloat Position[2];
    Position* position;

    /* シェーダープログラムのリンク*/
    glLinkProgram(gl2Program);
    glGetProgramiv(gl2Program, GL_LINK_STATUS, &linked);
    //printProgramInfoLog(gl2Program);
    if (linked == GL_FALSE) {
        fprintf(stderr, "Link error.\n");
        exit(1);
    }

    /* 頂点バッファオブジェクトを一つ作る */
    glGenBuffers(1, &buffer);
    /* 頂点バッファオブジェクトに4兆店分のメモリ領域を確保する */
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Position) * 4, NULL, GL_STATIC_DRAW);

    /* 頂点バッファオブジェクトのメモリをプログラムのメモリ空間にマップする */
    position = (Position*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

    /* 頂点バッファオブジェクトのメモリにデータを書き込む */
    position[0][0] = 0.9;
    position[0][1] = 0.9;
    position[1][0] = -0.9;
    position[1][1] = 0.9;
    position[2][0] = -0.9;
    position[2][1] = -0.9;
    position[3][0] = 0.9;
    position[3][1] = -0.9;

    /* 頂点バッファオブジェクトのメモリをプログラムのメモリ空間から切り離す */
    glUnmapBuffer(GL_ARRAY_BUFFER);
    /* 頂点バッファオブジェクトを解放する */
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

static void display() {
    /* 画面をクリア */
    glClear(GL_COLOR_BUFFER_BIT);

    /* シェーダープログラムを適用する */
    glUseProgram(gl2Program);

    /* indexが0のattribute変数に頂点情報を対応付ける */
    glEnableVertexAttribArray(0);

    /* 頂点バッファお武家宇都として buffer を指定する */
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    /* 頂点情報の格納場所と書式を指定する */
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

    /* 図形を描画する */
    glDrawArrays(GL_LINE_LOOP, 0, 4);
    /* 頂点バッファオブジェクトを開放する */
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    /* indexが0のattribute変数の頂点情報との対応付けを解除する */
    glDisableVertexAttribArray(0);
    glFlush();
}

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
    // アプリケーションの初期化
    MSG msg; HWND hWnd;
    WNDCLASSEX wcex = { sizeof(WNDCLASSEX), CS_OWNDC | CS_HREDRAW | CS_VREDRAW, WndProc, 0, 0, hInstance, NULL, NULL, (HBRUSH)(COLOR_WINDOW + 1), NULL, (TCHAR*)gName, NULL };
    if (!RegisterClassEx(&wcex))
        return 0;

    RECT R = { 0, 0, 640, 480 };
    AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, FALSE);
    if (!(hWnd = CreateWindow(gName, gName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, R.right - R.left, R.bottom - R.top, NULL, NULL, hInstance, NULL)))
        return 0;

    // OpenGL初期化
    // ピクセルフォーマット初期化
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
        return 0; // 該当するピクセルフォーマットが無い

    // OpenGLが使うデバイスコンテキストに指定のピクセルフォーマットをセット
    if (!SetPixelFormat(dc, format, &pfd))
        return 0; // DCへフォーマットを設定するのに失敗

    // OpenGL contextを作成
    HGLRC glRC = wglCreateContext(dc);

    // 作成されたコンテキストがカレント（現在使用中のコンテキスト）か？
    if (!wglMakeCurrent(dc, glRC))
        return 0; // 何か正しくないみたい…
    ShowWindow(hWnd, nCmdShow);
    // バッファーなどを確保
    init();
    display();
    SwapBuffers(dc);
    // メッセージ ループ
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