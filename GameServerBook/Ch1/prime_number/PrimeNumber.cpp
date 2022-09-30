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
	recursive_mutex num_mutex; // �����忡�� num ������ �����ϱ� ���� mutex
	auto t0 = chrono::system_clock::now();
	vector<shared_ptr<thread>> threads;
	recursive_mutex primes_mutex; // �����忡�� prime �迭�� �����ϱ� ���� mutex
	for (int i = 0; i < ThreadCount; i++)
	{
		shared_ptr<thread> thread1(new thread([&]() 
			{
				// num �� primes �� ��� ������ �������� ���� �����ε� �̿� ���� lock�� �ɾ����� �ʾƼ� ������ �߻��Ѵ�!
				while (true)
				{
					int n;
					{// num�� ���� ���ϱ� ������ lock�� �ؾ� �Ѵ�
						lock_guard<recursive_mutex> num_lock(num_mutex);
						n = num;
						num++;
					}
					if (n >= MaxCount) break;
					if (is_PrimeNumber(n))
					{
						{// vector�� ���� push�ϱ� ���� lock�Ѵ�.
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



