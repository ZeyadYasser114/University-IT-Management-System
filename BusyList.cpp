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
        if (cur->item->currentTicket != nullptr &&
            cur->item->currentTicket->rid == rid)
        {
            Technician* tech = cur->item;

            if (prev == nullptr)
                head = cur->next;        // head
            else
                prev->next = cur->next;  // middle or last

            delete cur;                  // node only, not the technician
            count--;
            return tech;
        }
        prev = cur;
        cur = cur->next;
    }
    return nullptr;
}