#pragma once
#include <pugg/driver.hpp>
#include <string>

class Animal
{
public:
    Animal() {}
    virtual ~Animal() {}

    virtual std::string kind() = 0;
    virtual bool can_swim() = 0;

    static const int version = 1;
    static const std::string server_name() {return "Animalserver";}
};

class Animaldriver : public pugg::driver
{
public:
    Animaldriver(std::string name, int version) : pugg::driver(Animal::server_name(),name,version) {}
    virtual Animal* create() = 0;
};
