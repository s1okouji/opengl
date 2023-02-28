#pragma once
#include <GL/glew.h>

// 図形データ
class Object
{
	// 頂点配列オブジェクト名
	GLuint vao;
	// 頂点バッファオブジェクト名
	GLuint vbo;

private:
	// コピーコンストラクタによるコピー禁止
	Object(const Object& o);

	// 代入によるコピー禁止
	Object& operator = (const Object& o);

public:
	// 頂点属性
	struct Vertex
	{
		// 位置
		GLfloat position[2];
	};

	void bind() const
	{
		// 描画する頂点配列オブジェクトを指定する
		glBindVertexArray(vao);
	}

	// コンストラクタ
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

	// デストラクタ
	virtual ~Object()
	{
		// 頂点配列オブジェクトを削除する
		glDeleteVertexArrays(1, &vao);
		// 頂点バッファオブジェクトを削除する
		glDeleteBuffers(1, &vbo);
	}
};