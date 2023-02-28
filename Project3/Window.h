#pragma once
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// �E�B���h�E�֘A�̏���
class Window
{
	// �E�B���h�E�̃n���h��
	GLFWwindow* const window;

	// �c����
	GLfloat aspect;

	// �E�B���h�E�̃T�C�Y
	GLfloat size[2];

	// ���[���h���W�n�ɑ΂���f�o�C�X���W�n�̊g�嗦
	GLfloat scale;


public:
	// �R���X�g���N�^
	Window(int width = 640, int height = 480, const char* title = "Hello!")
		: window(glfwCreateWindow(width, height, title, NULL, NULL))
	{
		if (window == NULL)
		{
			// �E�B���h�E���쐬�ł��Ȃ�����
			std::cerr << "Can't create GLFW window." << std::endl;
			exit(1);
		}

		// ���݂̃E�B���h�E�������Ώۂɂ���
		glfwMakeContextCurrent(window);

		// GLEW������������
		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK)
		{
			// GLEW�̏������Ɏ��s����
			std::cerr << "Can't initalize GLEW" << std::endl;
			exit(1);
		}

		// ���������̃^�C�~���O��҂�
		glfwSwapInterval(1);

		// ���̃C���X�^���X��this�|�C���^���L�^���Ă���
		glfwSetWindowUserPointer(window, this);

		// �E�B���h�E�́O�T�C�Y�ύX���ɌĂт��������̓o�^
		glfwSetWindowSizeCallback(window, resize);

		// �J�����E�B���h�E�̏����ݒ�
		resize(window, width, height);
	}

	virtual ~Window()
	{
		glfwDestroyWindow(window);
	}

	// �`�惋�[�v�̌p������
	explicit operator bool()
	{
		// �C�x���g�����o��
		glfwWaitEvents();

		// �E�B���h�E�����K�v���Ȃ����true��Ԃ�
		return !glfwWindowShouldClose(window);
	}

	// �_�u���o�b�t�@�����O
	void swapBuffers() const
	{
		// �J���[�o�b�t�@�����ւ���
		glfwSwapBuffers(window);
	}

	static void resize(GLFWwindow* const window, int width, int height)
	{
		// �t���[���o�b�t�@�̃T�C�Y�𒲂ׂ�
		int fbWidth, fbHeight;
		glfwGetFramebufferSize(window, &fbWidth, &fbHeight);

		// �t���[���o�b�t�@�S�̂��r���[�|�[�g�ɐݒ肷��
		glViewport(0, 0, fbWidth, fbHeight);

		// ���̃C���X�^���X��this�|�C���^�𓾂�
		Window* const
			instance(static_cast<Window*>(glfwGetWindowUserPointer(window)));
		if (instance != NULL)
		{
			// ���̃C���X�^���X���ێ�����c������X�V����
			instance->aspect =
				static_cast<GLfloat>(width) / static_cast<GLfloat>(height);
		}

	}
	// �c��������o��
	GLfloat getAspect() const { return aspect; }
};