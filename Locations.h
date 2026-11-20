#pragma once
#include <string>
#include "Location.h"            // From Youssef Haitham

// The only array in the project. Index = LID (1-based), index 0 is unused.
class Locations
{
private:
    Location** arr;              // array of pointers
    int capacity;                // largest LID allowed
    int count;                   // how many locations are stored

public:
    Locations(int maxLocations);
    ~Locations();

    bool add(Location* loc);                             // O(1), stored at index = its LID
    Location* getByLID(int lid) const;                   // O(1), nullptr if invalid or empty
    Location* findByCode(const std::string& code) const; // O(n), nullptr if not found
    bool isEmpty() const;
    int getCount() const;
    void print() const;
    void clear();
};