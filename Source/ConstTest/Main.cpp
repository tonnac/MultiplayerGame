#include <vector>
#include <array>

using namespace std;
class A
{
public:
	A(initializer_list<int> inInt)
		: mMember(inInt)
	{}

private:
	vector<int> mMember;
};

int main()
{
	A a = {1,2,3,4,5};

	return 0;
}