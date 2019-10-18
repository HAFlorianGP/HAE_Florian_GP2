#pragma once


template<typename Type>
class List
{
public:

	Type		 elem;
	List<Type>()* next = nullptr;

	List<const Type & elem>()
	{
		this->elem = elem;
	}

	void Append(const Type & elem)
	{

	}

	void Concat(const Type & elem)
	{

	}

	void Remove(const Type & elem)
	{

	}
};

typedef List<int> IntList;