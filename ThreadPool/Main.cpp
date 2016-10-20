#include "stdafx.h"
#include "ThreadPool.h"
#include "Functor.h"
#include "ConsoleLogger.h"

class Test1
{
public:
	void someTest(int f)
	{
		printf("%d", f);
	}
	int someTesting()
	{
		return 0;
	}
};


int main()
{
	ThreadPool* threadPool=new ThreadPool(*(new std::shared_ptr<ILogger>(new ConsoleLogger())), 10,5);
	Test1* s = new Test1();

	ThreadDelegateFunctor* r = new DelegateFunctorImpl< void (Test1::*)(int)>(&Test1::someTest, s, 7);

	auto as = new std::shared_ptr<ThreadDelegateFunctor>(new DelegateFunctorImpl< void (Test1::*)(int)>(&Test1::someTest,s,5));
	threadPool->addTask(*as);
	Sleep(500);
	delete threadPool;
	
	getchar();
	return 0;
}


