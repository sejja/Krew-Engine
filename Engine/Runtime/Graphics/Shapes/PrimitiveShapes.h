//
//	MeshRenderer.h
//	Good Neighbours
//
//	Created by Fernando Uribe on 09/17/21
//	Copyright © 2021 Diego Revilla. All Rights reserved
//

#ifndef _GFX_PRIMITIVE_SHAPES__H_
#define _GFX_PRIMITIVE_SHAPES__H_

namespace Engine
{
	enum SimpleShape { plane = 0u, cube, cone, cylinder, sphere, light, particle };
	enum DrawMode { 
		points = 0x0000,
		lines = 0x0001,
		line_loop = 0x0002,
		line_strip = 0x0003,
		triangles = 0x0004,
		triangle_strip = 0x0005,
		triangle_fan = 0x0006
	};

	struct VertexLine
	{
		VertexLine(const glm::vec3& pos, const glm::vec3& c);
		glm::vec3 position;
		glm::vec3 color;
	};

	struct VertexMesh
	{
		VertexMesh(const glm::vec3& pos, const glm::vec2& uv_);
		glm::vec3 position;
		glm::vec2 uv;
		glm::vec3 faceNormal = glm::vec3(0.0f);
		glm::vec3 faceTangent = glm::vec3(0.0f);
		glm::vec3 faceBitangent = glm::vec3(0.0f);
		glm::vec3 avgNormal = glm::vec3(0.0f);
		glm::vec3 avgTangent = glm::vec3(0.0f);
		glm::vec3 avgBitangent = glm::vec3(0.0f);
	};

	struct Mesh
	{
		// mesh Data
		std::vector<VertexMesh> vtx;
		std::vector<Graphics::Texture*> textures;

		// For debugging normals, tangents and bitangents
		std::vector<VertexLine> faceLines;
		std::vector<VertexLine> avgLines;

		// render data 
		unsigned vaoMesh = 0, vboMesh = 0,
			vaoFace = 0, vboFace = 0,
			vaoAvg = 0, vboAvg = 0;

		DrawMode drawMode = triangles;
		int isLight = false;

		glm::vec3 bvMin, bvMax;

		// constructor
		Mesh();
		~Mesh();
		Mesh(std::vector<VertexMesh> vtx0, std::vector<VertexLine> faceLines0, std::vector<VertexLine> avgLines0);
		Mesh(std::vector<VertexMesh> vtx0, std::vector<Graphics::Texture*> tex0);

		// initializes all the buffer objects/arrays
		void SetupMesh();

		// render the mesh
		void Render();
	};

	//struct InstancedMesh : public Mesh
	//{
	//	GLuint position_buffer;
	//	unsigned max_instances;
	//
	//	// constructor
	//	InstancedMesh();
	//	~InstancedMesh();
	//	InstancedMesh(std::vector<VertexMesh> vtx0, std::vector<VertexLine> faceLines0, std::vector<VertexLine> avgLines0);
	//	InstancedMesh(std::vector<VertexMesh> vtx0, std::vector<Graphics::Texture*> tex0);
	//
	//	// initializes all the buffer objects/arrays
	//	void SetupMesh();
	//
	//	// render the mesh
	//	void Render();
	//};

	struct OBJLoader
	{
		OBJLoader();
		~OBJLoader();

		Mesh* CreatePlane() const;
		Mesh* CreateCube() const;
		Mesh* CreateCone(unsigned slices) const;
		Mesh* CreateCylinder(unsigned slices) const;
		Mesh* CreateSphere(unsigned slices, bool isLight = false) const;
		std::vector<glm::vec3> CreateSphere(unsigned slices) const;

		std::map<std::string, Mesh*> meshes;
	};
}

#endif
