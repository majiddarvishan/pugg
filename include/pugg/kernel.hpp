#pragma once

#include "driver.hpp"
#include "plugin.hpp"

#include <map>
#include <memory>
#include <string>
#include <vector>

namespace pugg
{
namespace detail
{
class server
{
  public:
    server()
    {
    }

    ~server()
    {
        drivers_.clear();
    }

    std::map<std::string, std::shared_ptr<driver>>& drivers()
    {
        return drivers_;
    }

    void clear()
    {
        drivers_.clear();
    }

  private:
    std::map<std::string, std::shared_ptr<driver>> drivers_;
};
}

class kernel
{
  public:
    kernel() = default;
    virtual ~kernel()
    {
        server_.clear();
        plugins_.clear();
    }

    bool add_driver(std::shared_ptr<pugg::driver> driver)
    {
        if (!driver)
            return false;

        server_.drivers()[driver->name()] = driver;
        return true;
    }

    template<class driverType>
    std::shared_ptr<driverType> get_driver(const std::string& server_name, const std::string& name)
    {
        auto driver_iter = server_.drivers().find(name);
        if (driver_iter == server_.drivers().end())
            return NULL;

        return std::dynamic_pointer_cast<driverType>(driver_iter->second);
    }

    template<class driverType>
    std::vector<std::shared_ptr<driverType>> get_all_drivers(const std::string& server_name)
    {
        std::vector<std::shared_ptr<driverType>> drivers;

        for (auto iter = server_.drivers().begin(); iter != server_.drivers().end(); ++iter)
        {
            drivers.push_back(std::dynamic_pointer_cast<driverType>(iter->second));
        }
        return drivers;
    }

    bool load_plugin(const std::string& filename)
    {
        auto p = std::make_unique<pugg::detail::plugin>();
        if (p->load(filename))
        {
            p->register_plugin(this);
            plugins_.push_back(std::move(p));
            return true;
        }
        return false;
    }

    void clear_drivers()
    {
        server_.clear();
    }

    void clear()
    {
        server_.clear();
        plugins_.clear();
    }

  protected:
    pugg::detail::server server_;
    std::vector<std::unique_ptr<pugg::detail::plugin>> plugins_;
};
} // namespace pugg
