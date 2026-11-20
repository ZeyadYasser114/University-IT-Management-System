#pragma once
#include <string>
using namespace std;

class Location
{
private:
    int lid;     
    string code;   

public:
    Location(int id = 0, string c = "")
    {
        lid = id;
        code = c;
    }

    int getLID() const { return lid; }
    string getCode() const { return code; }
};