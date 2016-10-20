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

	ThreadDelegateFunctor* r = new DelegateFunctorImpl< void (Test1::*)(int)>(&Test1::someTest, s, 7);

	auto as = new std::shared_ptr<ThreadDelegateFunctor>(new DelegateFunctorImpl< void (Test1::*)(int)>(&Test1::someTest,s,5));
	//auto st = new std::shared_ptr<ThreadDelegateFunctor>(new DelegateFunctorImpl< void (Test1::*)(void)>(&Test1::someTesting, s,nullptr));
	for (int i=0;i<20;i++)
		threadPool->addTask(*as);
	//delete as;
	Sleep(500);
	printf("Complete\n");
	Sleep(2000);

	printf("Complete 23\n");
	delete threadPool;
	getchar();
	return 0;
}