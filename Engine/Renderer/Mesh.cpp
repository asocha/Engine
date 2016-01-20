//=====================================================
// Mesh.cpp
// by Andrew Socha
//=====================================================

#include "Engine/Core/EngineCore.hpp"
#include "Mesh.hpp"
#include "Engine/Renderer/OpenGLRenderer.hpp"
#include "Engine/Core/BinaryFileParser.hpp"
#include "Engine/Core/BinaryBufferBuilder.hpp"

///=====================================================
/// 
///=====================================================
EngineAndrew::Mesh::Mesh():
m_vboID(0),
m_vaoID(0),
m_iboID(0),
m_indeces(),
m_vertices(),
m_vertexType(nullptr){
}

///=====================================================
/// 
///=====================================================
EngineAndrew::Mesh::~Mesh(){
	delete m_vertexType;
}

///=====================================================
/// 
///=====================================================
void EngineAndrew::Mesh::Startup(OpenGLRenderer* /*renderer*/){
	//not yet implemented, use LoadFromC23File
}

///=====================================================
/// 
///=====================================================
void EngineAndrew::Mesh::Shutdown(const OpenGLRenderer* renderer){
	renderer->DeleteBuffer((GLuint*)&m_vboID);
	renderer->DeleteBuffer((GLuint*)&m_iboID);
	renderer->DeleteVAO((GLuint*)&m_vaoID);
}

///=====================================================
/// 
///=====================================================
bool EngineAndrew::Mesh::LoadFromC23File(const std::string& filename, const OpenGLRenderer* renderer){
	RECOVERABLE_ASSERT(m_vertexType == nullptr); //make sure mesh hasn't loaded before

	BinaryFileParser fileParser(filename);
	if (fileParser.ReadChar() != 'G') return false; //4-cc
	if (fileParser.ReadChar() != 'C') return false;
	if (fileParser.ReadChar() != '2') return false;
	if (fileParser.ReadChar() != '3') return false;
	if (fileParser.ReadChar() != 2) return false; //version
	char subtype = fileParser.ReadChar();
	if (subtype != 2 && subtype != 4) return false; //subversion
	fileParser.ReadString();

	m_vertexType = new Vertex_Anim();

	m_indeces.clear();
	m_vertices.clear();

	int numVerts = fileParser.ReadInt();
	m_vertices.reserve(numVerts);

	Vertex_Anim vert;

	for (int i = 0; i < numVerts; ++i){
		vert.m_position = fileParser.ReadVec3();
		vert.m_color = fileParser.ReadRGBAchars();
		vert.m_texCoords = fileParser.ReadVec2();
		
		vert.m_normal = fileParser.ReadVec3();
		vert.m_tangent = fileParser.ReadVec3();
		vert.m_bitangent = fileParser.ReadVec3();

		vert.m_boneIndeces[0] = fileParser.ReadUnsignedChar();
		vert.m_boneIndeces[1] = fileParser.ReadUnsignedChar();
		vert.m_boneIndeces[2] = fileParser.ReadUnsignedChar();
		vert.m_boneIndeces[3] = fileParser.ReadUnsignedChar();

		vert.m_boneWeights = fileParser.ReadVec3();

		m_vertices.push_back(vert);
	}

	int numIndeces = fileParser.ReadInt();
	if (numIndeces > 0){
		m_indeces.reserve(numIndeces);
		for (int i = 0; i < numIndeces; ++i){
			m_indeces.push_back(fileParser.ReadInt());
		}
	}
	else{
		numIndeces = numVerts;
		m_indeces.reserve(numIndeces);
		for (int i = 0; i < numIndeces; ++i){
			m_indeces.push_back(i);
		}
	}

	renderer->GenerateBuffer((GLuint*)&m_vboID);
	renderer->SendVertexDataToBuffer(m_vertices.data(), sizeof(Vertex_Anim) * numVerts, m_vboID);

	renderer->GenerateBuffer((GLuint*)&m_iboID);
	renderer->SendVertexDataToBuffer(m_indeces.data(), sizeof(int) * numIndeces, m_iboID);

	m_vaoID = renderer->CreateVAOBasic();

	return true;
}


const char* kC23FileFourCCString = "GC23";

///=====================================================
/// Based on code from Squirrel Eiserloh
///=====================================================
bool EngineAndrew::Mesh::WriteToC23File(const std::string& c23FilePath, const std::string& fileComment /*= ""*/, unsigned char subType /*= 2*/, unsigned char version /*= 1*/){
	C23Header header;
	memcpy(header.m_fourCC, kC23FileFourCCString, 4);
	header.m_subType = subType;
	header.m_version = version;

	size_t commentLength = fileComment.size();
	size_t estimatedBufferSize = sizeof(C23Header);
	estimatedBufferSize += commentLength + 1; // including zero terminator
	estimatedBufferSize += sizeof(m_vertices.size());
	estimatedBufferSize += m_vertices.size() * sizeof(Vertex_Anim);
	if (subType == 2){
		estimatedBufferSize += sizeof(m_indeces.size());
		estimatedBufferSize += m_indeces.size() * sizeof(unsigned int);
	}

	BinaryBufferBuilder buffer(estimatedBufferSize);
	buffer.AppendC23Header(header);

	if (subType == 2){
		WriteToC23Buffer(fileComment, buffer);
	}
	else{
		//unknown subtype
		FATAL_ERROR();
		return false;
	}

	buffer.WriteBufferToFile(c23FilePath);
	return true;
}

///=====================================================
/// Based on code from Squirrel Eiserloh
///=====================================================
void EngineAndrew::Mesh::WriteToC23Buffer(const std::string& fileComment, BinaryBufferBuilder& buffer){
	buffer.AppendString(fileComment);

	buffer.AppendUnsignedInt((unsigned int)m_vertices.size());
	for (unsigned int vertexNum = 0; vertexNum < m_vertices.size(); ++vertexNum){
		const Vertex_Anim& vertex = m_vertices[vertexNum];
		buffer.AppendVertex_Anim(vertex);
	}

	if (m_indeces.size() > 0){
		buffer.AppendUnsignedInt((unsigned int)m_indeces.size());
		for (unsigned int indexNum = 0; indexNum < m_indeces.size(); ++indexNum){
			buffer.AppendUnsignedInt(m_indeces[indexNum]);
		}
	}
	else{
		// Synthesize "fake" indexes; this file format subType demands indexed triangles
		buffer.AppendUnsignedInt((unsigned int)m_vertices.size());
		for (unsigned int vertexNum = 0; vertexNum < m_vertices.size(); ++vertexNum){
			buffer.AppendUnsignedInt(vertexNum);
		}
	}
}

///=====================================================
/// Code from Squirrel Eiserloh
///=====================================================
Vec3 EngineAndrew::Mesh::ComputeTriangleNormal(const Vec3& ccwPosition0, const Vec3& ccwPosition1, const Vec3& ccwPosition2)
{
	Vec3 edge01 = ccwPosition1 - ccwPosition0;
	Vec3 edge12 = ccwPosition2 - ccwPosition1;
	Vec3 normal = CrossProduct(edge01, edge12);
	normal.Normalize();
	return normal;
}

///=====================================================
/// Code from Squirrel Eiserloh
///=====================================================
void EngineAndrew::Mesh::ComputeSurfaceTangentsAtVertex(Vec3& surfaceTangentAtVertex_out, Vec3& surfaceBitangentAtVertex_out, const Vec3& positionOfThisVertex, const Vec2& texCoordsOfThisVertex,
	const Vec3& positionOfPreviousAdjacentVertex, const Vec2& texCoordsOfPreviousAdjacentVertex, const Vec3& positionOfNextAdjacentVertex, const Vec2& texCoordsOfNextAdjacentVertex, bool normalize /*= true*/)
{
	Vec3 vecToPrevious = positionOfPreviousAdjacentVertex - positionOfThisVertex;
	Vec3 vecToNext = positionOfNextAdjacentVertex - positionOfThisVertex;

	Vec2 texToPrevious = texCoordsOfPreviousAdjacentVertex - texCoordsOfThisVertex;
	Vec2 texToNext = texCoordsOfNextAdjacentVertex - texCoordsOfThisVertex;

	Vec3 uDirectionInWorldSpace(
		(texToNext.y * vecToPrevious.x - texToPrevious.y * vecToNext.x),
		(texToNext.y * vecToPrevious.y - texToPrevious.y * vecToNext.y),
		(texToNext.y * vecToPrevious.z - texToPrevious.y * vecToNext.z)
		);

	Vec3 vDirectionInWorldSpace(
		(texToPrevious.x * vecToNext.x - texToNext.x * vecToPrevious.x),
		(texToPrevious.x * vecToNext.y - texToNext.x * vecToPrevious.y),
		(texToPrevious.x * vecToNext.z - texToNext.x * vecToPrevious.z)
		);

	if (normalize){
		uDirectionInWorldSpace.Normalize();
		vDirectionInWorldSpace.Normalize();
	}
	else{
		float determinant = ((texToPrevious.x * texToNext.y) - (texToNext.x * texToPrevious.y));
		float invDeterminant = 1.0f / determinant;
		uDirectionInWorldSpace *= invDeterminant;
		vDirectionInWorldSpace *= invDeterminant;
	}

	surfaceTangentAtVertex_out = uDirectionInWorldSpace;
	surfaceBitangentAtVertex_out = vDirectionInWorldSpace;
}