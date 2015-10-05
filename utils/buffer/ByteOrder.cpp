#include "stdafx.h"
#include "ByteOrder.h"

ByteOrder* ByteOrder::BIG_ENDIAN = new ByteOrder("BIG_ENDIAN", true);
ByteOrder* ByteOrder::LITTLE_ENDIAN = new ByteOrder("LITTLE_ENDIAN", false);
ByteOrder* ByteOrder::NATIVE_ORDER = ByteOrder::LITTLE_ENDIAN;

ByteOrder* ByteOrder::nativeOrder()
{
	return NATIVE_ORDER;
}