#include "stdafx.h"
#include "ThreadPool.h"
#include "Functor.h"
#include "ConsoleLogger.h"

class Test1
{
public:
	void someTest(int f)
	{
		printf("%d\n", f);
	}

	void someTest1(void)
	{
		printf("%d\n", 7);
	}
	int someTesting()
	{
		static int i;
		printf("some %d\n", i++);
		return 0;
	}
};


int main()
{
	ThreadPool* threadPool=new ThreadPool(*(new std::shared_ptr<ILogger>(new ConsoleLogger())), 7,2);
	Test1* s = new Test1();

	auto r = new DelegateFunctorImpl< void (Test1::*)(int)>(&Test1::someTest, s, 7);

	auto as = new std::shared_ptr<ThreadDelegateFunctor>(new DelegateFunctorImpl< void (Test1::*)(void)>(&Test1::someTest1,s));
	for (int i=0;i<50;i++)
		threadPool->addTask(*as);
	//delete as;
	Sleep(500);
	printf("Complete\n");
	Sleep(2000);

	printf("Complete 23\n");
	getchar();
	delete threadPool;
	getchar();
	return 0;
}