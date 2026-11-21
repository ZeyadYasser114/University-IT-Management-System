#include "Technician.h"

Technician::Technician(int id, string t)
{
    tid = id;
    type = t;
    busyTime = 0;
    currentTicket = nullptr;
}

bool Technician::isBusy() const
{
    return currentTicket != nullptr;
}

void Technician::assignTicket(Ticket* t)
{
    currentTicket = t;
}

void Technician::releaseTicket()
{
    if (currentTicket != nullptr)
    {
        busyTime = busyTime + (currentTicket->getFT() - currentTicket->getST());
        currentTicket = nullptr;
    }
}

ostream& operator<<(ostream& out, const Technician& t)
{
    out << t.getTID();
    return out;
}

ostream& operator<<(ostream& out, const Technician* t)
{
    out << t->getTID();
    return out;
}
