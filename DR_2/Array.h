#pragma once
#include "stdafx.h"

template<typename T, unsigned int size>
class Array
{

public:
	
	Array() = default;
	
	std::size_t Count()const
	{
		return ElementCount<T>(elements);
	}
	
	T& operator[](const int& index)
	{
		
		return elements[index];
	};
	T elements[size];
};