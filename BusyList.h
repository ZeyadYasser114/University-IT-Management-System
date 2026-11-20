#pragma once
#include "LinkedPriorityQueue.h"      // From Zeyad 
#include "Technician.h"              //  From Youssef Haitham
#include "Ticket.h"                 //   From Youssef haitham 

class BusyList : public LinkedPriorityQueue<Technician*>
{
public:
    BusyList();
    Technician* removeByTicketID(int rid);
};