#include "BusyList.h"

// Earliest finish time first, ties by smaller RID   
static bool busyOrder(Technician* const& a, Technician* const& b)
{
    if (a->currentTicket->ft != b->currentTicket->ft)
        return a->currentTicket->ft < b->currentTicket->ft;
    return a->currentTicket->rid < b->currentTicket->rid;
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
        if (cur->data->currentTicket != nullptr &&
            cur->data->currentTicket->rid == rid)
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
