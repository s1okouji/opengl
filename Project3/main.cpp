#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glfw3dll.lib")
#pragma comment(lib, "glew32.lib")

#include <cstdlib>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <vector>
#include <memory>
#include "Shape.h"
#include "Window.h"

using namespace std;

static const GLchar vsrc[] =
"#version 460 core\n"
"in vec4 position;\n"
"void main()\n"
"{\n"
"	gl_Position = position;\n"
"}\n";

static constexpr GLchar fsrc[] =
"#version 460 core\n"
"out vec4 fragment;\n"
"void main()\n"
"{\n"
" fragment = vec4(1.0,0.0,0.0,1.0);\n"
"}\n";

// ��`�̒��_�̈ʒu
constexpr Object::Vertex rectangleVertex[] =
{
	{-0.5f, -0.5f},
	{1.5f, -0.5f},
	{1.5f, 1.5f},
	{-0.5f, 1.5f}
};

GLuint createProgram(const char* vsrc, const char* fsrc)
{
	const GLuint program(glCreateProgram());

	if (vsrc != NULL) {
		const GLuint vobj(glCreateShader(GL_VERTEX_SHADER));
		glShaderSource(vobj, 1, &vsrc, NULL);
		glCompileShader(vobj);

		glAttachShader(program, vobj);
		glDeleteShader(vobj);
	}

	if (fsrc != NULL) {
		const GLuint fobj(glCreateShader(GL_FRAGMENT_SHADER));
		glShaderSource(fobj, 1, &fsrc, NULL);
		glCompileShader(fobj);

		glAttachShader(program, fobj);
		glDeleteShader(fobj);
	}

	glBindAttribLocation(program, 0, "position");
	glBindFragDataLocation(program, 0, "fragment");
	glLinkProgram(program);

	return program;
}

bool readShaderSource(const char* name, vector<GLchar>& buffer)
{
	// �t�@�C������NULL
	if (name == NULL) return false;

	ifstream file(name, ios::binary);
	if (file.fail())
	{
		cerr << "Error: Can't open source file: " << name << endl;
		return false;
	}

	file.seekg(0L, ios::end);
	GLsizei length = static_cast<GLsizei>(file.tellg());

	// �t�@�C���T�C�Y�̃��������m��
	buffer.resize(length + 1);

	// �t�@�C����擪����ǂ�
	file.seekg(0L, ios::beg);
	file.read(buffer.data(), length);
	buffer[length] = '\0';

	if (file.fail())
	{
		cerr << "Error: Could not read source file: " << name << endl;
		file.close();
		return false;
	}

	file.close();
	return true;
}

GLuint loadProgram(const char* vert, const char* frag)
{
	// �V�F�[�_�̃\�[�X�t�@�C����ǍL
	vector<GLchar> vsrc;
	const bool vstat(readShaderSource(vert, vsrc));
	vector<GLchar> fsrc;
	const bool fstat(readShaderSource(frag, fsrc));

	return vstat && fstat ? createProgram(vsrc.data(), fsrc.data()) : 0;
}

int main() {
	// Initalize GLFW
	if (glfwInit() == GL_FALSE) {
		cerr << "Failed to initalize GLFW" << endl;
		return -1;
	}

	// �v���O�����̏I������
	atexit(glfwTerminate);

	// OpenmGL�̃o�[�W�����ݒ�
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	// OpenGL
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	Window window;

	//GLFWwindow* mainWindow = glfwCreateWindow(640, 480, "Sample Window", NULL, NULL);
	/*if (mainWindow == NULL) {
		cerr << "Failed to create GLFW window" << endl;		
		return -1;
	}*/
	// �E�B���h�E��OpenGL�̏����Ώۂɂ���
	//glfwMakeContextCurrent(mainWindow);

	// Init Glew
	/*glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		cerr << "Can't initalize GLEW" << endl;
		return -1;
	}*/



	//glfwSwapInterval(1);

	// �w�i�F���w��
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// �r���[�|�[�g��ݒ肷��
	//glViewport(100, 50, 300, 300);

	// �v���O�����I�u�W�F�N�g���쐬����
	const GLuint program(loadProgram("point.vert", "point.frag"));

	// uniform�ϐ��̏ꏊ���擾����
	const GLint aspectLoc(glGetUniformLocation(program, "aspect"));
	
	//const GLuint program(createProgram(vsrc, fsrc));

	// �}�`�f�[�^���쐬����
	std::unique_ptr<const Shape> shape(new Shape(2, 4, rectangleVertex));


	while (window) {
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(program);
			
		// uniform �ϐ��ɒl��ݒ肷��
		glUniform1f(aspectLoc, window.getAspect());

		// �}�`��`�悷��
		shape->draw();

		// �J���[�o�b�t�@�����ւ���
		window.swapBuffers();
	}
}