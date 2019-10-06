#pragma once

struct VertexC
{
	XMFLOAT3 p;
	XMFLOAT4 c;
	VertexC(float px, float py, float pz, float cx, float cy, float cz, float cw)
	{
		p = XMFLOAT3(px, py, pz);
		c = XMFLOAT4(cx, cy, cz, cw);
	}
	VertexC(const XMFLOAT3& position, const XMFLOAT4& color)
		: p(position), c(color)
	{}
};

struct VertexP
{
	XMFLOAT3 p;
	VertexP(float px, float py, float pz)
	{
		p = XMFLOAT3(px, py, pz);
	}
	VertexP(const XMFLOAT3& position)
		: p(position)
	{}
};

struct Vertex
{
	XMFLOAT3 p;
	XMFLOAT4 c;
	XMFLOAT2 t;

	Vertex(float px, float py, float pz, float cx, float cy, float cz, float cw, float tx, float ty)
	{
		p = XMFLOAT3(px, py, pz);
		c = XMFLOAT4(cx, cy, cz, cw);
		t = XMFLOAT2(tx, ty);
	}
	Vertex(const XMFLOAT3& position, const XMFLOAT4& color, const XMFLOAT2& texcoords)
		: p(position), c(color), t(texcoords)
	{}
};