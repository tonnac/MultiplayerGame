#include <chrono>
#include <iterator>

using namespace std::chrono;

typedef duration<float, milliseconds> milli;

int main()
{
	time_point<high_resolution_clock> p = high_resolution_clock::now();
	duration<float> rere;

	int qwe[500];

	constexpr auto por = std::size(qwe);

	for (int i = 0; i < 50000; ++i)
	{
		for (int k = 0; k < 5000; ++k)
		{
		}
	}

	time_point<high_resolution_clock> e = high_resolution_clock::now();

	milliseconds pqw = duration_cast<milliseconds>(e - p);

	duration<float> qwesd = e - p;

	duration<double> re = e - p;

	return 0;
}