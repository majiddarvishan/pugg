#pragma once
#include <Animal.h>

class Whale : public Animal
{
public:
    std::string kind() {return "Whale";}
    bool can_swim() {return true;}
};

class Whaledriver : public Animaldriver
{
public:
    Whaledriver() : Animaldriver("Whaledriver", Whale::version) {}
    Animal* create() {return new Whale();}
};