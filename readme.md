# Pugg 1.0.0

#### A Simple C++ Framework to load classes from dll files in an OO way.
<br/>

**Contact** For bugs, feature requests and other stuff:

Tunç Bahçecioğlu tunc.bahcecioglu@gmail.com

- Pugg @ Bitbucket

- Pugg @ Sourceforge

- Pugg Website

## Credits

- **nuclex** blog for giving me the idea and the basic source code to implement pugg.

- Sebastian (hackspider) for bugfix.

- Szymon Janikowski for fix in old tutorial.

- Alex Elzenaar for code improvements and code for Linux support.

## Features

- Auto loading of dll files and classes

- Version control for loaded plugins

- Header only library

- OOP Design

- Platform independent

## Requirements

- Somewhat current C++ compiler (No C++11 features were used in this
 version)

## Installation

No installation is required. Files to include are provided in the include folder of the release. On some Linux platforms you need to link with libdl library.

In order to build example projects use **CMake**

## Tutorial

Suppose you have an animal class:

```C++
class Animal
{
public :
    Animal() {}
    virtual ~Animal() {}

    virtual std::string kind() = 0 ;
    virtual bool can_swim() = 0 ;
};
```
You want to load subclasses of Animal from dll files.

Add a version and a name to create a server:

```c++
class Animal
{
public:
    // ....
    static const int version = 1;
    static const std::string server_name() {return "Animalserver";}
};
```
Then add a driver to tell the name and version of the server to Pugg.

```C++
class Animaldriver : public pugg::driver
{
public :
    Animaldriver(std::string name, int version)
        : pugg::driver(Animal::server_name(),name,version) {}
    virtual Animal* create() = 0 ;
};
```
Now in a separate project create your dll. Lets create a cat and its driver:

```C++
class Cat : public Animal
{
public :
    std::string kind() { return "Cat";}
    bool can_swim() { return false ;}
};


class Catdriver : public Animaldriver
{
public :
    Catdriver() : Animaldriver("Catdriver", Cat::version) {}
    Animal* create() { return new Cat();}
};
```

Note that version is the same version from the Animal class at the time Cat is compiled. If Animal class changes in the future and increases its version Cat class will be outdated and won't be loaded by Pugg.

</br>
Let's export the Cat class from dll.

```C++
#include <pugg\kernel.hpp>
#include "Cat.h"

#ifdef _WIN
# define EXPORTIT __declspec( dllexport )
#else
# define EXPORTIT
#endif

extern "C" EXPORTIT void register_pugg_plugin(pugg::kernel* kernel)
{
    kernel->add_driver( new Catdriver());
}
```
register_pugg_plugin function is a special function called by Pugg to load drivers from a dll file.

Now in our main app, we can import dlls and have fun

```C++
#include <pugg\kernel.hpp>
#include <Animal.h>

#include <iostream>
#include <vector>

using namespace std;

int main()
{
    cout << "Loading plugins..." << endl;
    pugg::kernel kernel;
    kernel.add_server(Animal::server_name(), Animal::version);
    #ifdef WIN
    kernel.load_plugin("PangeaAnimals.dll"); // Cat class is in this dll.
    #else
    kernel.load_plugin("libPangeaAnimals.so");
    #endif
    ```
    ```
    // we can load all drivers from a specific server
    vector<Animaldriver*> drivers =kernel.get_all_drivers<Animaldriver>(Animal::server_name());
    // to load a specific driver
    ```

    ```
    // Animaldriver* animal_driver = kernel.get_driver<Animaldriver>(Animal::server_name, "Catdriver");
    vector<Animal*> animals;
    for (vector<Animaldriver*>::iterator iter = drivers.begin(); iter != drivers.end(); ++iter) {
    Animaldriver& driver = *(*iter);
    animals.push_back(driver.create());
    }
    ```
    ```
    for (vector<Animal*>::iterator iter = animals.begin(); iter != animals.end(); ++iter) {
    Animal& animal = *(*iter);
    cout << "Animal kind = " << animal.kind() << endl;
    cout << "Can Animal Swim = " << animal.can_swim() << endl;
    }

    for (vector<Animal*>::iterator iter = animals.begin(); iter != animals.end(); ++iter) {
    delete *iter;
    }
}
```

## Class Documentation

### driver

#### driver class registers user class to Pugg.

```C++
// /include/pugg/driver.hpp
namespace pugg {
class driver
{
    public :
    // server_name : Name of the server driver belongs to.
    // name : Name of the driver
    // version : version of the driver
    driver(std::string server_name, std::string name, int version)
    virtual ~driver() {}
    ```
    ```
    std::string server_name() const { return _server_name;}
    std::string name() const { return _name;}
    int version() const { return _version;}
};
}
```

### kernel
```C++
// /include/pugg/kernel.hpp
namespace pugg {
class kernel
{
public :
    ~kernel(); // destroys drivers, servers and links to dll files.
    // loads drivers from the dll file
    bool load_plugin( const std::string& filename);
    // adds a server
    // name : name of the server
    // min_driver_version : minimum driver version to load to server.
    // Lower version drivers are not loaded.
    void add_server(std::string name, int min_driver_version );
    // adds a driver. usually called in the register_pugg_plugin function of dlls.
    bool add_driver(pugg::driver* driver);

    // gets a specific driver from a server
    template < class driverType>
    driverType* get_driver( const std::string& server_name, const std::string& name);
    // gets all drivers from a server
    template < class driverType>
    std::vector<driverType*> get_all_drivers( const std::string& server_name);
    // clears all drivers
    void clear_drivers();
    // clears all drivers, servers and plugins
    void clear();
};
}
```

## Release Notes

### 1.0.

Added clear methods. 1.0.0 release.

### 0.60

Finally Pugg supports Linux. CMAKE is used for creating the example projects on different platforms.

#### 0.50

This version is an attempt to get back to Pugg development. Thus I replaced most of the code from the previous version with the version I use in my own projects. I omitted C++11 code (mostly auto and unique_ptr) in order not to break old compilers.
</br>
Wstring support is removed as it complicates the code a and I don't think it is used by anyone (If I'm wrong mail me and I will add it back).
</br>
Name convention changes break old code but it won't be very hard to replace old code.

## History

### 1.0.

- Main repository is moved to Bitbucket.

- Added clear methods.

- 1.0.0 release 0.60

- Linux support

- Bug fixes, Code improvements etc...

### 0.55

- Switched to CMAKE build system for example projects.

### 0.51

- Removed unnecessary try catch block from plugin.cpp

### 0.5

- New Documentation

- Changed to Boost license for more freedom.

- Name convention changes

- wstring support is removed

- server class is in kernel.hpp and is used from kernel class.

- Changed to mercurial

### 0.41

- A memory leak occurring while trying to load dll files not having the registerplugin function is fixed.

### 0.4

- std::wstring file name support

### 0.3

- Name convention changes

- Release pack for the library and related example projects are put together

- More source code Documentation

### 0.21

- Bug Fix related to UNICODE support


### 0.2

- Version control system on plug-in system is removed. Every driver's version is controlled separately.

- server class became template, removing the need to write a server class for every driver and casting the drivers before usage.

### 0.11

- Bug fixes

### 0.1

- Initial release


