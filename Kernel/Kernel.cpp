#include <Kernel/Drivers/VGA.h>
#include <Kernel/KPrintf.h>
#include <Kernel/KPanic.h>

namespace Kernel
{
	void InitKernel()
	{
		if (Drivers::VGA::Init() < 0)
			KPanic("Failed To Initialize VGA Driver");
	}
}

extern "C"
{
	void KMain()
	{
		Kernel::InitKernel();
	}
}
