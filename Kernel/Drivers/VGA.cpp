#include <Kernel/Drivers/VGA.h>
#include <Kernel/KString.h>
#include <Kernel/CPU/Port.h>
#include <Kernel/KPrintf.h>

#define VGA_ENCODE(ch, color) ((color << 8) | ch)
#define VGA_GET_OFFSET(col, row) ((row * VGA_WIDTH + col) * 2)

namespace Kernel
{
	namespace Drivers
	{
		uint16_t* VGA::m_VideoAddress = nullptr;
		uint16_t VGA::m_Col = 0;
		uint16_t VGA::m_Row = 0;
		VGAColor VGA::m_Color = VGAColor::White;

		int32_t VGA::Init()
		{
			m_VideoAddress = (uint16_t*)0xB8000;
			m_Col = 0;
			m_Row = 0;
			m_Color = VGAColor::White;
			Clear();
			KPrintf("Initializing VGA Driver...\n");
			return 0;
		}

		void VGA::Clear()
		{
			for (uint16_t y = 0; y < VGA_HEIGHT; y++)
				for (uint16_t x = 0; x < VGA_WIDTH; x++)
					PutCharLocation(x, y, ' ', m_Color);
			SetCursor(0);
		}

		void VGA::PutChar(uint8_t ch)
		{
			PutCharColor(ch, m_Color);
		}

		void VGA::PutCharColor(uint8_t ch, VGAColor color)
		{
			if (m_Row >= VGA_HEIGHT)
				ScrollDown();

			if (ch == '\n')
			{
				m_Col = 0;
				m_Row += 1;
				return;
			}

			PutCharLocation(m_Col, m_Row, ch, color);
			m_Col += 1;

			if (m_Col >= VGA_WIDTH)
			{
				m_Col = 0;
				m_Row += 1;
			}

			SetCursor(VGA_GET_OFFSET(m_Col, m_Row));
		}

		void VGA::PutStr(const char* str)
		{
			for (uint32_t i = 0; i < strlen(str); i++)
				PutChar(str[i]);
		}

		VGAColor VGA::GetColor()
		{
			return m_Color;
		}

		void VGA::SetColor(VGAColor color)
		{
			m_Color = color;
		}

		void VGA::EnableCursor(uint8_t start, uint8_t end)
		{
			Port::outb(VGA_CTRL_PORT, 0x0A);
			Port::outb(VGA_DATA_PORT, (Port::insb(VGA_DATA_PORT) & 0xC0) | start);
			Port::outb(VGA_CTRL_PORT, 0x0B);
			Port::outb(VGA_DATA_PORT, (Port::insb(VGA_DATA_PORT) & 0xE0) | end);
		}

		void VGA::DisableCursor()
		{
			Port::outb(VGA_CTRL_PORT, 0x0A);
			Port::outb(VGA_DATA_PORT, 0x20);
		}

		void VGA::SetCursor(uint16_t offset)
		{
			offset /= 2;
			Port::outb(VGA_CTRL_PORT, 0x0F);
			Port::outb(VGA_DATA_PORT, (offset & 0xFF));
			Port::outb(VGA_CTRL_PORT, 0x0E);
			Port::outb(VGA_DATA_PORT, ((offset >> 8) & 0xFF));
		}

		uint16_t VGA::GetCursor()
		{
			uint16_t offset = 0;
			Port::outb(VGA_CTRL_PORT, 0x0F);
			offset = Port::insb(VGA_DATA_PORT);
			Port::outb(VGA_CTRL_PORT, 0x0E);
			offset |= (Port::insb(VGA_DATA_PORT) << 8);
			return offset;
		}

		void VGA::PutCharLocation(uint16_t col, uint16_t row, uint8_t ch, VGAColor color)
		{
			m_VideoAddress[(row * VGA_WIDTH) + col] = VGA_ENCODE(ch, (uint8_t)color);
		}

		void VGA::ScrollDown()
		{
			uint8_t* start = (uint8_t*)m_VideoAddress + VGA_WIDTH * 2;
			uint32_t size = m_Row * VGA_WIDTH * 2;
			memcpy(m_VideoAddress, start, size);
			start = (uint8_t*)m_VideoAddress + size;
			memsetw(start, VGA_ENCODE(' ', (uint8_t)VGAColor::Black), size);
			m_Row--;
		}
	}
}
