//=====================================================
// MatrixStack.hpp
// by Andrew Socha
//=====================================================

#pragma once

#ifndef __included_MatrixStack__
#define __included_MatrixStack__

#include "Engine/Math/Matrix4.hpp"
#include <stack>

class MatrixStack{
private:
	std::stack<Matrix4> m_stack;
	Matrix4 m_totalTransformation;

public:
	inline MatrixStack():m_totalTransformation(){}

	void PushMatrix(const Matrix4& matrix);
	const Matrix4 PopMatrix();

	inline const Matrix4 GetTop() const{ return m_stack.top(); }
	inline const Matrix4 GetTotalMatrixTransformation() const{ return m_totalTransformation; }
};

#endif