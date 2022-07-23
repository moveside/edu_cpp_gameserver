#include<iostream>
#include<thread>
#include<vector>
#include<chrono>
#include<memory>
using namespace std;

const int MaxCount = 150000;
const int ThreadCount = 4;

bool is_PrimeNumber(int number)
{
	if (number == 1) return false;
	if (number == 2 || number == 3) return true;
	for (int i = 2; i < number - 1; i++)
	{
		if ((number % i) == 0) return false;
	}
	return true;
}

void PrintNumbers(const vector<int>& primes)
{
	for (int v : primes)
	{
		cout << v << endl;
	}
}

int main()
{
	int num = 1;
	vector<int> primes;

	auto t0 = chrono::system_clock::now();
	vector<shared_ptr<thread>> threads;
	for (int i = 0; i < ThreadCount; i++)
	{
		shared_ptr<thread> thread1(new thread([&]() 
			{
				// num 와 primes 는 모든 스레드 공통으로 쓰는 변수인데 이에 대한 lock을 걸어주지 않아서 오류가 발생한다!
				while (true)
				{
					int n;
					n = num;
					num++;

					if (n >= MaxCount) 
						break;
					if (is_PrimeNumber(n))
					{
						primes.push_back(n);
					}
				}
			}));
		threads.push_back(thread1);
	}
	for (auto thread : threads)
	{
		thread->join();
	}

	auto t1 = chrono::system_clock::now();
	auto duration = chrono::duration_cast<chrono::milliseconds>(t1 - t0).count();
	cout << "Took " << duration << " milliseconds." << endl;

	return 0;
}




