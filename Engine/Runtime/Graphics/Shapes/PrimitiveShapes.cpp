#include "Shared.h"

namespace Engine
{

	using namespace glm;

	//Line vertex constructor
	VertexLine::VertexLine(const vec3& pos, const vec3& c) :
		position(pos), color(c)
	{}

	//Mesh vertex constructor
	VertexMesh::VertexMesh(const vec3& pos, const vec2& uv_) :
		position(pos), uv(uv_)
	{}

	Mesh::Mesh() {}

	Mesh::~Mesh()
	{
		// Delete the VBOs
		glDeleteBuffers(1, &vboMesh);
		glDeleteBuffers(1, &vboFace);
		glDeleteBuffers(1, &vboAvg);
		// Delete the VAO
		glDeleteVertexArrays(1, &vaoMesh);
		glDeleteVertexArrays(1, &vaoFace);
		glDeleteVertexArrays(1, &vaoAvg);

		vtx.clear();
		faceLines.clear();
		avgLines.clear();
	}

	// constructor
	Mesh::Mesh(std::vector<VertexMesh> vtx0, std::vector<VertexLine> faceLines0, std::vector<VertexLine> avgLines0)
	{
		vtx = vtx0;
		faceLines = faceLines0;
		avgLines = avgLines0;

		// now that we have all the required data, set the vertex buffers and its attribute pointers.
		SetupMesh();
	}

	Mesh::Mesh(std::vector<VertexMesh> vtx0, std::vector<Graphics::Texture*> tex0)
	{
		vtx = vtx0;
		textures = tex0;

		SetupMesh();
	}

	// initializes all the buffer objects/arrays
	void Mesh::SetupMesh()
	{
		// Delete the VBOs
		glDeleteBuffers(1, &vboMesh);
		glDeleteBuffers(1, &vboFace);
		glDeleteBuffers(1, &vboAvg);
		// Delete the VAO
		glDeleteVertexArrays(1, &vaoMesh);
		glDeleteVertexArrays(1, &vaoFace);
		glDeleteVertexArrays(1, &vaoAvg);

		// create vertex buffers/arrays
		glGenVertexArrays(1, &vaoMesh);
		glBindVertexArray(vaoMesh);

		glGenBuffers(1, &vboMesh);
		glBindBuffer(GL_ARRAY_BUFFER, vboMesh);
		glBufferData(GL_ARRAY_BUFFER, vtx.size() * sizeof(VertexMesh), &vtx[0], GL_STATIC_DRAW);

		// position
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexMesh), (void*)offsetof(VertexMesh, position));

		// uv
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexMesh), (void*)offsetof(VertexMesh, uv));

		// face normal
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexMesh), (void*)offsetof(VertexMesh, faceNormal));
		// face tangent
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexMesh), (void*)offsetof(VertexMesh, faceTangent));
		// face bitangent
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(VertexMesh), (void*)offsetof(VertexMesh, faceBitangent));

		// average normal
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(VertexMesh), (void*)offsetof(VertexMesh, avgNormal));
		// average tangent
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, sizeof(VertexMesh), (void*)offsetof(VertexMesh, avgTangent));
		// average bitangent
		glEnableVertexAttribArray(7);
		glVertexAttribPointer(7, 3, GL_FLOAT, GL_FALSE, sizeof(VertexMesh), (void*)offsetof(VertexMesh, avgBitangent));

		if (!isLight)
		{
			// create face normal buffers/arrays
			glGenVertexArrays(1, &vaoFace);
			glBindVertexArray(vaoFace);

			glGenBuffers(1, &vboFace);
			glBindBuffer(GL_ARRAY_BUFFER, vboFace);
			glBufferData(GL_ARRAY_BUFFER, faceLines.size() * sizeof(VertexLine), &faceLines[0], GL_STATIC_DRAW);

			// line vertex positions
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexLine), (void*)0);

			// line vertex color
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexLine), (void*)offsetof(VertexLine, color));

			// create average normal buffers/arrays
			glGenVertexArrays(1, &vaoAvg);
			glBindVertexArray(vaoAvg);

			glGenBuffers(1, &vboAvg);
			glBindBuffer(GL_ARRAY_BUFFER, vboAvg);
			glBufferData(GL_ARRAY_BUFFER, avgLines.size() * sizeof(VertexLine), &avgLines[0], GL_STATIC_DRAW);

			// line vertex positions
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexLine), (void*)0);

			// line vertex color
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexLine), (void*)offsetof(VertexLine, color));
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		// Bounding volume
		bvMin = vec3(INFINITY);
		bvMax = vec3(-INFINITY);
		for (const auto& v : vtx)
		{
			bvMin = glm::min(bvMin, v.position);
			bvMax = glm::max(bvMax, v.position);
		}
	}

	void Mesh::Render()
	{
		// draw mesh
		glBindVertexArray(vaoMesh);
		glDrawArrays(drawMode, 0, (unsigned)vtx.size());

		// unbind
		glBindVertexArray(0);
	}

	/*InstancedMesh::InstancedMesh() {}

	InstancedMesh::~InstancedMesh()
	{}

	// constructor
	InstancedMesh::InstancedMesh(std::vector<VertexMesh> vtx0, std::vector<VertexLine> faceLines0, std::vector<VertexLine> avgLines0) : Mesh(vtx0, faceLines, avgLines0)
	{}

	InstancedMesh::InstancedMesh(std::vector<VertexMesh> vtx0, std::vector<Graphics::Texture*> tex0) : Mesh(vtx0, tex0)
	{}

	// initializes all the buffer objects/arrays
	void InstancedMesh::SetupMesh()
	{
		// Delete the VBOs
		glDeleteBuffers(1, &vboMesh);
		// Delete the VAO
		glDeleteVertexArrays(1, &vaoMesh);

		// create vertex buffers/arrays
		glGenVertexArrays(1, &vaoMesh);
		glBindVertexArray(vaoMesh);

		glGenBuffers(1, &vboMesh);
		glBindBuffer(GL_ARRAY_BUFFER, vboMesh);
		glBufferData(GL_ARRAY_BUFFER, vtx.size() * sizeof(VertexMesh), &vtx[0], GL_STATIC_DRAW);

		// position buffer
		glGenBuffers(1, &position_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, position_buffer);
		// Initialize with empty (NULL) buffer : it will be updated later, each frame.
		glBufferData(GL_ARRAY_BUFFER, max_instances * sizeof(glm::vec3), NULL, GL_STREAM_DRAW);

		// position
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexMesh), (void*)offsetof(VertexMesh, position));

		// uv
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexMesh), (void*)offsetof(VertexMesh, uv));

		// face normal
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexMesh), (void*)offsetof(VertexMesh, faceNormal));
		// face tangent
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexMesh), (void*)offsetof(VertexMesh, faceTangent));
		// face bitangent
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(VertexMesh), (void*)offsetof(VertexMesh, faceBitangent));

		// average normal
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(VertexMesh), (void*)offsetof(VertexMesh, avgNormal));
		// average tangent
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, sizeof(VertexMesh), (void*)offsetof(VertexMesh, avgTangent));
		// average bitangent
		glEnableVertexAttribArray(7);
		glVertexAttribPointer(7, 3, GL_FLOAT, GL_FALSE, sizeof(VertexMesh), (void*)offsetof(VertexMesh, avgBitangent));


		// Bounding volume
		bvMin = vec3(INFINITY);
		bvMax = vec3(-INFINITY);
		for (unsigned i = 0; i < 3; ++i)
			for (const auto& v : vtx)
			{
				if (bvMin[i] > v.position[i])
					bvMin[i] = v.position[i];
				if (bvMax[i] < v.position[i])
					bvMax[i] = v.position[i];
			}
	}

	void Mesh::Render()
	{
		// draw mesh
		glBindVertexArray(vaoMesh);
		glDrawArrays(drawMode, 0, (unsigned)vtx.size());

		// unbind
		glBindVertexArray(0);
	}*/

	bool isEqualVec3(const vec3& a, const vec3& b)
	{
		return abs(a.x - b.x) < EPSILON && abs(a.y - b.y) < EPSILON && abs(a.z - b.z) < EPSILON;
	}

	//Helper class for hashing the unordered map of normals(vectors)
	struct KeyFuncs
	{
		size_t operator()(const vec3& k)const
		{
			return std::hash<int>()((int)k.x) ^ std::hash<int>()((int)k.y) ^ std::hash<int>()((int)k.z);
		}

		bool operator()(const vec3& a, const vec3& b)const
		{
			return isEqualVec3(a, b);
		}
	};

	//GetNormal Helper function
	vec3 GetNormal(VertexMesh& p0, VertexMesh& p1, VertexMesh& p2)
	{
		vec3 p01(p1.position - p0.position);
		vec3 p02(p2.position - p0.position);
		vec3 n = normalize(cross(p01, p02));

		p0.faceNormal = n;
		p1.faceNormal = n;
		p2.faceNormal = n;

		return n;
	}

	void GramSchmidt(vec3& n, vec3& t, vec3& b)
	{
		n = normalize(n);
		// Gram-Schmidt orthogonalization
		vec3 orthoT = t - dot(t, n) * n;

		// Set tangent to (1,0,0) when lenght is 0
		t = length(orthoT) > 0.0f ?
			normalize(orthoT) :
			vec3(1.0f, 0.0f, 0.0f);

		// Compute the new perpendicular bitangent
		vec3 orthoB = cross(n, t);

		b = dot(orthoB, b) > 0.0f ?
			normalize(orthoB) :
			-normalize(orthoB);
	}

	void ComputeVectors(Mesh& mesh)
	{
		auto& vtx = mesh.vtx;
		auto& faceLines = mesh.faceLines;
		auto& avgLines = mesh.avgLines;

		// Loop through the triangles (using the index buffer)
		for (unsigned i = 0; i < vtx.size(); i += 3)
		{
			VertexMesh& v0 = vtx[i];
			VertexMesh& v1 = vtx[i + 1];
			VertexMesh& v2 = vtx[i + 2];

			// Find T and B
			const vec3 v10 = v1.position - v0.position;
			const vec2 uv10 = v1.uv - v0.uv;
			const vec3 v20 = v2.position - v0.position;
			const vec2 uv20 = v2.uv - v0.uv;
			const float div = (uv10.y * uv20.x - uv20.y * uv10.x);
			const float mul = div ? 1.0f / div : 0;
			const vec3 t = (uv10.y * v20 - uv20.y * v10) * mul;
			const vec3 b = (uv20.x * v10 - uv10.x * v20) * mul;

			// Accumulate
			v0.faceTangent += t;
			v1.faceTangent += t;
			v2.faceTangent += t;
			v0.faceBitangent += b;
			v1.faceBitangent += b;
			v2.faceBitangent += b;
		}

		//Face normals
		for (unsigned i = 0; i < vtx.size(); i += 3)
			GetNormal(vtx[i], vtx[i + 1], vtx[i + 2]);

		std::unordered_map<vec3, std::pair<vec3, std::pair<vec3, vec3> >, KeyFuncs, KeyFuncs> avgVectors;
		std::unordered_map<vec3, std::vector<vec3>, KeyFuncs, KeyFuncs> doneNormals;

		// Loop through every vertex
		for (unsigned i = 0; i < vtx.size(); i++)
		{
			const vec3& pos = vtx[i].position;
			vec3& nFace = vtx[i].faceNormal;
			vec3& tFace = vtx[i].faceTangent;
			vec3& bFace = vtx[i].faceBitangent;

			GramSchmidt(nFace, tFace, bFace);

			auto it = avgVectors.begin();
			for (; it != avgVectors.end(); it++)
				if (isEqualVec3(it->first, pos))
					break;
			if (it == avgVectors.end())
			{
				doneNormals[pos].push_back(nFace);
				avgVectors[pos] = std::make_pair(nFace, std::make_pair(tFace, bFace));
				//Face normal
				faceLines.push_back({ pos, Color::blue });
				faceLines.push_back({ pos + nFace / 3.0f, Color::blue });
				//Face tangent
				faceLines.push_back({ pos, Color::red });
				faceLines.push_back({ pos + tFace / 3.0f, Color::red });
				//Face bitangent
				faceLines.push_back({ pos, Color::green });
				faceLines.push_back({ pos + bFace / 3.0f, Color::green });
			}
			else
			{
				auto& n = doneNormals[pos];
				auto dup = n.begin();
				for (; dup != n.end(); dup++)
					if (isEqualVec3(*dup, nFace))
						break;
				if (dup == n.end())
				{
					n.push_back(nFace);
					it->second.first += nFace;
					it->second.second.first += tFace;
					it->second.second.second += bFace;
					//Face normal
					faceLines.push_back({ pos, Color::blue });
					faceLines.push_back({ pos + nFace / 3.0f, Color::blue });
					//Face tangent
					faceLines.push_back({ pos, Color::red });
					faceLines.push_back({ pos + tFace / 3.0f, Color::red });
					//Face bitangent
					faceLines.push_back({ pos, Color::green });
					faceLines.push_back({ pos + bFace / 3.0f, Color::green });
				}
			}
		}
		for (auto& it : avgVectors)
		{
			const vec3& pos = it.first;
			vec3& nAvg = it.second.first;
			vec3& tAvg = it.second.second.first;
			vec3& bAvg = it.second.second.second;
			GramSchmidt(nAvg, tAvg, bAvg);
			avgLines.push_back({ pos, Color::blue });
			avgLines.push_back({ pos + nAvg / 3.0f, Color::blue });
			avgLines.push_back({ pos, Color::red });
			avgLines.push_back({ pos + tAvg / 3.0f, Color::red });
			avgLines.push_back({ pos, Color::green });
			avgLines.push_back({ pos + bAvg / 3.0f, Color::green });
		}
		for (auto& v : vtx)
		{
			auto& vAvg = avgVectors[v.position];
			v.avgNormal = vAvg.first;
			v.avgTangent = vAvg.second.first;
			v.avgBitangent = vAvg.second.second;
		}
	}

	OBJLoader::OBJLoader()
	{
		meshes[meshTypes[0]] = CreatePlane();
		meshes[meshTypes[1]] = CreateCube();
		meshes[meshTypes[2]] = CreateCone(24u);
		meshes[meshTypes[3]] = CreateCylinder(24u);
		meshes[meshTypes[4]] = CreateSphere(24u, false);
		meshes[meshTypes[5]] = CreateSphere(6u, true);
		meshes[meshTypes[6]] = CreateSphere(6u, false);
	}

	OBJLoader::~OBJLoader()
	{
		for (auto m : meshes)
			delete m.second;
		meshes.clear();
	}

	//Object plane mesh
	Mesh* OBJLoader::CreatePlane() const
	{
		Mesh *mesh = new Mesh;
		auto& vtx = mesh->vtx;

		VertexMesh p0(vec3(-0.5f, -0.5f, 0.0f), vec2(0.0f, 0.0f));
		VertexMesh p1(vec3(0.5f, -0.5f, 0.0f), vec2(1.0f, 0.0f));
		VertexMesh p2(vec3(0.5f, 0.5f, 0.0f), vec2(1.0f, 1.0f));
		VertexMesh p3(vec3(-0.5f, 0.5f, 0.0f), vec2(0.0f, 1.0f));

		//Triangle 1
		vtx.push_back(p0);
		vtx.push_back(p1);
		vtx.push_back(p2);

		//Triangle 2
		vtx.push_back(p0);
		vtx.push_back(p2);
		vtx.push_back(p3);

		//Triangle 3
		vtx.push_back(p0);
		vtx.push_back(p2);
		vtx.push_back(p1);

		//Triangle 4
		vtx.push_back(p0);
		vtx.push_back(p3);
		vtx.push_back(p2);

		ComputeVectors(*mesh);
		mesh->SetupMesh();
		return mesh;
	}

	//Object cube mesh
	Mesh* OBJLoader::CreateCube() const
	{
		Mesh* mesh = new Mesh;
		auto& vtx = mesh->vtx;

		vec3 p0(-0.5f, -0.5f, 0.5f);
		vec3 p1(0.5f, -0.5f, 0.5f);
		vec3 p2(0.5f, 0.5f, 0.5f);
		vec3 p3(-0.5f, 0.5f, 0.5f);
		vec3 p4(-0.5f, -0.5f, -0.5f);
		vec3 p5(0.5f, -0.5f, -0.5f);
		vec3 p6(0.5f, 0.5f, -0.5f);
		vec3 p7(-0.5f, 0.5f, -0.5f);

		//Near faces
		vtx.push_back(VertexMesh(p0, vec2(0.0f, 0.0f)));
		vtx.push_back(VertexMesh(p1, vec2(1.0f, 0.0f)));
		vtx.push_back(VertexMesh(p2, vec2(1.0f, 1.0f)));
		vtx.push_back(VertexMesh(p0, vec2(0.0f, 0.0f)));
		vtx.push_back(VertexMesh(p2, vec2(1.0f, 1.0f)));
		vtx.push_back(VertexMesh(p3, vec2(0.0f, 1.0f)));

		//Right faces
		vtx.push_back(VertexMesh(p1, vec2(0.0f, 0.0f)));
		vtx.push_back(VertexMesh(p5, vec2(1.0f, 0.0f)));
		vtx.push_back(VertexMesh(p6, vec2(1.0f, 1.0f)));
		vtx.push_back(VertexMesh(p1, vec2(0.0f, 0.0f)));
		vtx.push_back(VertexMesh(p6, vec2(1.0f, 1.0f)));
		vtx.push_back(VertexMesh(p2, vec2(0.0f, 1.0f)));

		//Far faces
		vtx.push_back(VertexMesh(p5, vec2(0.0f, 0.0f)));
		vtx.push_back(VertexMesh(p4, vec2(1.0f, 0.0f)));
		vtx.push_back(VertexMesh(p7, vec2(1.0f, 1.0f)));
		vtx.push_back(VertexMesh(p5, vec2(0.0f, 0.0f)));
		vtx.push_back(VertexMesh(p7, vec2(1.0f, 1.0f)));
		vtx.push_back(VertexMesh(p6, vec2(0.0f, 1.0f)));

		//Left faces
		vtx.push_back(VertexMesh(p4, vec2(0.0f, 0.0f)));
		vtx.push_back(VertexMesh(p0, vec2(1.0f, 0.0f)));
		vtx.push_back(VertexMesh(p3, vec2(1.0f, 1.0f)));
		vtx.push_back(VertexMesh(p4, vec2(0.0f, 0.0f)));
		vtx.push_back(VertexMesh(p3, vec2(1.0f, 1.0f)));
		vtx.push_back(VertexMesh(p7, vec2(0.0f, 1.0f)));

		//Top faces
		vtx.push_back(VertexMesh(p2, vec2(0.0f, 0.0f)));
		vtx.push_back(VertexMesh(p6, vec2(1.0f, 0.0f)));
		vtx.push_back(VertexMesh(p7, vec2(1.0f, 1.0f)));
		vtx.push_back(VertexMesh(p2, vec2(0.0f, 0.0f)));
		vtx.push_back(VertexMesh(p7, vec2(1.0f, 1.0f)));
		vtx.push_back(VertexMesh(p3, vec2(0.0f, 1.0f)));

		//Bot faces
		vtx.push_back(VertexMesh(p0, vec2(0.0f, 0.0f)));
		vtx.push_back(VertexMesh(p4, vec2(1.0f, 0.0f)));
		vtx.push_back(VertexMesh(p5, vec2(1.0f, 1.0f)));
		vtx.push_back(VertexMesh(p0, vec2(0.0f, 0.0f)));
		vtx.push_back(VertexMesh(p5, vec2(1.0f, 1.0f)));
		vtx.push_back(VertexMesh(p1, vec2(0.0f, 1.0f)));

		ComputeVectors(*mesh);
		mesh->SetupMesh();
		return mesh;
	}

	//Object cone mesh
	Mesh* OBJLoader::CreateCone(unsigned slices) const
	{
		Mesh* mesh = new Mesh;
		auto& vtx = mesh->vtx;

		//Angle increment
		float a = aLoop / (float)slices;
		//UV.x increment
		float u = 1.0f / (float)slices;

		vec3 p0(0.0f, 0.5f, 0.0f);
		vec3 p1(0.0f, -0.5f, 0.0f);

		for (unsigned i = 0; i < slices; i++)
		{
			VertexMesh B0({ 0.5f * cos(a * i), -0.5f, 0.5f * sin(a * i) }, { u * i, 0.0f });
			VertexMesh B1({ 0.5f * cos(a * (i + 1)), -0.5f, 0.5f * sin(a * (i + 1)) }, { u * (i + 1), 0.0f });
			vtx.push_back(VertexMesh(p0, vec2(u * i, 1.0f)));
			vtx.push_back(B1);
			vtx.push_back(B0);

			vtx.push_back(VertexMesh(p1, vec2(u * i, 0.0f)));
			vtx.push_back(B0);
			vtx.push_back(B1);
		}

		ComputeVectors(*mesh);
		mesh->SetupMesh();
		return mesh;
	}

	//Object cylinder mesh
	Mesh* OBJLoader::CreateCylinder(unsigned slices) const
	{
		Mesh* mesh = new Mesh;
		auto& vtx = mesh->vtx;

		//Angle increment
		float a = aLoop / (float)slices;
		//UV.x increment
		float u = 1.0f / (float)slices;

		vec3 p0(0.0f, 0.5f, 0.0f);
		vec3 p1(0.0f, -0.5f, 0.0f);

		for (unsigned i = 0; i < slices; i++)
		{
			VertexMesh T0({ 0.5f * cos(a * i), 0.5f, 0.5f * sin(a * i) }, { u * i, 1.0f });
			VertexMesh T1({ 0.5f * cos(a * (i + 1)), 0.5f, 0.5f * sin(a * (i + 1)) }, { u * (i + 1), 1.0f });
			VertexMesh B0({ 0.5f * cos(a * i), -0.5f, 0.5f * sin(a * i) }, { u * i, 0.0f });
			VertexMesh B1({ 0.5f * cos(a * (i + 1)), -0.5f, 0.5f * sin(a * (i + 1)) }, { u * (i + 1), 0.0f });

			vtx.push_back(VertexMesh(p0, vec2(u * i, 1.0f)));
			vtx.push_back(T1);
			vtx.push_back(T0);

			vtx.push_back(T0);
			vtx.push_back(B1);
			vtx.push_back(B0);

			vtx.push_back(T0);
			vtx.push_back(T1);
			vtx.push_back(B1);

			vtx.push_back(VertexMesh(p1, vec2(u * i, 0.0f)));
			vtx.push_back(B0);
			vtx.push_back(B1);
		}

		ComputeVectors(*mesh);
		mesh->SetupMesh();
		return mesh;
	}

	//Object sphere mesh
	Mesh* OBJLoader::CreateSphere(unsigned slices, bool isLight) const
	{
		Mesh* mesh = new Mesh;
		mesh->isLight = isLight;
		auto& vtx = mesh->vtx;

		unsigned rings = (unsigned)ceil(slices / 2.0f);
		//Angle increments
		float s = aLoop / (float)slices;
		float r = (aLoop / 2.0f) / (float)rings;
		//UV increments
		float u = 1.0f / (float)slices;
		float v = 1.0f / (float)rings;

		for (unsigned j = 0; j < rings; j++)
			for (unsigned i = 0; i < slices; i++)
			{
				VertexMesh V1({
						0.5f * cos(s * i) * sin(r * j),
						0.5f * cos(r * j),
						0.5f * sin(s * i) * sin(r * j) },
					{ u * i, 1 - v * j });
				VertexMesh V4({
						0.5f * cos(s * (i + 1)) * sin(r * (j + 1)),
						0.5f * cos(r * (j + 1)),
						0.5f * sin(s * (i + 1)) * sin(r * (j + 1)) },
					{ u * (i + 1), 1 - v * (j + 1) });

				if (j != rings - 1)
				{
					vtx.push_back(V1);
					vtx.push_back(V4);
					vtx.push_back(VertexMesh({ 0.5f * cos(s * i) * sin(r * (j + 1)), 0.5f * cos(r * (j + 1)), 0.5f * sin(s * i) * sin(r * (j + 1)) }, { u * i, 1 - v * (j + 1) }));
				}
				if (j != 0)
				{
					vtx.push_back(V1);
					vtx.push_back(VertexMesh({ 0.5f * cos(s * (i + 1)) * sin(r * j), 0.5f * cos(r * j), 0.5f * sin(s * (i + 1)) * sin(r * j) }, { u * (i + 1), 1 - v * j }));
					vtx.push_back(V4);
				}
			}

		if (!isLight) ComputeVectors(*mesh);
		mesh->SetupMesh();
		return mesh;
	}

	std::vector<glm::vec3> OBJLoader::CreateSphere(unsigned slices) const
	{
		std::vector<glm::vec3> vtx;

		unsigned rings = (unsigned)ceil(slices / 2.0f);
		//Angle increments
		float s = aLoop / (float)slices;
		float r = (aLoop / 2.0f) / (float)rings;
		//UV increments
		float u = 1.0f / (float)slices;
		float v = 1.0f / (float)rings;

		for (unsigned j = 0; j < rings; j++)
		{
			for (unsigned i = 0; i < slices; i++)
			{
				glm::vec3 V1({
						0.5f * cos(s * i) * sin(r * j),
						0.5f * cos(r * j),
						0.5f * sin(s * i) * sin(r * j) });
				glm::vec3 V4({
						0.5f * cos(s * (i + 1)) * sin(r * (j + 1)),
						0.5f * cos(r * (j + 1)),
						0.5f * sin(s * (i + 1)) * sin(r * (j + 1)) });

				if (j != rings - 1)
				{
					vtx.push_back(V1);
					vtx.push_back(V4);
					vtx.push_back(glm::vec3({ 0.5f * cos(s * i) * sin(r * (j + 1)), 0.5f * cos(r * (j + 1)), 0.5f * sin(s * i) * sin(r * (j + 1)) }));
				}
				if (j != 0)
				{
					vtx.push_back(V1);
					vtx.push_back(glm::vec3({ 0.5f * cos(s * (i + 1)) * sin(r * j), 0.5f * cos(r * j), 0.5f * sin(s * (i + 1)) * sin(r * j) }));
					vtx.push_back(V4);
				}
			}
		}

		return vtx;
	}
}
