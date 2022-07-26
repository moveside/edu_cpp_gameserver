#include "stdafx.h"
#include "CriticalSection.h"

using namespace std;

int a;
CriticalSection a_mutex;
int b;
CriticalSection b_mutex;

int main()
{
    // t1 �����带 �����Ѵ�.
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

    // t2 �����带 �����Ѵ�.
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

    // ��������� ���� ���������� ��ٸ���.
    // ��ǻ� ���ѷ����̱� ������ �������� ���Ѵ�.
    t1.join();
    t2.join();

    return 0;
}



