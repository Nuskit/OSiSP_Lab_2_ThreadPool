#include "stdafx.h"
#include "ThreadPool.h"
#include "Functor.h"
#include "FileLogger.h"
#include <iostream>
#include "Mutex.h"
#ifndef WINDOWS_SYSTEM
  #include "unistd.h"
#endif
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
#ifdef WINDOWS_SYSTEM
    Sleep(50);
#endif
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
#ifdef WINDOWS_SYSTEM
    Sleep(5);
#endif
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
atomic_int TestClass::id(0);

typedef std::shared_ptr<ThreadDelegateFunctor> SharedPtrThreadDelegate;
int main()
{
  ThreadPool* threadPool=new ThreadPool(*(new std::shared_ptr<ILogger>(new FileLogger())), 3,5);
  TestClass* testObject = new TestClass();
#ifdef WINDOWS_SYSTEM
  int&& argument = 7;
  auto testFunctorWithArgument = new SharedPtrThreadDelegate(new DelegateFunctorImpl< void (TestClass::*)(int)>(&TestClass::methodWithArgument, testObject, std::move(argument)));
#endif
  auto testFunctorStaticMethod = new SharedPtrThreadDelegate(new DelegateFunctorImpl<void (void)>(&testMethod));
  auto testFunctorWithoutArgument = new SharedPtrThreadDelegate(new DelegateFunctorImpl< void (TestClass::*)(void)>(&TestClass::MethodWithoutArgument,testObject));
#ifdef WINDOWS_SYSTEM
  Sleep(100);
#endif
  for (int i = 0; i < 200; i++)
  {
    threadPool->addTask(*testFunctorWithoutArgument);
    threadPool->addTask(*testFunctorStaticMethod);
#ifdef WINDOWS_SYSTEM
    threadPool->addTask(*testFunctorWithArgument);
    ++argument;
    dynamic_cast<DelegateFunctorImpl< void (TestClass::*)(int)>*>(testFunctorWithArgument->get())->setArgs(std::move(argument));
    Sleep(10);
#endif
  }
  cout << "Add all task, wait" << endl;
  printf("Complete\n");
#ifdef WINDOWS_SYSTEM
  Sleep(500);
#else
  sleep(1);
#endif
  cout << "End wait" << endl;
  delete threadPool;
  getchar();
  return 0;
}
