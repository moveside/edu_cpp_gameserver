#include "stdafx.h"
#include "CriticalSection.h"

using namespace std;

int a;
CriticalSection a_mutex;
int b;
CriticalSection b_mutex;

int main()
{
    // t1 스레드를 시작한다.
    thread t1([]()
        {
            while (1)
            {
                CriticalSectionLock lock(a_mutex);
                a++;
                CriticalSectionLock lock2(b_mutex);
                b++;
                cout << "t1 done.\n";
            }
        });

    // t2 스레드를 시작한다.
    thread t2([]()
        {
            while (1)
            {
                CriticalSectionLock lock(b_mutex);
                b++;
                CriticalSectionLock lock2(a_mutex);
                a++;
                cout << "t2 done.\n";
            }
        });

    // 스레드들의 일이 끝날때까지 기다린다.
    // 사실상 무한루프이기 떄문에 끝나지는 못한다.
    t1.join();
    t2.join();

    return 0;
}



