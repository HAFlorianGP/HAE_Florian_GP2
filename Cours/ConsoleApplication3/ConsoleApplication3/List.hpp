#pragma once

template<typename Type>
class List
{
public:

	Type        elem;
	List<Type>* next = nullptr;

	List(const Type & elem)
	{
		this->elem = elem;
	}

	~List()
	{
		
	}

	int Length()
	{
		if (next == nullptr)
			return 1;
		else
			return 1+next->Length()+1;
	}

	void AddLast(const Type & elem)
	{
		if (next == nullptr)
		{
			next = new List(elem);
		}
		else
		{
			next->AddLast(elem);
		}
	}

	List<Type> * AddFirst(const Type & elem)
	{
		auto Toto = new List(elem);
		Toto->next = this;
		return Toto;
	}

	List<Type>* Remove(const Type & RemoveMe)
	{
		if (this->elem == RemoveMe)
		{
			auto SuiteDeListe = next;
			this->next = nullptr;
			if (SuiteDeListe != 0)
				return SuiteDeListe->Remove(RemoveMe);
			else
				return nullptr;
			
		}
		else
			{
				if (next)
					next = next->Remove(RemoveMe);
			}
	}

	bool Contains(const Type & elemAtrouver)
	{
		if (next == nullptr)
		{
			return false;
		}
		if (elem == elemAtrouver)
			return true;
		else
			next->Contains(elemAtrouver);
	}
};

typedef List<int> IntList;