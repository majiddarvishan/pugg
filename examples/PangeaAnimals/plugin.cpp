#include <pugg/kernel.hpp>

#include "Cat.h"
#include "Dog.h"

#ifdef _WIN32
#  define EXPORTIT __declspec( dllexport )
#else
#  define EXPORTIT
#endif

extern "C" EXPORTIT void register_pugg_plugin(pugg::Kernel* kernel)
{
	kernel->add_driver(new CatDriver());
	kernel->add_driver(new DogDriver());
}
