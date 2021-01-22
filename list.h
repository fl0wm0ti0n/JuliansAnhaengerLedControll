#pragma once
#ifndef _LIST_h
#define _LIST_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

template<typename T>
class List {

public:
	T Last;
	//void append(T item) {
	//    if (length < 16) data[length++] = item;
	//}

	//void remove(T index) {
	//    if (index >= length) return;
	//    memmove(&data[index], &data[index + 1], length - index - 1);
	//    length--;
	//}

	void push_back(T& value)
	{
		Last = value;
		_length++;
		T* tmp_array = nullptr;
		tmp_array = new T[_length];

		for (unsigned int i = 0; i < _length; i++)
		{
			tmp_array[i] = data[i];
		}

		tmp_array[_length] = value;
		delete[] data;
		data = tmp_array;
	}

	void pop_back()
	{
		_length--;
		T* tmp_array = nullptr;
		tmp_array = new T[_length];

		for (unsigned int i = 0; i < _length - 1; i++)
		{
			tmp_array[i] = data[i];
		}

		delete[] data;
		data = tmp_array;
	}

	//T find(T& value)
	//{
	//	for (unsigned int i = 0; i < _length - 1; i++)
	//	{
	//		if (_data[i] = value)
	//		{
	//			
	//		}

	//	}

	//	return;
	//}
	T* data = nullptr;
	
private:
	byte _length;

};

#endif