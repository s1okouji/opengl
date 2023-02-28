#pragma once
#include <memory>

// �}�`�f�[�^
#include "Object.h"

// �}�`�̕`��
class Shape
{
	// �}�`�f�[�^
	std::shared_ptr<const Object> object;
protected:
	// �`��Ɏg�����_�̐�
	const GLsizei vertexcount;

public:
	Shape(GLint size, GLsizei vertexcount, const Object::Vertex* vertex)
		: object(new Object(size, vertexcount, vertex))
		, vertexcount(vertexcount)
	{
	}

	// �`��
	void draw() const
	{
		// ���_�z��I�u�W�F�N�g����������
		object->bind();
		// �`������s����
		execute();
	}

	// �`��̎��s
	virtual void execute() const
	{
		// �܂���ŕ`�悷��
		glDrawArrays(GL_LINE_LOOP, 0, vertexcount);
	}
};