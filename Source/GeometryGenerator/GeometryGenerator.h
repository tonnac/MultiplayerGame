#pragma once

#include <cstdint>
#include <DirectXMath.h>
#include <vector>

using std::vector;

using DirectX::XMFLOAT2;
using DirectX::XMFLOAT3;
using DirectX::XMFLOAT4;

using DirectX::XMVECTOR;

class GeometryGenerator
{
public:
	using uint16 = uint16_t;
	using uint32 = uint16_t;

	struct Vertex
	{
		Vertex() {}
		Vertex(
			const XMFLOAT3& p,
			const XMFLOAT4& c,
			const XMFLOAT2& uv) :
			Position(p),
			Color(c),
			TexC(uv) {}
		Vertex(
			float px, float py, float pz,
			float cx, float cy, float cz, float cw,
			float u, float v) :
			Position(px, py, pz),
			Color(cx, cy, cz, cw),
			TexC(u, v) {}

		XMFLOAT3 Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
		XMFLOAT4 Color = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
		XMFLOAT2 TexC = XMFLOAT2(0.0f, 0.0f);
	};

	struct MeshData
	{
		vector<Vertex> Vertices;
		vector<uint32> Indices32;

		vector<uint16>& GetIndices16()
		{
			if (mIndices16.empty())
			{
				mIndices16.resize(Indices32.size());
				{
					for (size_t i = 0; i < Indices32.size(); ++i)
					{
						mIndices16[i] = static_cast<uint16>(Indices32[i]);
					}
				}
			}
			return mIndices16;
		}

	private:
		vector<uint16> mIndices16;
	};

	MeshData CreateBox(float width, float height, float depth, uint32 numSubdivisions);
	MeshData CreateSphere(float radius, uint32 sliceCount, uint32 stackCount);
	MeshData CreateGeosphere(float radius, uint32 numSubdivisions);
	MeshData CreateCylinder(float bottomRadius, float topRadius, float height, uint32 sliceCount, uint32 stackCount);
	MeshData CreateGrid(float width, float depth, uint32 m, uint32 n);
	MeshData CreateQuad(float x, float y, float w, float h, float depth);
	MeshData CreateCapsule(float radius, float halfHeight, uint32 sliceCount = 8);

private:
	void Subdivide(MeshData& meshData);
	Vertex MidPoint(const Vertex& v0, const Vertex& v1);
	void BuildCylinderTopCap(float bottomRadius, float topRadius, float height, uint32 sliceCount, uint32 stackCount, MeshData& meshData);
	void BuildCylinderBottomCap(float bottomRadius, float topRadius, float height, uint32 sliceCount, uint32 stackCount, MeshData& meshData);

	static constexpr unsigned int MaxSubDivisions = 6u;
};