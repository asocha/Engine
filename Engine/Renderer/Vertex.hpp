//=====================================================
// Vertex.hpp
// by Andrew Socha
//=====================================================

#pragma once

#ifndef __included_Vertex__
#define __included_Vertex__

#include "Engine/Renderer/Material.hpp"
#include "Engine/Renderer/glext.h"
#include "Engine/Color/RGBAchars.hpp"

struct Vertex{
protected:
	const Uniform* BindAttribute(GLuint programID, const char* name, GLint count, GLenum type, GLboolean normalize, GLsizei stride, GLsizei offset, bool isIntegerAttribute = false) const;

public:
	virtual const Uniforms SetAttributes(int programID) const = 0;

	inline virtual ~Vertex(){}
};

struct Vertex_Anim : public Vertex{
	Vec3 m_position;
	RGBAchars m_color;
	Vec3 m_normal;
	Vec2 m_texCoords;
	Vec3 m_tangent;
	Vec3 m_bitangent;
	unsigned char m_boneIndeces[4];
	Vec3 m_boneWeights;

	const Uniforms SetAttributes(int programID) const;

	inline Vertex_Anim() :m_position(), m_normal(), m_texCoords(), m_tangent(), m_bitangent(), m_color(), m_boneWeights(1.0f, 0.0f, 0.0f){ m_boneIndeces[0] = 0;  m_boneIndeces[1] = 0;  m_boneIndeces[2] = 0;  m_boneIndeces[3] = 0; }
	inline Vertex_Anim(const Vec3& position) : m_position(position), m_normal(), m_texCoords(), m_tangent(), m_bitangent(), m_color(), m_boneWeights(1.0f, 0.0f, 0.0f) { m_boneIndeces[0] = 0;  m_boneIndeces[1] = 0;  m_boneIndeces[2] = 0;  m_boneIndeces[3] = 0; }
};

struct Vertex3D_PCNTTTB : public Vertex{
	Vec3 m_position;
	RGBAchars m_color;
	Vec3 m_normal;
	Vec2 m_texCoords;
	Vec2 m_texCoords2;
	Vec3 m_tangent;
	Vec3 m_bitangent;

	const Uniforms SetAttributes(int programID) const;

	inline Vertex3D_PCNTTTB() :m_position(), m_normal(), m_texCoords(), m_texCoords2(), m_tangent(), m_bitangent(), m_color(){}
	inline explicit Vertex3D_PCNTTTB(const Vec3& position, const Vec3& normal, const Vec2& texCoords, const Vec2& texCoords2, const Vec3& tangent, const Vec3& bitangent, const RGBAchars& color = RGBAchars::WHITE) :
		m_position(position), m_normal(normal), m_texCoords(texCoords), m_texCoords2(texCoords2), m_tangent(tangent), m_bitangent(bitangent), m_color(color){}
};
typedef std::vector<Vertex3D_PCNTTTB> Vertex3D_PCNTTTBs;

struct Vertex3D_PCTNTB : public Vertex{
	Vec3 m_position;
	RGBAchars m_color;
	Vec2 m_texCoords;
	Vec3 m_normal;
	Vec3 m_tangent;
	Vec3 m_bitangent;

	const Uniforms SetAttributes(int programID) const;

	inline Vertex3D_PCTNTB() :m_position(), m_normal(), m_texCoords(), m_tangent(), m_bitangent(), m_color(){}
	inline explicit Vertex3D_PCTNTB(const Vec3& position, const Vec2& texCoords, const Vec3& normal, const Vec3& tangent, const Vec3& bitangent, const RGBAchars& color = RGBAchars::WHITE) :
		m_position(position), m_normal(normal), m_texCoords(texCoords), m_tangent(tangent), m_bitangent(bitangent), m_color(color){}
};
typedef std::vector<Vertex3D_PCNTTTB> Vertex3D_PCNTTTBs;

struct Vertex2D_PCT : public Vertex{
	Vec2 m_position;
	RGBAchars m_color;
	Vec2 m_texCoords;

	const Uniforms SetAttributes(int programID) const;

	inline Vertex2D_PCT() :m_position(), m_texCoords(), m_color(){}
	inline explicit Vertex2D_PCT(const Vec2& position, const Vec2& texCoords, const RGBAchars& color = RGBAchars::WHITE) :
		m_position(position), m_texCoords(texCoords), m_color(color){}
};
typedef std::vector<Vertex2D_PCT> Vertex2D_PCTs;

struct Vertex3D_PC : public Vertex{
	Vec3 m_position;
	RGBAchars m_color;

	const Uniforms SetAttributes(int programID) const;

	inline Vertex3D_PC() :m_position(), m_color(){}
	inline explicit Vertex3D_PC(const Vec3& position, const RGBAchars& color = RGBAchars::WHITE) :
		m_position(position), m_color(color){}
};
typedef std::vector<Vertex3D_PC> Vertex3D_PCs;

struct Vertex3D_PCT : public Vertex{
	Vec3 m_position;
	RGBAchars m_color;
	Vec2 m_texCoords;

	const Uniforms SetAttributes(int programID) const;

	inline Vertex3D_PCT() :m_position(), m_texCoords(), m_color(){}
	inline explicit Vertex3D_PCT(const Vec3& position, const Vec2& texCoords, const RGBAchars& color = RGBAchars::WHITE) :
		m_position(position), m_texCoords(texCoords), m_color(color){}
};
typedef std::vector<Vertex3D_PCT> Vertex3D_PCTs;

struct Vertex3D_PCT_Face{
	Vertex3D_PCT vertexes[4];
};
typedef std::vector<Vertex3D_PCT_Face> Vertex3D_PCT_Faces;

#endif