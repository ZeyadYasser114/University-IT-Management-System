#pr#pragma once
#include "LinkedPriorityQueue.h"      // From Zeyad
#include "Technician.h"              // From Youssef Haitham 
#include "Ticket.h"                 // From Youssef Haitham 

class BusyList : public LinkedPriorityQueue<Technician*>
{
public:
    BusyList();
    Technician* removeByTicketID(int rid);   // O(n), returns nullptr if not found
};
