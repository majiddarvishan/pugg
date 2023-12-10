#pragma once

#include "../Zoo/Animal.h"

class Cat : public Animal
{
public:
    std::string kind() {return "Cat";}
    bool can_swim() {return false;}
};

class Catdriver : public Animaldriver
{
public:
    Catdriver() : Animaldriver("Catdriver", Cat::version) {}
    Animal* create() {return new Cat();}
};