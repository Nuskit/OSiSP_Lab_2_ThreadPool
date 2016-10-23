#include "stdafx.h"
#include "ThreadPool.h"
#include "Functor.h"
#include "ConsoleLogger.h"

class Test1
{
public:
	void someTest(int f)
	{
		printf("value %d\n", f);
		Sleep(30);
	}

	Test1()
	{
		++id;
	}

	void someTest1(void)
	{
		static int i = -5;
		printf("%d\n", i++);
		if (++i % 10)
		{
			throw 9;
		}
	}

	int someTesting()
	{
		static int i;
		printf("some %d\n", i++);
		return 0;
	}

	void toString(ostream& stream)
	{
		stream << "Test task " << id << ".";
	}

private:
	static atomic_int id;
};
atomic_int Test1::id = 0;


int main()
{
	ThreadPool* threadPool=new ThreadPool(*(new std::shared_ptr<ILogger>(new ConsoleLogger())), 4,7);
	Test1* s = new Test1();

	auto r = new std::shared_ptr<ThreadDelegateFunctor>(new DelegateFunctorImpl< void (Test1::*)(int)>(&Test1::someTest, s, 7));

	auto as = new std::shared_ptr<ThreadDelegateFunctor>(new DelegateFunctorImpl< void (Test1::*)(void)>(&Test1::someTest1,s));
	Sleep(100);
	for (int i = 0; i < 20; i++)
	{
		threadPool->addTask(*as);
		threadPool->addTask(*r);
	}
	printf("Complete\n");
	Sleep(2000);

	printf("Complete 23\n");
	delete threadPool;
	getchar();
	return 0;
}