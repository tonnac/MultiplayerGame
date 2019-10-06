#include "GeometryGenerator.h"
#include "MathHelper.h"
#include <array>

#define M

using MeshData = GeometryGenerator::MeshData;
using Vertex = GeometryGenerator::Vertex;
using std::array;
using namespace DirectX;

MeshData GeometryGenerator::CreateBox(float width, float height, float depth, uint32 numSubdivisions)
{
	MeshData meshData;

	float halfWidth = width * 0.5f;
	float halfHeight = height * 0.5f;
	float halfDepth = depth * 0.5f;

	array<Vertex, 24> box = {
		//Front
		Vertex(XMFLOAT3(-halfWidth,	-halfHeight, -halfDepth), XMFLOAT4(0.f, 0.f ,0.f ,1.f), XMFLOAT2(0.f, 1.f)),
		Vertex(XMFLOAT3(-halfWidth,	halfHeight, -halfDepth), XMFLOAT4(0.f, 0.f ,0.f ,1.f), XMFLOAT2(0.f, 0.f)),
		Vertex(XMFLOAT3(halfWidth,	halfHeight, -halfDepth), XMFLOAT4(0.f, 0.f ,0.f ,1.f), XMFLOAT2(1.f, 0.f)),
		Vertex(XMFLOAT3(halfWidth,	-halfHeight, -halfDepth), XMFLOAT4(0.f, 0.f ,0.f ,1.f), XMFLOAT2(1.f, 1.f)),
		//Up
		Vertex(XMFLOAT3(-halfWidth, halfHeight, -halfDepth), XMFLOAT4(0.f, 0.f ,0.f ,1.f), XMFLOAT2(0.f, 1.f)),
		Vertex(XMFLOAT3(-halfWidth, halfHeight,	halfDepth), XMFLOAT4(0.f, 0.f ,0.f ,1.f), XMFLOAT2(0.f, 0.f)),
		Vertex(XMFLOAT3(halfWidth,	halfHeight,	halfDepth), XMFLOAT4(0.f, 0.f ,0.f ,1.f), XMFLOAT2(1.f, 0.f)),
		Vertex(XMFLOAT3(halfWidth,	halfHeight, -halfDepth), XMFLOAT4(0.f, 0.f ,0.f ,1.f), XMFLOAT2(1.f, 1.f)),
		//Left
		Vertex(XMFLOAT3(-halfWidth, -halfHeight, halfDepth), XMFLOAT4(0.f, 0.f ,0.f ,1.f), XMFLOAT2(0.f, 1.f)),
		Vertex(XMFLOAT3(-halfWidth, halfHeight,	halfDepth), XMFLOAT4(0.f, 0.f ,0.f ,1.f), XMFLOAT2(0.f, 0.f)),
		Vertex(XMFLOAT3(-halfWidth, halfHeight,	-halfDepth), XMFLOAT4(0.f, 0.f ,0.f ,1.f), XMFLOAT2(1.f, 0.f)),
		Vertex(XMFLOAT3(-halfWidth, -halfHeight, -halfDepth), XMFLOAT4(0.f, 0.f ,0.f ,1.f), XMFLOAT2(1.f, 1.f)),
		//Right
		Vertex(XMFLOAT3(halfWidth, -halfHeight,	-halfDepth), XMFLOAT4(0.f, 0.f ,0.f ,1.f), XMFLOAT2(0.f, 1.f)),
		Vertex(XMFLOAT3(halfWidth,	halfHeight,	-halfDepth), XMFLOAT4(0.f, 0.f ,0.f ,1.f), XMFLOAT2(0.f, 0.f)),
		Vertex(XMFLOAT3(halfWidth,	halfHeight,	halfDepth), XMFLOAT4(0.f, 0.f ,0.f ,1.f), XMFLOAT2(1.f, 0.f)),
		Vertex(XMFLOAT3(halfWidth, -halfHeight,	halfDepth), XMFLOAT4(0.f, 0.f ,0.f ,1.f), XMFLOAT2(1.f, 1.f)),
		//Back
		Vertex(XMFLOAT3(-halfWidth,	-halfHeight, halfDepth), XMFLOAT4(0.f, 0.f ,0.f ,1.f), XMFLOAT2(0.f, 1.f)),
		Vertex(XMFLOAT3(halfWidth,	-halfHeight, halfDepth), XMFLOAT4(0.f, 0.f ,0.f ,1.f), XMFLOAT2(0.f, 0.f)),
		Vertex(XMFLOAT3(halfWidth,	halfHeight,	halfDepth), XMFLOAT4(0.f, 0.f ,0.f ,1.f), XMFLOAT2(1.f, 0.f)),
		Vertex(XMFLOAT3(-halfWidth, halfHeight,	halfDepth), XMFLOAT4(0.f, 0.f ,0.f ,1.f), XMFLOAT2(1.f, 1.f)),
		//Down
		Vertex(XMFLOAT3(-halfWidth,	-halfHeight, -halfDepth), XMFLOAT4(0.f, 0.f ,0.f ,1.f), XMFLOAT2(0.f, 1.f)),
		Vertex(XMFLOAT3(halfWidth,	-halfHeight, -halfDepth), XMFLOAT4(0.f, 0.f ,0.f ,1.f), XMFLOAT2(0.f, 0.f)),
		Vertex(XMFLOAT3(halfWidth,	-halfHeight, halfDepth), XMFLOAT4(0.f, 0.f ,0.f ,1.f), XMFLOAT2(1.f, 0.f)),
		Vertex(XMFLOAT3(-halfWidth,	-halfHeight, halfDepth), XMFLOAT4(0.f, 0.f ,0.f ,1.f), XMFLOAT2(1.f, 1.f))
	};

	meshData.Vertices.assign(&(box.data())[0], &(box.data())[box.size()]);

	array<uint16_t, 36> indicies = {
		0, 1, 2 ,
		0, 2, 3 ,

		4, 5, 6 ,
		4, 6, 7 ,

		8, 9, 10 ,
		8, 10, 11 ,

		12, 13, 14 ,
		12, 14, 15 ,

		16, 17, 18 ,
		16, 18, 19 ,

		20, 21, 22 ,
		20, 22, 23
	};
	
	meshData.Indices32.assign(&(indicies.data())[0], &(indicies.data())[indicies.size()]);

	numSubdivisions = std::min<uint32>(numSubdivisions, MaxSubDivisions);

	for (uint32 i = 0; i < numSubdivisions; ++i)
	{
		Subdivide(meshData);
	}

	return meshData;
}

MeshData GeometryGenerator::CreateSphere(float radius, uint32 sliceCount, uint32 stackCount)
{
	MeshData meshData;

	Vertex top(XMFLOAT3(0.f, radius, 0.f), XMFLOAT4(0.f, 0.f, 0.f, 1.f), XMFLOAT2(0.0f, 0.0f));
	Vertex bottom(XMFLOAT3(0.f, -radius, 0.f), XMFLOAT4(0.f, 0.f, 0.f, 1.f), XMFLOAT2(0.0f, 1.0f));

	meshData.Vertices.push_back(top);

	float thetaStep = XM_2PI / sliceCount;
	float phiStep = XM_PI / stackCount;

	for (int i = 1; i <= stackCount - 1; ++i)
	{
		float phi = phiStep * i;
		float d0 = XMConvertToDegrees(phi);

		for (int k = 0; k <= sliceCount; ++k)
		{
			float theta = thetaStep * k;

			float x, y, z;
			x = radius * sinf(phi) * cosf(theta);
			y = radius * cosf(phi);
			z = radius * sinf(phi) * sinf(theta);
			Vertex v(XMFLOAT3(x, y, z), XMFLOAT4(0.f, 0.f, 0.f, 1.f), XMFLOAT2(theta / XM_2PI, phi / XM_PI));
			meshData.Vertices.push_back(v);
		}
	}

	meshData.Vertices.push_back(bottom);

	for (int i = 1; i <= sliceCount; ++i)
	{
		meshData.Indices32.push_back(i);
		meshData.Indices32.push_back(0);
		meshData.Indices32.push_back(i + 1);
	}

	uint32 baseIndex = 1;
	uint32 ringVertexCount = sliceCount + 1;

	for (int i = 0; i < stackCount - 2; ++i)
	{
		for (int k = 0; k <= sliceCount; ++k)
		{
			int i0 = (i + 1) * (sliceCount + 1) + k + 1;
			int i1 = i * (sliceCount + 1) + k + 1;
			int i2 = i1 + 1;
			int i3 = i0 + 1;

			meshData.Indices32.push_back(i0);
			meshData.Indices32.push_back(i1);
			meshData.Indices32.push_back(i2);

			meshData.Indices32.push_back(i0);
			meshData.Indices32.push_back(i2);
			meshData.Indices32.push_back(i3);
		}
	}

	int bottomIndex = (int)meshData.Vertices.size() - 1;

	for (int i = 1; i <= sliceCount; ++i)
	{
		auto i0 = (stackCount - 2) * (sliceCount + 1) + i;
		auto i1 = (stackCount - 2) * (sliceCount + 1) + i + 1;

		meshData.Indices32.push_back(bottomIndex);
		meshData.Indices32.push_back(i0);
		meshData.Indices32.push_back(i1);
	}

	return meshData;
}

MeshData GeometryGenerator::CreateGeosphere(float radius, uint32 numSubdivisions)
{
	static constexpr float X = 0.525731f;
	static constexpr float Z = 0.850651f;

	MeshData meshData;

	numSubdivisions = std::min<uint32>(numSubdivisions, MaxSubDivisions);

	XMFLOAT3 pos[12] =
	{
		XMFLOAT3(-X, 0.0f, Z),  XMFLOAT3(X, 0.0f, Z),
		XMFLOAT3(-X, 0.0f, -Z), XMFLOAT3(X, 0.0f, -Z),
		XMFLOAT3(0.0f, Z, X),   XMFLOAT3(0.0f, Z, -X),
		XMFLOAT3(0.0f, -Z, X),  XMFLOAT3(0.0f, -Z, -X),
		XMFLOAT3(Z, X, 0.0f),   XMFLOAT3(-Z, X, 0.0f),
		XMFLOAT3(Z, -X, 0.0f),  XMFLOAT3(-Z, -X, 0.0f)
	};

	uint32 k[60] =
	{
		1,4,0,  4,9,0,  4,5,9,  8,5,4,  1,8,4,
		1,10,8, 10,3,8, 8,3,5,  3,2,5,  3,7,2,
		3,10,7, 10,6,7, 6,11,7, 6,0,11, 6,1,0,
		10,1,6, 11,0,9, 2,11,9, 5,2,9,  11,2,7
	};

	meshData.Vertices.resize(12);
	meshData.Indices32.assign(&k[0], &k[60]);

	for (uint32 i = 0; i < 12; ++i)
		meshData.Vertices[i].Position = pos[i];

	for (uint32 i = 0; i < numSubdivisions; ++i)
		Subdivide(meshData);

	for (uint32 i = 0; i < meshData.Vertices.size(); ++i)
	{
		XMVECTOR n = XMVector3Normalize(XMLoadFloat3(&meshData.Vertices[i].Position));

		XMStoreFloat3(&meshData.Vertices[i].Position, n * radius);

		float theta = atan2f(meshData.Vertices[i].Position.z, meshData.Vertices[i].Position.x);

		// Put in [0, 2pi].
		if (theta < 0.0f)
			theta += XM_2PI;

		float phi = acosf(meshData.Vertices[i].Position.y / radius);

		meshData.Vertices[i].TexC.x = theta / XM_2PI;
		meshData.Vertices[i].TexC.y = phi / XM_PI;
	}

	return meshData;
}

MeshData GeometryGenerator::CreateCylinder(float bottomRadius, float topRadius, float height, uint32 sliceCount, uint32 stackCount)
{
	MeshData meshData;
	BuildCylinderTopCap(bottomRadius, topRadius, height, sliceCount, stackCount, meshData);
	
	float heightStep = height / stackCount;
	float thetaStep = XM_2PI / sliceCount;

	for (int i = 0; i <= stackCount; ++i)
	{
		float y = (height * 0.5f) - (heightStep * i);
		float radius = MathHelper::Lerp(topRadius, bottomRadius, (float)i / stackCount);
		for (int k = 0; k <= sliceCount; ++k)
		{
			float theta = thetaStep * k;
			float x = radius * cosf(theta);
			float z = radius * sinf(theta);

			Vertex v(XMFLOAT3(x, y, z), XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f), XMFLOAT2((float)k / sliceCount, (float)i / stackCount));
			meshData.Vertices.push_back(v);
		}
	}

	for (int i = 0; i < stackCount; ++i)
	{
		for (int k = 0; k < sliceCount; ++k)
		{
			int i0 = (i + 1) * (sliceCount + 1) + k + 1;
			int i1 = (i + 1) * (sliceCount + 1) + k + 2;
			int i2 = (i + 2) * (sliceCount + 1) + k + 1;
			int i3 = (i + 2) * (sliceCount + 1) + k + 2;

			meshData.Indices32.push_back(i2);
			meshData.Indices32.push_back(i0);
			meshData.Indices32.push_back(i1);

			meshData.Indices32.push_back(i2);
			meshData.Indices32.push_back(i1);
			meshData.Indices32.push_back(i3);
		}
	}

	BuildCylinderBottomCap(bottomRadius, topRadius, height, sliceCount, stackCount, meshData);

	return meshData;
}

void GeometryGenerator::BuildCylinderTopCap(float bottomRadius, float topRadius, float height, uint32 sliceCount, uint32 stackCount, MeshData& meshData)
{
	float y = height * 0.5f;

	Vertex v(XMFLOAT3(0.0f, y, 0.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f), XMFLOAT2(0.5f, 0.5f));

	meshData.Vertices.push_back(v);
	float thetaStep = XM_2PI / sliceCount;

	for (int i = 0; i <= sliceCount; ++i)
	{
		float theta = thetaStep * i;
		float x = topRadius * cosf(theta);
		float z = topRadius * sinf(theta);

		float u = (x / height) * 0.5f + 0.5f;
		float v = (z / height) * 0.5f + 0.5f;
		Vertex v0(XMFLOAT3(x, y, z), XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f), XMFLOAT2(u, v));
		meshData.Vertices.push_back(v0);
	}

	for (int i = 0; i < sliceCount; ++i)
	{
		int i0 = 0;
		int i1 = i + 1;
		int i2 = i + 2;

		meshData.Indices32.push_back(i0);
		meshData.Indices32.push_back(i2);
		meshData.Indices32.push_back(i1);
	}
}

void GeometryGenerator::BuildCylinderBottomCap(float bottomRadius, float topRadius, float height, uint32 sliceCount, uint32 stackCount, MeshData& meshData)
{
	float thetaStep = XM_2PI / sliceCount;

	uint32_t baseindex = (uint32_t)meshData.Vertices.size();

	for (int i = 0; i <= sliceCount; ++i)
	{
		float theta = thetaStep * i;
		float x = bottomRadius * cosf(theta);
		float z = bottomRadius * sinf(theta);

		float u = (x / height) * 0.5f + 0.5f;
		float v = (z / height) * 0.5f + 0.5f;

		Vertex v0(XMFLOAT3(x, -height * 0.5f, z), XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f), XMFLOAT2(u, v));
		meshData.Vertices.push_back(v0);
	}

	Vertex v(XMFLOAT3(0.0f, -height * 0.5f, 0.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f), XMFLOAT2(0.5f, 0.5f));

	meshData.Vertices.push_back(v);
	uint32_t bottomindex = (uint32_t)meshData.Vertices.size() - 1;

	for (int i = 0; i < sliceCount; ++i)
	{
		meshData.Indices32.push_back(bottomindex);
		meshData.Indices32.push_back(baseindex + i);
		meshData.Indices32.push_back(baseindex + i + 1);
	}
}

MeshData GeometryGenerator::CreateGrid(float width, float depth, uint32 m, uint32 n)
{
	return MeshData();
}

MeshData GeometryGenerator::CreateQuad(float x, float y, float w, float h, float depth)
{
	return MeshData();
}

MeshData GeometryGenerator::CreateCapsule(float radius, float height, uint32 sliceCount)
{
	return MeshData();
}

void GeometryGenerator::Subdivide(MeshData& meshData)
{
	MeshData copy = meshData;

	meshData.Vertices.resize(0);
	meshData.Indices32.resize(0);

	int triNum = static_cast<int>(copy.Indices32.size() / 3);

	for (int i = 0; i < triNum; ++i)
	{
		Vertex v0 = copy.Vertices[copy.Indices32[(size_t)i * 3 + 0]];
		Vertex v1 = copy.Vertices[copy.Indices32[(size_t)i * 3 + 1]];
		Vertex v2 = copy.Vertices[copy.Indices32[(size_t)i * 3 + 2]];

		Vertex m0 = MidPoint(v0, v1);
		Vertex m1 = MidPoint(v1, v2);
		Vertex m2 = MidPoint(v0, v2);

		meshData.Vertices.push_back(v0);
		meshData.Vertices.push_back(m0);
		meshData.Vertices.push_back(m2);
		meshData.Vertices.push_back(v1);
		meshData.Vertices.push_back(m1);
		meshData.Vertices.push_back(v2);

		meshData.Indices32.push_back(i * 6 + 0);
		meshData.Indices32.push_back(i * 6 + 1);
		meshData.Indices32.push_back(i * 6 + 2);
		meshData.Indices32.push_back(i * 6 + 1);

		meshData.Indices32.push_back(i * 6 + 4);
		meshData.Indices32.push_back(i * 6 + 2);
		meshData.Indices32.push_back(i * 6 + 1);
		meshData.Indices32.push_back(i * 6 + 3);

		meshData.Indices32.push_back(i * 6 + 4);
		meshData.Indices32.push_back(i * 6 + 2);
		meshData.Indices32.push_back(i * 6 + 4);
		meshData.Indices32.push_back(i * 6 + 5);
	}
}

Vertex GeometryGenerator::MidPoint(const Vertex& v0, const Vertex& v1)
{
	XMVECTOR lPosVec = DirectX::XMLoadFloat3(&v0.Position);
	XMVECTOR rPosVec = DirectX::XMLoadFloat3(&v1.Position);

	XMVECTOR midPos = (lPosVec + rPosVec) * 0.5f;

	XMFLOAT3 Pos;

	DirectX::XMStoreFloat3(&Pos, midPos);

	XMVECTOR lColVec = DirectX::XMLoadFloat4(&v0.Color);
	XMVECTOR rColVec = DirectX::XMLoadFloat4(&v1.Color);

	XMVECTOR midColor = (lColVec + rColVec) * 0.5f;

	XMFLOAT4 color;

	DirectX::XMStoreFloat4(&color, midColor);

	XMVECTOR lTexVec = XMLoadFloat2(&v0.TexC);
	XMVECTOR rTexVec = XMLoadFloat2(&v1.TexC);

	XMFLOAT2 texCoords;

	DirectX::XMStoreFloat2(&texCoords, (lTexVec + rTexVec) * 0.5f);

	return { Pos , color, texCoords };
}