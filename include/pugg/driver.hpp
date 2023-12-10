#pragma once

#include <string>

namespace pugg
{
class driver
{
  public:
    driver(std::string server_name, std::string name, int version)
        : server_name_(server_name)
        , name_(name)
        , version_(version)
    {
    }

    virtual ~driver()
    {
    }

    std::string server_name() const
    {
        return server_name_;
    }

    std::string name() const
    {
        return name_;
    }

    int version() const
    {
        return version_;
    }

  private:
    std::string name_;
    std::string server_name_;
    int version_;
};
} // namespace pugg