#ifndef FUNCTOR_H_
#define FUNCTOR_H_
#include "stdafx.h"
#include <tuple>

class ThreadDelegateFunctor
{
public:
	virtual void complete() = 0;
  virtual void toString(std::ostream& stream) = 0;
	virtual ~ThreadDelegateFunctor() {};
};

template<typename R>
class DelegateFunctor:public ThreadDelegateFunctor
{
public:
	R complete() = 0;
};

template<typename ... Args> class DelegateFunctorImpl;

template<typename R, typename ... Args>
class DelegateFunctorImpl<R(Args ...)> :public DelegateFunctor<R>
{
public:
	typedef R(*FT)(Args ...);

	DelegateFunctorImpl(FT fn,Args&&... args) : m_fn(fn),m_args(std::forward<Args>(args)...) {  }

	R complete()
	{
		return m_fn(std::get<Args>(m_args)...);
	}

	void setArgs(Args&&... args)
	{
		m_args = std::make_tuple(args...);
	}

	void toString(std::ostream& stream)
	{
		stream << "WithOut object";
	}
private:
	FT m_fn;
	std::tuple<Args...> m_args;
};

template<typename T, typename R, typename ... Args>
class DelegateFunctorImpl<R(T::*)(Args ...)>:public DelegateFunctor<R>
{
public:
	typedef R(T::*FT)(Args ...);
	typedef T HostType;

	DelegateFunctorImpl(FT fn, T* obj, Args&&... args) : m_fn(fn), m_obj(obj), m_args(std::forward<Args>(args)...) {  }

	R complete()
  {
    return (m_obj->*m_fn)(std::get<Args>(m_args)...);
	}

	void setArgs(Args&&... args)
	{
		m_args = std::make_tuple(args...);
	}

	void toString(std::ostream& stream)
	{
		m_obj->toString(stream);
	}
private:
	FT m_fn;
	T* m_obj;
	std::tuple<Args...> m_args;
};
#endif
