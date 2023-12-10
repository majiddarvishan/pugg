#include <pugg/kernel.hpp>

#include "Fish.h"
#include "Whale.h"

#ifdef _WIN32
#  define EXPORTIT __declspec( dllexport )
#else
#  define EXPORTIT
#endif

extern "C" EXPORTIT void register_pugg_plugin(pugg::kernel* kernel)
{
	kernel->add_driver(new Fishdriver());
	kernel->add_driver(new Whaledriver());
}
