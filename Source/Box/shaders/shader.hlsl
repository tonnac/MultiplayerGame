cbuffer VS_CB : register(b1)
{
	float4x4 gWorldViewProj;
};


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
	vOut.p = mul(float4(vIn.p, 1.f), gWorldViewProj);
	vOut.c = vIn.c;
	return vOut;
}

float4 PS(VertexOut vOut) : SV_Target
{
	return vOut.c;
}