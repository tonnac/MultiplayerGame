cbuffer VS_CB : register(b0)
{
	float4x4 gViewProj;
};

cbuffer VS_PER : register(b1)
{
	float4x4 gWorld;
}


struct VertexIn
{
	float3 p : POSITION;
	float4 c : COLOR;
};

struct VertexOut
{
	float4 p : SV_POSITION;
	float4 c : COLOR;
};

VertexOut VS(VertexIn vIn)
{
	VertexOut vOut;
	float4x4 WorldViewProj = mul(gWorld, gViewProj);
	vOut.p = mul(float4(vIn.p, 1.f), WorldViewProj);
	vOut.c = vIn.c;
	return vOut;
}

float4 PS(VertexOut vOut) : SV_Target
{
	return vOut.c;
}