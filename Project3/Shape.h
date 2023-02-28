#pragma once
#include <memory>

// 図形データ
#include "Object.h"

// 図形の描画
class Shape
{
	// 図形データ
	std::shared_ptr<const Object> object;
protected:
	// 描画に使う頂点の数
	const GLsizei vertexcount;

public:
	Shape(GLint size, GLsizei vertexcount, const Object::Vertex* vertex)
		: object(new Object(size, vertexcount, vertex))
		, vertexcount(vertexcount)
	{
	}

	// 描画
	void draw() const
	{
		// 頂点配列オブジェクトを結合する
		object->bind();
		// 描画を実行する
		execute();
	}

	// 描画の実行
	virtual void execute() const
	{
		// 折れ線で描画する
		glDrawArrays(GL_LINE_LOOP, 0, vertexcount);
	}
};