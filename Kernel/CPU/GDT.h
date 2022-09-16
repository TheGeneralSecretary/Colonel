#pragma once

#include <stdint.h>

#define GDT_TOTAL_SEGMENTS 6

namespace Kernel
{
	struct GDTDesc
	{
		uint16_t LimitLow;
		uint16_t BaseLow;
		uint8_t BaseMid;
		uint8_t Access;
		uint8_t Granularity;
		uint8_t BaseHigh;
	} __attribute__((packed));

	struct GDTRDesc
	{
		uint16_t Limit;
		uint32_t Base;
	} __attribute__((packed));

	class GDT
	{
	public:
		static int32_t Init();
		static void SetGate(uint32_t seg, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran);

	private:
		static GDTRDesc s_GDTR;
		static GDTDesc s_GDT[GDT_TOTAL_SEGMENTS];
	};
}