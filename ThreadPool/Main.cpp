#include "stdafx.h"
#include "ThreadPool.h"
#include "Functor.h"
#include "FileLogger.h"

class TestClass
{
public:
	void methodWithArgument(int value)
	{
		printf("Method value %d\n", value);
		Sleep(50);
	}

	TestClass()
	{
		++id;
	}

	void MethodWithoutArgument(void)
	{
		static int i = -5;
		printf("Method static value =%d\n", i++);
		if (++i % 10)
		{
			throw 9;
		}
	}

	void toString(ostream& stream)
	{
		stream << "Test object " << id << ".";
	}

private:
	static atomic_int id;
};
atomic_int TestClass::id = 0;


int main()
{
	ThreadPool* threadPool=new ThreadPool(*(new std::shared_ptr<ILogger>(new FileLogger())), 3,5);
	TestClass* testObject = new TestClass();
	int&& argument = 7;

	auto testFunctorWithArgument = new std::shared_ptr<ThreadDelegateFunctor>(new DelegateFunctorImpl< void (TestClass::*)(int)>(&TestClass::methodWithArgument, testObject, std::move(argument)));

	auto testFunctorWithoutArgument = new std::shared_ptr<ThreadDelegateFunctor>(new DelegateFunctorImpl< void (TestClass::*)(void)>(&TestClass::MethodWithoutArgument,testObject));
	Sleep(100);
	for (int i = 0; i < 200; i++)
	{
		threadPool->addTask(*testFunctorWithoutArgument);
		threadPool->addTask(*testFunctorWithArgument);
		++argument;
		dynamic_cast<DelegateFunctorImpl< void (TestClass::*)(int)>*>(testFunctorWithArgument->get())->setArgs(std::move(argument));
		Sleep(10);
	}
	printf("Complete\n");
	Sleep(500);
	delete threadPool;
	getchar();
	return 0;
}