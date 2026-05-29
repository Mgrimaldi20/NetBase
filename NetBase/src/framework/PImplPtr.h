#ifndef __NETBASE_FRAMEWORK_PIMPLPTR_H__
#define __NETBASE_FRAMEWORK_PIMPLPTR_H__

template<typename T>
class PImplPtr
{
public:
	PImplPtr();
	PImplPtr(T val);

	~PImplPtr();

	T *operator->();

private:
	T *ptr;
};

template<typename T>
inline PImplPtr<T>::PImplPtr()
	: ptr(new T)
{
}

template<typename T>
inline PImplPtr<T>::PImplPtr(T val)
	: ptr(new T(val))
{
}

template<typename T>
inline PImplPtr<T>::~PImplPtr()
{
	if (ptr)
		delete ptr;
}

template<typename T>
inline T *PImplPtr<T>::operator->()
{
	return ptr;
}

#endif
