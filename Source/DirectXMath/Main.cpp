#include <DirectXMath.h>

using namespace DirectX;

int main()
{
	XMFLOAT3 e(1.f, 1.f, 1.f);
	XMFLOAT3 f(1.f, 1.f, 1.f);

	XMVECTOR a = XMLoadFloat3(&e);
	XMVECTOR b = XMLoadFloat3(&f);

	XMVECTOR c = a + b;

	return 0;
}