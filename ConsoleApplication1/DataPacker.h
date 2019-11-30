#pragma once
#include "AllHeader.h"


class UIntPacker
{
private:
	unsigned char m_data = 0;
	unsigned int m_bitCount = 0;

public:

	static UIntPacker& getInstance()
	{
		static UIntPacker instance;
		return instance;
	}

	UIntPacker()
	{
		reset();
	}

	void reset()
	{
		m_data = 0;
		m_bitCount = 0;
	}

	void SetData(unsigned char data)
	{
		m_data = data;
	}

	unsigned char GetData()
	{
		unsigned char temp = m_data;
		reset();
		return temp;
	}

	bool pack(unsigned int value, unsigned int bitCount)
	{
		if (bitCount + m_bitCount <= 8)
		{
			value <<= m_bitCount;
			m_data |= value;
			m_bitCount += bitCount;
			return true;

		}
		else if (bitCount + m_bitCount > 8)
		{
			std::cout << "Not enough bits to pack this integer!!" << std::endl;
			return false;
		}
	}

	unsigned int extract(unsigned int bitCount)
	{
		unsigned int value;
		value = m_data & ((1 << bitCount) - 1);
		m_data = m_data >> bitCount;
		return value;
	}
};


class UIntSplitter
{
public:
	unsigned char m_bytes[4];

	void storeUInt(unsigned int value) //store "1 unsigned int" value into "4 unsigned char"
	{
		unsigned int byteDivide = 32 / 4;

		for (int i = 0; i < 4; i++)
		{
			unsigned int temp = value;
			unsigned int rightShift = byteDivide * i;
			temp >>= rightShift;
			m_bytes[i] = (char)temp;
		}
	}

	unsigned int getInt(void) //merge the bytes(unsigned char) and return the unsigned int value
	{
		unsigned int byteDivide = 32 / 4;
		unsigned int value = 0;

		for (int i = 0; i < 4; i++)
		{
			unsigned int temp = m_bytes[i];
			unsigned int leftShift = byteDivide * i;
			temp <<= leftShift;
			value |= temp;
		}
		return value;
	}

};

