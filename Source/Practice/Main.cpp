#include <iostream>
#include <vector>
#include <string>
#include <cmath>

using namespace std;

void PrintArr(const vector<int>& arr)
{
	cout << "[ ";
	for (const int& element : arr)
	{
		cout << element << " ";
	}
	cout << "]\n";
}

string MakeString(int num)
{
	char buf[6];

	memset(buf, ' ', std::size(buf));
	buf[5] = 0;

	// 10000 01000 00100 00010 00001  (16 8 4 2 1)
	for (int i = 0; i < 5; ++i)
	{
		if (num & (int)pow(2, i))
		{
			buf[i] = '#';
		}
	}

	return string(buf);
}

int main()
{
	int i;
	
	cin >> i;

	vector<int> arr1(i);
	vector<int> arr2(i);
	vector<int> arr3(i);

	for (int k = 0; k < i; ++k)
	{
		arr1[k] = rand() % 60;
		arr2[k] = rand() % 60;
		arr3[k] = (arr1[k] & 31) | (arr2[k] & 31);
	}

	PrintArr(arr1);
	PrintArr(arr2);

	cout << "[";

	for (int k = 0; k < i; ++k)
	{
		cout << MakeString(arr3[k]);
		cout << ", ";
	}

	cout << "]\n";


 	return 0;
}