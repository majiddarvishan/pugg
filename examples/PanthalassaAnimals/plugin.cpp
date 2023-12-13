#include "Fish.h"
#include "Whale.h"

#ifdef _WIN32
#  define EXPORTIT __declspec( dllexport )
#else
#  define EXPORTIT
#endif

// extern "C" EXPORTIT std::shared_ptr<pugg::driver> create_driver(const std::string& name)
// {
//     if(name == "fish")
//         return std::make_shared<Fishdriver>();

//     if(name == "whale")
//         return std::make_shared<Whaledriver>();

//     return nullptr;
// }

extern "C" EXPORTIT pugg::driver* create_driver(const std::string& name)
{
    if(name == "fish")
        return new Fishdriver();

    if(name == "whale")
        return new Whaledriver();

    return nullptr;
}
