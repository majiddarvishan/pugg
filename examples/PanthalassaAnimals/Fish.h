#pragma once
#include <Animal.h>

class Fish : public Animal
{
public:
    std::string kind() {return "Fish";}
    bool can_swim() {return true;}
};

class Fishdriver : public Animaldriver
{
public:
    Fishdriver() : Animaldriver("Fishdriver", Fish::version) {}
    Animal* create() {return new Fish();}
};