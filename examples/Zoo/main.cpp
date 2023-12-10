#include <pugg/kernel.hpp>

#include "Animal.h"

#include "../PangeaAnimals/Cat.h"

#include <cstdio>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main()
{
    cout << "Zoo example" << endl;
    cout << "Zoo loads animals from plugins" << endl;
    cout << "Loading plugins..." << endl;


    pugg::kernel kernel;
    kernel.add_server(Animal::server_name(), Animal::version);

#ifdef WIN32
    #define DLL_PANTHALASSA_ANIMALS "PanthalassaAnimals/PanthalassaAnimals.dll"
    #define DLL_PANGEA_ANIMALS "PangeaAnimals/PangeaAnimals.dll"
#else
    #define DLL_PANTHALASSA_ANIMALS "./libPanthalassaAnimals.so"
    #define DLL_PANGEA_ANIMALS "./libPangeaAnimals.so"
#endif

    kernel.load_plugin(DLL_PANTHALASSA_ANIMALS);
    kernel.load_plugin(DLL_PANGEA_ANIMALS);

    auto drivers = kernel.get_all_drivers<Animaldriver>(Animal::server_name());
    vector<Animal*> animals;
    for (auto iter = drivers.begin(); iter != drivers.end(); ++iter) {
        Animaldriver& driver = *(*iter);
        animals.push_back(driver.create());
    }


    for(vector<Animal*>::iterator iter = animals.begin(); iter != animals.end(); ++iter) {
        Animal& animal = *(*iter);
        cout << "Animal kind = " << animal.kind() << endl;
        cout << "Can Animal Swim = " << animal.can_swim() << endl;
    }

    for(vector<Animal*>::iterator iter = animals.begin(); iter != animals.end(); ++iter) {
        delete *iter;
    }

    cout << "Press ENTER to exit..." << endl;
    getchar();
}
