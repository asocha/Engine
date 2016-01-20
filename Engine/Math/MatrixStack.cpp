//=====================================================
// MatrixStack.cpp
// by Andrew Socha
//=====================================================

#include "MatrixStack.hpp"

///=====================================================
/// 
///=====================================================
void MatrixStack::PushMatrix(const Matrix4& matrix){
	m_stack.push(matrix);
	m_totalTransformation.TransformByMatrix(matrix);
}

///=====================================================
/// 
///=====================================================
const Matrix4 MatrixStack::PopMatrix(){
	if (m_stack.empty()) return Matrix4();

	const Matrix4 temp(m_stack.top());
	m_totalTransformation.TransformByMatrix(temp.GetInverse());
	m_stack.pop();

	return temp;
}