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

    bool add_driver(std::string name, std::shared_ptr<driver> d)
    {
        const auto [it, success] = drivers_.insert({name, d});
        return success;

    }

    template<class driver_type>
    std::shared_ptr<driver_type> get_driver(const std::string& name) const
    {
        auto it = drivers_.find(name);
        if (it != drivers_.end())
            return std::dynamic_pointer_cast<driver_type>(it->second);

        return nullptr;
    }

    template<class driver_type>
    std::vector<std::shared_ptr<driver_type>> get_all_drivers() const
    {
        std::vector<std::shared_ptr<driver_type>> drivers;

        for (auto it = drivers_.begin(); it != drivers_.end(); ++it)
        {
            drivers.push_back(std::dynamic_pointer_cast<driver_type>(it->second));
        }

        return drivers;
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

    bool add_driver(std::shared_ptr<pugg::driver> d)
    {
        if (!d)
            return false;

        return server_.add_driver(d->name(), d);
    }

    template<class driver_type>
    std::shared_ptr<driver_type> get_driver(const std::string& name) const
    {
        return server_.get_driver<driver_type>(name);
    }

    template<class driver_type>
    std::vector<std::shared_ptr<driver_type>> get_all_drivers() const
    {
        return server_.get_all_drivers<driver_type>();
    }

    bool load_plugin(const std::string& file_name, const std::string& object_name)
    {
        auto p = std::make_unique<pugg::detail::plugin>();
        if (p->load(file_name))
        {
            add_driver(p->create_driver(object_name));
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
