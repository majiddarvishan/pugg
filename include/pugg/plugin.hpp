//          Copyright Tunc Bahcecioglu 2009 - 2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <string>

#ifdef WIN32
    #ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
    #endif
    #ifndef NOMINMAX
    #define NOMINMAX
    #endif
    #include <windows.h>
#else
    #include <dlfcn.h>
#endif

namespace pugg {

class kernel;

namespace detail {

typedef void fnRegisterplugin(pugg::kernel*);

class dll_loader
{
public:
    ~dll_loader()
    {
        this->free();
    }

    bool load(std::string filename)
    {
#ifdef WIN32
		_handle = LoadLibraryA(filename.c_str());
#else
		_handle = dlopen(filename.c_str(), RTLD_NOW);
#endif
        return (_handle != NULL);
    }
    fnRegisterplugin* register_function()
    {
#ifdef WIN32
        return reinterpret_cast<fnRegisterplugin*>(GetProcAddress(_handle, "register_pugg_plugin"));
#else
        return reinterpret_cast<fnRegisterplugin*>(dlsym(_handle, "register_pugg_plugin"));
#endif
    }
    void free()
    {
#ifdef WIN32
        if (_handle) { FreeLibrary(_handle); }
#else
        if (_handle) { dlclose(_handle); }
#endif
    }
private:
#ifdef WIN32
	HMODULE _handle;
#else
    void* _handle;
#endif
};

class plugin
{
public:
	plugin() : register_function_(NULL) {}

	bool load(const std::string& filename)
	{
        if (! dll_loader_.load(filename)) return false;
        register_function_ = dll_loader_.register_function();

        if (register_function_) {
            return true;
        } else {
            dll_loader_.free();
            return false;
        }
    }

	void register_plugin(pugg::kernel* kernel)
	{
		register_function_(kernel);
	}
private:

	fnRegisterplugin* register_function_;
    dll_loader dll_loader_;
};



}
}
