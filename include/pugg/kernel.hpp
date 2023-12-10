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
    server(std::string name, int min_driver_version)
        : name_(name)
        , min_driver_version_(min_driver_version)
    {
    }

    ~server()
    {
        drivers_.clear();
    }

    std::string name()
    {
        return name_;
    }

    int min_driver_version()
    {
        return min_driver_version_;
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
    std::string name_;
    int min_driver_version_;
    std::map<std::string, std::shared_ptr<driver>> drivers_;
};
} // namespace detail

class kernel
{
  public:
    virtual ~kernel()
    {
        servers_.clear();
        plugins_.clear();
    }

    void add_server(std::string name, int min_driver_version)
    {
        servers_[name] = new pugg::detail::server(name, min_driver_version);
    }

    bool add_driver(std::shared_ptr<pugg::driver> driver)
    {
        if (!driver)
            return false;

        pugg::detail::server* server = get_server(driver->server_name());
        if (!server)
            return NULL;

        if (server->min_driver_version() > driver->version())
            return false;

        server->drivers()[driver->name()] = driver;
        return true;
    }

    template<class driverType>
    std::shared_ptr<driverType> get_driver(const std::string& server_name, const std::string& name)
    {
        pugg::detail::server* server = get_server(server_name);
        if (!server)
            return NULL;

        auto driver_iter = server->drivers().find(name);
        if (driver_iter == server->drivers().end())
            return NULL;

        return std::dynamic_pointer_cast<driverType>(driver_iter->second);
    }

    template<class driverType>
    std::vector<std::shared_ptr<driverType>> get_all_drivers(const std::string& server_name)
    {
        std::vector<std::shared_ptr<driverType>> drivers;

        pugg::detail::server* server = get_server(server_name);
        if (!server)
            return drivers;

        for (auto iter = server->drivers().begin(); iter != server->drivers().end(); ++iter)
        {
            drivers.push_back(std::dynamic_pointer_cast<driverType>(iter->second));
        }
        return drivers;
    }

    bool load_plugin(const std::string& filename)
    {
        pugg::detail::plugin* plugin = new pugg::detail::plugin();
        if (plugin->load(filename))
        {
            plugin->register_plugin(this);
            plugins_.push_back(plugin);
            return true;
        }
        else
        {
            delete plugin;
            return false;
        }
    }

    void clear_drivers()
    {
        for (std::map<std::string, pugg::detail::server*>::iterator iter = servers_.begin(); iter != servers_.end(); ++iter)
        {
            iter->second->clear();
        }
    }

    void clear()
    {
        servers_.clear();
        plugins_.clear();
    }

  protected:
    std::map<std::string, pugg::detail::server*> servers_;
    std::vector<pugg::detail::plugin*> plugins_;

    pugg::detail::server* get_server(const std::string& name)
    {
        std::map<std::string, pugg::detail::server*>::iterator server_iter = servers_.find(name);
        if (server_iter == servers_.end())
            return NULL;

        return server_iter->second;
    }
};
} // namespace pugg
