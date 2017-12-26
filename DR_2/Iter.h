#pragma once
#pragma once


class Iterate
{
	int min, max, pos;
	Iterate* iter = nullptr;
	Iterate(Iterate* iterator, int pos)
	{
		this->pos = pos;
		this->iter = iterator;
	}
public:
	Iterate(int min, int max)
		:min(min), max(max), pos(min)
	{}
	Iterate begin() { return Iterate(this, min); }
	Iterate end() { return Iterate(this, max); }
	// increment position
	Iterate& operator++()
	{
		pos++;
		return *this;
	}
	// use while iter != end()
	bool operator != (Iterate& rhs)
	{
		return pos != rhs.pos;
	}
	// pointer overload
	int operator* ()
	{
		return pos;
	}
};