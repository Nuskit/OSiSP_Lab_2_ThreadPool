#include "stdafx.h"
#include "ThreadPool.h"

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
	//ThreadPool threadPool(10,5);
	Test1* s = new Test1();
	DelegateFunctor<void>* r = new DelegateFunctorImpl< void (Test1::*)(int)>(&Test1::someTest, s,7);
	r->complete();

	auto* as = new DelegateFunctorImpl< void (Test1::*)(int)>(&Test1::someTest,s,5);
	as->complete();
	//threadPool.addTask(s->someTesting);
	
	return 0;
}