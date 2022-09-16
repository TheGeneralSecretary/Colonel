#include <Kernel/Drivers/VGA.h>
#include <Kernel/KPrintf.h>
#include <Kernel/KPanic.h>
#include <Kernel/CPU/GDT.h>

namespace Kernel
{
	void InitKernel()
	{
		if (Drivers::VGA::Init() < 0)
			KPanic("Failed To Initialize VGA Driver");

		if (GDT::Init() < 0)
			KPanic("Failed To Initialize GDT");
	}
}

extern "C" void KMain()
{
	Kernel::InitKernel();
}
