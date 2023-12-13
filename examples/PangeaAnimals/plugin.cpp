#include "Cat.h"
#include "Dog.h"

#ifdef _WIN32
#  define EXPORTIT __declspec( dllexport )
#else
#  define EXPORTIT
#endif

// extern "C" EXPORTIT std::shared_ptr<pugg::driver> create_driver(const std::string& name)
// {
//     if(name == "cat")
//         return std::make_shared<Catdriver>();

//     if(name == "dog")
//         return std::make_shared<Dogdriver>();

//     return nullptr;
// }

extern "C" EXPORTIT pugg::driver* create_driver(const std::string& name)
{
    if(name == "cat")
        return new Catdriver();

    if(name == "dog")
        return new Dogdriver();

    return nullptr;
}
