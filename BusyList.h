#pragma once
#include "LinkedPriorityQueue.h"
#include "Technician.h"
#include "Ticket.h"

class BusyList : public LinkedPriorityQueue<Technician*>
{
public:
    BusyList();
    Technician* removeByTicketID(int rid);   // O(n), returns nullptr if not found
};
