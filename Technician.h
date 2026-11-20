#pragma once
#include <string>
#include "Ticket.h"
using namespace std;

class Technician
{
private:
    int tid;               
    string type;             
    int busyTime;            
    Ticket* currentTicket;   

public:
    Technician(int id = 0, string t = "ST");


    int getTID() const { return tid; }
    string getType() const { return type; }
    int getBusyTime() const { return busyTime; }
    Ticket* getCurrentTicket() const { return currentTicket; }

   
    bool isBusy() const;
    void assignTicket(Ticket* t);
    void releaseTicket();
};