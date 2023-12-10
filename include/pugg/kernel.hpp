//          Copyright Tunc Bahcecioglu 2009 - 2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include <string>
#include <map>
#include <vector>

#include "driver.hpp"
#include "plugin.hpp"

namespace pugg {

class kernel;

namespace detail {

template <class K, class V>
void delete_all_values(std::map<K,V>& map) {
    for (typename std::map<K,V>::iterator iter = map.begin(); iter != map.end(); ++iter) {
        delete iter->second;
    }
}
template <class V>
void delete_all_values(std::vector<V>& vec) {
    for (typename std::vector<V>::iterator iter = vec.begin(); iter != vec.end(); ++iter) {
        delete *iter;
    }
}

class server
{
public:
	server(std::string name, int min_driver_version) : name_(name), min_driver_version_(min_driver_version) {}
    ~server() {delete_all_values(drivers_);}

    std::string name() {return name_;}
    int min_driver_version() {return min_driver_version_;}
    std::map<std::string, driver*>& drivers() {return drivers_;}

	void clear()
	{
		delete_all_values(drivers_);
	}
private:
	std::string name_;
	int min_driver_version_;
	std::map<std::string, driver*> drivers_;
};
}

class kernel
{
public:
	virtual ~kernel()
    {
        pugg::detail::delete_all_values(servers_);
        pugg::detail::delete_all_values(plugins_);
    }

	void add_server(std::string name, int min_driver_version )
	{
        servers_[name] = new pugg::detail::server(name, min_driver_version);
	}

	bool add_driver(pugg::driver* driver)
	{
        if (! driver) return false;

        pugg::detail::server* server = get_server(driver->server_name());
        if (! server) return NULL;

        if (server->min_driver_version() > driver->version()) return false;

        server->drivers()[driver->name()] = driver;
        return true;
	}

    template <class driverType>
    driverType* get_driver(const std::string& server_name, const std::string& name)
    {
        pugg::detail::server* server = get_server(server_name);
        if (! server) return NULL;

        std::map<std::string,pugg::driver*>::iterator driver_iter = server->drivers().find(name);
        if (driver_iter == server->drivers().end()) return NULL;
        return static_cast<driverType*>(driver_iter->second);
    }

    template <class driverType>
    std::vector<driverType*> get_all_drivers(const std::string& server_name)
    {
        std::vector<driverType*> drivers;

        pugg::detail::server* server = get_server(server_name);
        if (! server) return drivers;

        for (std::map<std::string, pugg::driver*>::iterator iter = server->drivers().begin(); iter != server->drivers().end(); ++iter) {
            drivers.push_back(static_cast<driverType*>(iter->second));
        }
        return drivers;
    }


	bool load_plugin(const std::string& filename)
	{
        pugg::detail::plugin* plugin = new pugg::detail::plugin();
        if (plugin->load(filename)) {
            plugin->register_plugin(this);
            plugins_.push_back(plugin);
            return true;
        } else {
            delete plugin;
            return false;
        }
	}

	void clear_drivers()
	{
		for (std::map<std::string,pugg::detail::server*>::iterator iter = servers_.begin(); iter != servers_.end(); ++iter) {
			iter->second->clear();
		}
	}

	void clear()
	{
        pugg::detail::delete_all_values(servers_);
        pugg::detail::delete_all_values(plugins_);
	}

protected:
	std::map<std::string,pugg::detail::server*> servers_;
    std::vector<pugg::detail::plugin*> plugins_;

    pugg::detail::server* get_server(const std::string& name)
    {
        std::map<std::string,pugg::detail::server*>::iterator server_iter = servers_.find(name);
        if (server_iter == servers_.end())
            return NULL;
        else
            return server_iter->second;
    }
};

}





