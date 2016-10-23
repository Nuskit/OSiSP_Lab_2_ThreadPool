#include "stdafx.h"
#include "ThreadPool.h"
#include "Functor.h"
#include "FileLogger.h"
#include <iostream>
#include "Mutex.h"
Mutex mutex;

void testMethod()
{
	static int i = 0;
	mutex.enter();
	cout << "Static method ~ " << i++ << endl;
	mutex.exit();
}

class TestClass
{
public:
	void methodWithArgument(int value)
	{
		mutex.enter();
		cout << "Method value " << value << endl;
		mutex.exit();
		Sleep(50);
	}

	TestClass()
	{
		++id;
	}

	void MethodWithoutArgument(void)
	{
		static int i = -5;
		mutex.enter();
		cout << "Method static value = " << i << endl;
		mutex.exit();
		if (++i % 10)
		{
			throw 9;
		}
		Sleep(5);
	}

	void toString(ostream& stream)
	{
		mutex.enter();
		stream << "Test object " << id << ".";
		mutex.exit();
	}

private:
	static atomic_int id;
};
atomic_int TestClass::id = 0;

typedef std::shared_ptr<ThreadDelegateFunctor> SharedPtrThreadDelegate;
int main()
{
	ThreadPool* threadPool=new ThreadPool(*(new std::shared_ptr<ILogger>(new FileLogger())), 3,5);
	TestClass* testObject = new TestClass();
	int&& argument = 7;

	auto testFunctorWithArgument = new SharedPtrThreadDelegate(new DelegateFunctorImpl< void (TestClass::*)(int)>(&TestClass::methodWithArgument, testObject, std::move(argument)));
	auto testFunctorStaticMethod = new SharedPtrThreadDelegate(new DelegateFunctorImpl<void (void)>(&testMethod));
	auto testFunctorWithoutArgument = new SharedPtrThreadDelegate(new DelegateFunctorImpl< void (TestClass::*)(void)>(&TestClass::MethodWithoutArgument,testObject));
	Sleep(100);
	for (int i = 0; i < 200; i++)
	{
		threadPool->addTask(*testFunctorWithoutArgument);
		threadPool->addTask(*testFunctorWithArgument);
		threadPool->addTask(*testFunctorStaticMethod);
		++argument;
		dynamic_cast<DelegateFunctorImpl< void (TestClass::*)(int)>*>(testFunctorWithArgument->get())->setArgs(std::move(argument));
		Sleep(10);
	}
	cout << "Add all task, wait" << endl;
	printf("Complete\n");
	Sleep(500);
	cout << "End wait" << endl;
	delete threadPool;
	getchar();
	return 0;
}