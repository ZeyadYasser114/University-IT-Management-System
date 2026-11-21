#include <iostream>
#include "ProblemConfig.h"
#include "Location.h"
#include "Ticket.h"
#include "Technician.h"
#include "LinkedQueue.h"
#include "LinkedPriorityQueue.h"
#include "NRQueue.h"
#include "BusyList.h"
using namespace std;

void check(string name, bool condition)
{
    if (condition)
        cout << "[PASS] " << name << endl;
    else
        cout << "[FAIL] " << name << endl;
}

int main()
{
    cout << "--- Entity classes ---" << endl;
    ProblemConfig pcfg(5, 'H', 30);
    check("ProblemConfig", pcfg.getCode() == 5 && pcfg.getCategory() == 'H' && pcfg.getDuration() == 30);

    Location loc(1, "Library");
    check("Location", loc.getLID() == 1 && loc.getCode() == "Library");

    Ticket t(101, 10, 1, "NR", 5, 2);
    t.setST(15);
    t.setFT(45);
    check("WT = ST - QT = 5", t.getWaitingTime() == 5);
    check("Resolution = FT - QT = 35", t.getResolutionTime() == 35);

    Technician tech(1, "ST");
    tech.assignTicket(&t);
    check("technician is busy", tech.isBusy());
    tech.releaseTicket();
    check("technician is free, busyTime = 30", !tech.isBusy() && tech.getBusyTime() == 30);

    cout << "--- FIFO queue ---" << endl;
    Ticket* a = new Ticket(1, 0, 1, "SR", 1, 0);
    Ticket* b = new Ticket(2, 0, 1, "SR", 1, 0);
    Ticket* c = new Ticket(3, 0, 1, "SR", 1, 0);

    LinkedQueue<Ticket*> fifo;
    fifo.enqueue(a);
    fifo.enqueue(b);
    fifo.enqueue(c);

    Ticket* x = nullptr;
    fifo.dequeue(x);
    check("1st out = RID 1", x->getRID() == 1);
    fifo.dequeue(x);
    check("2nd out = RID 2", x->getRID() == 2);
    fifo.dequeue(x);
    check("3rd out = RID 3", x->getRID() == 3);
    check("queue is empty", fifo.isEmpty());

    cout << "--- Waiting_UR ordering ---" << endl;
    LinkedPriorityQueue<Ticket*> ur(urOrder);
    Ticket* u1 = new Ticket(1, 10, 1, "UR", 1, 1);
    Ticket* u2 = new Ticket(2, 5, 1, "UR", 1, 3);
    Ticket* u3 = new Ticket(3, 5, 1, "UR", 1, 3);
    Ticket* u4 = new Ticket(4, 2, 1, "UR", 1, 3);
    ur.enqueue(u1);
    ur.enqueue(u2);
    ur.enqueue(u3);
    ur.enqueue(u4);

    ur.dequeue(x);
    check("1st = RID 4 (same PR, earliest QT)", x->getRID() == 4);
    ur.dequeue(x);
    check("2nd = RID 2 (tie on PR and QT, smaller RID)", x->getRID() == 2);
    ur.dequeue(x);
    check("3rd = RID 3", x->getRID() == 3);
    ur.dequeue(x);
    check("4th = RID 1 (lowest PR)", x->getRID() == 1);

    cout << "--- Free_ST ordering ---" << endl;
    LinkedPriorityQueue<Technician*> freeST(freeTechOrder);
    Technician* f1 = new Technician(5, "ST");
    Technician* f2 = new Technician(2, "ST");
    Technician* f3 = new Technician(9, "ST");
    freeST.enqueue(f1);
    freeST.enqueue(f2);
    freeST.enqueue(f3);

    Technician* y = nullptr;
    freeST.dequeue(y);
    check("1st = TID 2", y->getTID() == 2);
    freeST.dequeue(y);
    check("2nd = TID 5", y->getTID() == 5);
    freeST.dequeue(y);
    check("3rd = TID 9", y->getTID() == 9);

    cout << "--- Completed_Tickets ordering ---" << endl;
    LinkedPriorityQueue<Ticket*> done(completedOrder);
    Ticket* d1 = new Ticket(7, 0, 1, "NR", 1, 0);
    Ticket* d2 = new Ticket(3, 0, 1, "NR", 1, 0);
    Ticket* d3 = new Ticket(5, 0, 1, "NR", 1, 0);
    d1->setFT(30);
    d2->setFT(20);
    d3->setFT(30);
    done.enqueue(d1);
    done.enqueue(d2);
    done.enqueue(d3);

    done.dequeue(x);
    check("1st = RID 3 (FT 20)", x->getRID() == 3);
    done.dequeue(x);
    check("2nd = RID 5 (FT 30, smaller RID)", x->getRID() == 5);
    done.dequeue(x);
    check("3rd = RID 7 (FT 30)", x->getRID() == 7);

    cout << "--- NRQueue cancel ---" << endl;
    NRQueue nr;
    Ticket* n1 = new Ticket(10, 0, 1, "NR", 1, 0);
    Ticket* n2 = new Ticket(11, 0, 1, "NR", 1, 0);
    Ticket* n3 = new Ticket(12, 0, 1, "NR", 1, 0);
    Ticket* n4 = new Ticket(13, 0, 1, "NR", 1, 0);
    Ticket* n5 = new Ticket(14, 0, 1, "NR", 1, 0);
    nr.enqueue(n1);
    nr.enqueue(n2);
    nr.enqueue(n3);
    nr.enqueue(n4);

    check("cancel middle (11) returns that ticket", nr.cancelByID(11) == n2);
    check("cancel head (10) returns that ticket", nr.cancelByID(10) == n1);
    check("cancel tail (13) returns that ticket", nr.cancelByID(13) == n4);
    check("cancelByID(99) returns nullptr", nr.cancelByID(99) == nullptr);
    check("count = 1", nr.getCount() == 1);

    nr.enqueue(n5);
    nr.dequeue(x);
    check("order kept: 12 first", x->getRID() == 12);
    nr.dequeue(x);
    check("order kept: 14 second", x->getRID() == 14);

    cout << "--- BusyList ---" << endl;
    BusyList busy;
    Ticket* b21 = new Ticket(21, 0, 1, "NR", 1, 0);
    Ticket* b22 = new Ticket(22, 0, 1, "NR", 1, 0);
    Ticket* b23 = new Ticket(23, 0, 1, "NR", 1, 0);
    b21->setST(10);  b21->setFT(50);
    b22->setST(10);  b22->setFT(30);
    b23->setST(10);  b23->setFT(40);

    Technician* k1 = new Technician(1, "ST");
    Technician* k2 = new Technician(2, "GT");
    Technician* k3 = new Technician(3, "ST");
    k1->assignTicket(b21);
    k2->assignTicket(b22);
    k3->assignTicket(b23);
    busy.enqueue(k1);
    busy.enqueue(k2);
    busy.enqueue(k3);

    busy.peek(y);
    check("peek = technician with earliest FT (ticket 22)", y == k2);

    Technician* removed = busy.removeByTicketID(23);
    check("removeByTicketID(23) returns its technician", removed == k3);
    check("count = 2", busy.getCount() == 2);
    check("removeByTicketID(99) returns nullptr", busy.removeByTicketID(99) == nullptr);

    cout << "--- Pointer cleanup ---" << endl;
    removed->releaseTicket();
    check("pointer is nullptr after release", removed->getCurrentTicket() == nullptr);

    busy.clear();
    nr.clear();

    delete a;  delete b;  delete c;
    delete u1; delete u2; delete u3; delete u4;
    delete f1; delete f2; delete f3;
    delete d1; delete d2; delete d3;
    delete n1; delete n2; delete n3; delete n4; delete n5;
    delete b21; delete b22; delete b23;
    delete k1; delete k2; delete k3;

    cout << "--- Done ---" << endl;
    return 0;
}
