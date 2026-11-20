#include "Locations.h"
#include <iostream>

Locations::Locations(int maxLocations)
{
    capacity = maxLocations;
    count = 0;
    arr = new Location*[capacity + 1];    // +1 because LID starts at 1
    for (int i = 0; i <= capacity; i++)
        arr[i] = nullptr;
}

Locations::~Locations()
{
    clear();
    delete[] arr;
}

bool Locations::add(Location* loc)
{
    if (loc == nullptr)
        return false;

    int lid = loc->getLID();
    if (lid < 1 || lid > capacity)        // invalid LID
        return false;
    if (arr[lid] != nullptr)              // slot already used
        return false;

    arr[lid] = loc;
    count++;
    return true;
}

Location* Locations::getByLID(int lid) const
{
    if (lid < 1 || lid > capacity)        // out of range fails safely
        return nullptr;
    return arr[lid];
}

Location* Locations::findByCode(const std::string& code) const
{
    for (int i = 1; i <= capacity; i++)
    {
        if (arr[i] != nullptr && arr[i]->getCode() == code)
            return arr[i];
    }
    return nullptr;
}

bool Locations::isEmpty() const
{
    return count == 0;
}

int Locations::getCount() const
{
    return count;
}

void Locations::print() const
{
    for (int i = 1; i <= capacity; i++)
    {
        if (arr[i] != nullptr)
            std::cout << arr[i]->getLID() << " " << arr[i]->getCode() << std::endl;
    }
}

void Locations::clear()
{
    for (int i = 1; i <= capacity; i++)
    {
        delete arr[i];                    // Locations owns its Location objects
        arr[i] = nullptr;
    }
    count = 0;
}