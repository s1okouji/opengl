#pragma once
#include <GL/glew.h>

// �}�`�f�[�^
class Object
{
	// ���_�z��I�u�W�F�N�g��
	GLuint vao;
	// ���_�o�b�t�@�I�u�W�F�N�g��
	GLuint vbo;

private:
	// �R�s�[�R���X�g���N�^�ɂ��R�s�[�֎~
	Object(const Object& o);

	// ����ɂ��R�s�[�֎~
	Object& operator = (const Object& o);

public:
	// ���_����
	struct Vertex
	{
		// �ʒu
		GLfloat position[2];
	};

	void bind() const
	{
		// �`�悷�钸�_�z��I�u�W�F�N�g���w�肷��
		glBindVertexArray(vao);
	}

	// �R���X�g���N�^
	Object(GLint size, GLsizei vertexcount, const Vertex* vertex)
	{		
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex), vertex, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);
	}

	// �f�X�g���N�^
	virtual ~Object()
	{
		// ���_�z��I�u�W�F�N�g���폜����
		glDeleteVertexArrays(1, &vao);
		// ���_�o�b�t�@�I�u�W�F�N�g���폜����
		glDeleteBuffers(1, &vbo);
	}
};