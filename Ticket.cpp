#include "Ticket.h"

Ticket::Ticket(int r, int q, int l, string t, int p, int prio)
{
    rid = r;
    qt = q;
    lid = l;
    type = t;
    pc = p;
    pr = prio;

    at = -1;
    st = -1;
    ft = -1;
}

int Ticket::getWaitingTime() const
{
    return st - qt;
}

int Ticket::getResolutionTime() const
{
    return ft - qt;
}