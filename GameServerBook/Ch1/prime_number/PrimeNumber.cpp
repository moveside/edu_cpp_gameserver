#include<iostream>
#include<thread>
#include<vector>
#include<chrono>
#include<memory>
#include<mutex>

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
	recursive_mutex num_mutex; // 스레드에서 num 변수에 접근하기 위한 mutex
	auto t0 = chrono::system_clock::now();
	vector<shared_ptr<thread>> threads;
	recursive_mutex primes_mutex; // 스레드에서 prime 배열에 접근하기 위한 mutex
	for (int i = 0; i < ThreadCount; i++)
	{
		shared_ptr<thread> thread1(new thread([&]() 
			{
				// num 와 primes 는 모든 스레드 공통으로 쓰는 변수인데 이에 대한 lock을 걸어주지 않아서 오류가 발생한다!
				while (true)
				{
					int n;
					{// num의 값이 변하기 때문에 lock을 해야 한다
						lock_guard<recursive_mutex> num_lock(num_mutex);
						n = num;
						num++;
					}
					if (n >= MaxCount) break;
					if (is_PrimeNumber(n))
					{
						{// vector에 값을 push하기 위해 lock한다.
							lock_guard<recursive_mutex> prime_lokc(primes_mutex);
							primes.push_back(n);
						}
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




