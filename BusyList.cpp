#include "BusyList.h"

// Earliest finish time first, ties by smaller RID
static bool busyOrder(Technician* const& a, Technician* const& b)
{
    Ticket* ta = a->getCurrentTicket();
    Ticket* tb = b->getCurrentTicket();

    if (ta->getFT() != tb->getFT())
        return ta->getFT() < tb->getFT();
    return ta->getRID() < tb->getRID();
}

BusyList::BusyList() : LinkedPriorityQueue<Technician*>(busyOrder)
{
}

Technician* BusyList::removeByTicketID(int rid)
{
    Node<Technician*>* prev = nullptr;
    Node<Technician*>* cur = head;

    while (cur != nullptr)
    {
        Ticket* t = cur->data->getCurrentTicket();

        if (t != nullptr && t->getRID() == rid)
        {
            Technician* tech = cur->data;

            if (prev == nullptr)
                head = cur->next;        // removing the head
            else
                prev->next = cur->next;  // middle or last node

            delete cur;                  // delete the node only, not the technician
            count--;
            return tech;
        }
        prev = cur;
        cur = cur->next;
    }
    return nullptr;                      // empty list or not found
}
