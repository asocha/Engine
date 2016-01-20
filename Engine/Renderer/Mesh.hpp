//=====================================================
// Mesh.hpp
// by Andrew Socha
//=====================================================

#pragma once

#ifndef __included_Mesh__
#define __included_Mesh__

#include <string>
class OpenGLRenderer;
struct Vertex;
struct Vec3;
struct Vec2;
class BinaryBufferBuilder;
#include <vector>
#include "Engine/Renderer/Vertex.hpp"

struct C23Header
{
	char				m_fourCC[4];
	unsigned char		m_subType;
	unsigned char		m_version;
};

namespace EngineAndrew{
	class Mesh{
	public:
		int m_vboID;
		int m_vaoID;
		int m_iboID;
		Vertex* m_vertexType;
		std::vector<Vertex_Anim> m_vertices;
		std::vector<int> m_indeces;

		Mesh();
		~Mesh();

		void Startup(OpenGLRenderer* renderer);
		void Shutdown(const OpenGLRenderer* renderer);

		bool LoadFromC23File(const std::string& filename, const OpenGLRenderer* renderer);
		bool WriteToC23File(const std::string& c23FilePath, const std::string& fileComment = "", unsigned char subType = 2, unsigned char version = 1);
		void WriteToC23Buffer(const std::string& fileComment, BinaryBufferBuilder& buffer);

		static Vec3 ComputeTriangleNormal(const Vec3& ccwPosition0, const Vec3& ccwPosition1, const Vec3& ccwPosition2);
		static void ComputeSurfaceTangentsAtVertex(Vec3& surfaceTangentAtVertex_out, Vec3& surfaceBitangentAtVertex_out, const Vec3& positionOfThisVertex, const Vec2& texCoordsOfThisVertex,
			const Vec3& positionOfPreviousAdjacentVertex, const Vec2& texCoordsOfPreviousAdjacentVertex, const Vec3& positionOfNextAdjacentVertex, const Vec2& texCoordsOfNextAdjacentVertex, bool normalize = true);
	};
}

#endif