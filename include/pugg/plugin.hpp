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

namespace pugg
{
class kernel;

namespace detail
{
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
        handle_ = LoadLibraryA(filename.c_str());
#else
        handle_ = dlopen(filename.c_str(), RTLD_NOW);
#endif
        return (handle_ != NULL);
    }

    fnRegisterplugin* register_function()
    {
#ifdef WIN32
        return reinterpret_cast<fnRegisterplugin*>(GetProcAddress(handle_, "register_pugg_plugin"));
#else
        return reinterpret_cast<fnRegisterplugin*>(dlsym(handle_, "register_pugg_plugin"));
#endif
    }

    void free()
    {
#ifdef WIN32
        if (handle_)
        {
            FreeLibrary(handle_);
        }
#else
        if (handle_)
        {
            dlclose(handle_);
        }
#endif
    }

  private:
#ifdef WIN32
    HMODULE handle_;
#else
    void* handle_;
#endif
};

class plugin
{
  public:
    plugin()
        : register_function_(nullptr)
    {
    }

    bool load(const std::string& filename)
    {
        if (!dll_loader_.load(filename))
            return false;
        register_function_ = dll_loader_.register_function();

        if (register_function_)
        {
            return true;
        }
        else
        {
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
} // namespace detail
} // namespace pugg
