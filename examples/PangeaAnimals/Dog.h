#pragma once

#include <pugg/driver.hpp>
#include <Animal.h>

class Dog : public Animal
{
public:
    std::string kind() {return "Dog";}
    bool can_swim() {return true;}
};

class Dogdriver : public Animaldriver
{
public:
    Dogdriver() : Animaldriver("Dogdriver", Dog::version) {}
    Animal* create() {return new Dog();}
};
