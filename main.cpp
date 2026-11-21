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

// ---------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------

// prints PASS / FAIL and counts the failures
void check(string name, bool condition, int& failed)
{
    if (condition)
        cout << "[PASS] " << name << endl;
    else
    {
        cout << "[FAIL] " << name << endl;
        failed++;
    }
}

// creates a ticket with new, and ALSO stores its pointer in "owner".
// owner is only used at the end to delete every ticket exactly once.
Ticket* makeTicket(LinkedQueue<Ticket*>& owner, int rid, int qt, string type, int pr)
{
    Ticket* t = new Ticket(rid, qt, 1, type, 1, pr);
    owner.enqueue(t);
    return t;
}

Technician* makeTech(LinkedQueue<Technician*>& owner, int tid, string type)
{
    Technician* t = new Technician(tid, type);
    owner.enqueue(t);
    return t;
}

// takes the next ticket out of ANY ticket queue and checks its RID
template <class Q>
bool nextRID(Q& q, int expected)
{
    Ticket* t = nullptr;
    if (!q.dequeue(t))
        return false;
    return t->getRID() == expected;
}

// takes the next technician out of ANY technician queue and checks its TID
template <class Q>
bool nextTID(Q& q, int expected)
{
    Technician* t = nullptr;
    if (!q.dequeue(t))
        return false;
    return t->getTID() == expected;
}

// deletes every object whose pointer is stored in owner
template <class T>
void deleteAll(LinkedQueue<T*>& owner)
{
    T* p = nullptr;
    while (owner.dequeue(p))
        delete p;
}

// ---------------------------------------------------------------
// Tests
// ---------------------------------------------------------------

void testEntities(int& failed)
{
    cout << "\n--- Entity classes ---" << endl;

    ProblemConfig pcfg(5, 'H', 30);
    check("ProblemConfig code/category/duration",
          pcfg.getCode() == 5 && pcfg.getCategory() == 'H' && pcfg.getDuration() == 30, failed);

    Location loc(1, "Library");
    check("Location LID/code", loc.getLID() == 1 && loc.getCode() == "Library", failed);

    Ticket t(101, 10, 1, "NR", 5, 2);
    check("Ticket starts with AT/ST/FT = -1",
          t.getAT() == -1 && t.getST() == -1 && t.getFT() == -1, failed);
    check("Ticket starts as WAITING", t.getStatus() == "WAITING", failed);

    t.setST(15);
    t.setFT(45);
    check("WT = ST - QT = 5", t.getWaitingTime() == 5, failed);
    check("Resolution = FT - QT = 35", t.getResolutionTime() == 35, failed);

    Technician tech(1, "ST");
    check("new technician is free", !tech.isBusy(), failed);
    tech.assignTicket(&t);
    check("technician busy after assign", tech.isBusy() && tech.getCurrentTicket() == &t, failed);
    tech.releaseTicket();
    check("busyTime = FT - ST = 30", tech.getBusyTime() == 30, failed);
    check("technician free after release", !tech.isBusy(), failed);
    tech.releaseTicket();
    check("releasing a free technician is safe", tech.getBusyTime() == 30, failed);
}

void testFIFO(int& failed)
{
    cout << "\n--- FIFO queue (LinkedQueue) ---" << endl;

    LinkedQueue<Ticket*> owner;
    LinkedQueue<Ticket*> q;
    Ticket* a = makeTicket(owner, 1, 0, "SR", 0);
    Ticket* b = makeTicket(owner, 2, 0, "SR", 0);
    Ticket* c = makeTicket(owner, 3, 0, "SR", 0);

    q.enqueue(a);
    q.enqueue(b);
    q.enqueue(c);
    check("count = 3", q.getCount() == 3, failed);

    Ticket* front = nullptr;
    q.peek(front);
    check("peek returns the first ticket and does not remove it",
          front == a && q.getCount() == 3, failed);

    check("dequeue order 1", nextRID(q, 1), failed);
    check("dequeue order 2", nextRID(q, 2), failed);
    check("dequeue order 3", nextRID(q, 3), failed);
    check("queue is empty", q.isEmpty() && q.getCount() == 0, failed);

    Ticket* none = nullptr;
    check("dequeue on empty returns false", q.dequeue(none) == false, failed);
    check("peek on empty returns false", q.peek(none) == false, failed);

    // "share, don't copy": the same ticket sits in two lists
    LinkedQueue<Ticket*> other;
    q.enqueue(a);
    other.enqueue(a);
    Ticket* fromQ = nullptr;
    Ticket* fromOther = nullptr;
    q.dequeue(fromQ);
    other.dequeue(fromOther);
    check("both lists point to the SAME ticket object", fromQ == fromOther && fromQ == a, failed);

    deleteAll(owner);
}

void testWaitingUR(int& failed)
{
    cout << "\n--- Waiting_UR (PR desc, QT asc, RID asc) ---" << endl;

    LinkedQueue<Ticket*> owner;
    LinkedPriorityQueue<Ticket*> q(urOrder);

    // makeTicket(owner, RID, QT, type, PR)
    q.enqueue(makeTicket(owner, 1, 10, "UR", 1));
    q.enqueue(makeTicket(owner, 2, 5, "UR", 3));
    q.enqueue(makeTicket(owner, 3, 5, "UR", 3));
    q.enqueue(makeTicket(owner, 4, 2, "UR", 3));
    q.enqueue(makeTicket(owner, 5, 1, "UR", 2));

    check("count = 5", q.getCount() == 5, failed);
    check("1st: RID 4 (PR 3, earliest QT)", nextRID(q, 4), failed);
    check("2nd: RID 2 (PR 3, QT 5, smaller RID)", nextRID(q, 2), failed);
    check("3rd: RID 3 (PR 3, QT 5, bigger RID)", nextRID(q, 3), failed);
    check("4th: RID 5 (PR 2)", nextRID(q, 5), failed);
    check("5th: RID 1 (PR 1)", nextRID(q, 1), failed);
    check("queue is empty", q.isEmpty(), failed);

    deleteAll(owner);
}

void testFreeTechs(int& failed)
{
    cout << "\n--- Free_ST / Free_GT (TID ascending) ---" << endl;

    LinkedQueue<Technician*> owner;
    LinkedPriorityQueue<Technician*> freeST(freeTechOrder);

    freeST.enqueue(makeTech(owner, 5, "ST"));
    freeST.enqueue(makeTech(owner, 2, "ST"));
    freeST.enqueue(makeTech(owner, 9, "ST"));
    freeST.enqueue(makeTech(owner, 1, "ST"));

    Technician* top = nullptr;
    freeST.peek(top);
    check("peek = smallest TID (1)", top != nullptr && top->getTID() == 1, failed);
    check("1st: TID 1", nextTID(freeST, 1), failed);
    check("2nd: TID 2", nextTID(freeST, 2), failed);
    check("3rd: TID 5", nextTID(freeST, 5), failed);
    check("4th: TID 9", nextTID(freeST, 9), failed);

    deleteAll(owner);
}

void testCompleted(int& failed)
{
    cout << "\n--- Completed_Tickets (FT asc, RID asc) ---" << endl;

    LinkedQueue<Ticket*> owner;
    LinkedPriorityQueue<Ticket*> done(completedOrder);

    Ticket* t7 = makeTicket(owner, 7, 0, "NR", 0);  t7->setFT(30);
    Ticket* t3 = makeTicket(owner, 3, 0, "NR", 0);  t3->setFT(20);
    Ticket* t5 = makeTicket(owner, 5, 0, "NR", 0);  t5->setFT(30);
    Ticket* t1 = makeTicket(owner, 1, 0, "NR", 0);  t1->setFT(20);

    done.enqueue(t7);
    done.enqueue(t3);
    done.enqueue(t5);
    done.enqueue(t1);

    check("1st: RID 1 (FT 20, smaller RID)", nextRID(done, 1), failed);
    check("2nd: RID 3 (FT 20)", nextRID(done, 3), failed);
    check("3rd: RID 5 (FT 30, smaller RID)", nextRID(done, 5), failed);
    check("4th: RID 7 (FT 30)", nextRID(done, 7), failed);

    deleteAll(owner);
}

void testNRCancel(int& failed)
{
    cout << "\n--- NRQueue: cancel from the middle ---" << endl;

    LinkedQueue<Ticket*> owner;
    NRQueue nr;
    nr.enqueue(makeTicket(owner, 10, 0, "NR", 0));
    nr.enqueue(makeTicket(owner, 11, 0, "NR", 0));
    nr.enqueue(makeTicket(owner, 12, 0, "NR", 0));
    nr.enqueue(makeTicket(owner, 13, 0, "NR", 0));

    Ticket* found = nr.findByID(12);
    check("findByID(12) finds it and does not remove it",
          found != nullptr && found->getRID() == 12 && nr.getCount() == 4, failed);
    check("findByID(99) returns nullptr", nr.findByID(99) == nullptr, failed);

    Ticket* cancelled = nr.cancelByID(11);                     // middle
    check("cancel middle (11) returns that ticket",
          cancelled != nullptr && cancelled->getRID() == 11, failed);
    check("count = 3 after cancel", nr.getCount() == 3, failed);
    check("cancelled ticket is gone", nr.findByID(11) == nullptr, failed);

    Ticket* head = nr.cancelByID(10);                          // head
    check("cancel head (10)", head != nullptr && head->getRID() == 10, failed);

    Ticket* tail = nr.cancelByID(13);                          // tail
    check("cancel tail (13)", tail != nullptr && tail->getRID() == 13, failed);
    check("count = 1", nr.getCount() == 1, failed);

    nr.enqueue(makeTicket(owner, 14, 0, "NR", 0));             // rear pointer must still be right
    check("order kept after cancels: 12", nextRID(nr, 12), failed);
    check("order kept after cancels: 14", nextRID(nr, 14), failed);

    check("cancelByID(99) on empty queue returns nullptr", nr.cancelByID(99) == nullptr, failed);

    deleteAll(owner);
}

void testBusyList(int& failed)
{
    cout << "\n--- BusyList (FT asc, removeByTicketID) ---" << endl;

    LinkedQueue<Ticket*> ownerT;
    LinkedQueue<Technician*> ownerTech;
    BusyList busy;

    // each technician gets a ticket with its own FT
    Ticket* t21 = makeTicket(ownerT, 21, 0, "NR", 0);  t21->setST(10);  t21->setFT(50);
    Ticket* t22 = makeTicket(ownerT, 22, 0, "NR", 0);  t22->setST(10);  t22->setFT(30);
    Ticket* t23 = makeTicket(ownerT, 23, 0, "NR", 0);  t23->setST(10);  t23->setFT(40);
    Ticket* t20 = makeTicket(ownerT, 20, 0, "NR", 0);  t20->setST(10);  t20->setFT(30);   // same FT as 22

    Technician* a = makeTech(ownerTech, 1, "ST");  a->assignTicket(t21);
    Technician* b = makeTech(ownerTech, 2, "GT");  b->assignTicket(t22);
    Technician* c = makeTech(ownerTech, 3, "ST");  c->assignTicket(t23);
    Technician* d = makeTech(ownerTech, 4, "GT");  d->assignTicket(t20);

    busy.enqueue(a);
    busy.enqueue(b);
    busy.enqueue(c);
    busy.enqueue(d);

    Technician* top = nullptr;
    busy.peek(top);
    check("peek = technician of RID 20 (FT 30 tie, smaller RID first)",
          top == d, failed);

    Technician* removed = busy.removeByTicketID(23);           // middle
    check("removeByTicketID(23) returns the technician serving it", removed == c, failed);
    check("count = 3 after removal", busy.getCount() == 3, failed);

    removed->releaseTicket();
    check("released technician is free again", !removed->isBusy(), failed);
    check("released technician busyTime = 30", removed->getBusyTime() == 30, failed);

    check("removeByTicketID(99) returns nullptr", busy.removeByTicketID(99) == nullptr, failed);
    check("count still 3", busy.getCount() == 3, failed);

    Technician* head = busy.removeByTicketID(20);              // head
    check("remove head (RID 20)", head == d, failed);
    Technician* last = busy.removeByTicketID(21);              // last
    check("remove last (RID 21)", last == a, failed);

    check("only RID 22 is left", nextTID(busy, 2), failed);
    check("BusyList is empty", busy.isEmpty(), failed);

    deleteAll(ownerTech);
    deleteAll(ownerT);
}

void testCleanup(int& failed)
{
    cout << "\n--- Pointer cleanup ---" << endl;

    LinkedQueue<Ticket*> owner;
    LinkedQueue<Technician*> ownerTech;
    LinkedQueue<Ticket*> waiting;
    BusyList busy;

    Ticket* t = makeTicket(owner, 1, 0, "NR", 0);
    t->setST(0);
    t->setFT(10);
    Technician* tech = makeTech(ownerTech, 1, "ST");

    waiting.enqueue(t);

    // move the ticket: waiting -> technician -> busy list
    Ticket* moved = nullptr;
    waiting.dequeue(moved);
    tech->assignTicket(moved);
    busy.enqueue(tech);
    check("ticket moved without copying", tech->getCurrentTicket() == t, failed);

    // finish: busy list -> technician released
    Technician* freeTech = nullptr;
    busy.dequeue(freeTech);
    freeTech->releaseTicket();
    check("technician pointer is nullptr after release",
          freeTech->getCurrentTicket() == nullptr, failed);

    // queues only delete their nodes; we delete the objects ONCE here
    waiting.clear();
    busy.clear();
    deleteAll(ownerTech);
    deleteAll(owner);
    check("owners are empty (every object deleted once)",
          owner.isEmpty() && ownerTech.isEmpty(), failed);
}

void testPrint()
{
    cout << "\n--- print() / operator<< ---" << endl;

    LinkedQueue<Ticket*> owner;
    LinkedQueue<Ticket*> q;
    LinkedPriorityQueue<Ticket*> pq(urOrder);

    q.enqueue(makeTicket(owner, 1, 0, "SR", 0));
    q.enqueue(makeTicket(owner, 2, 0, "SR", 0));
    pq.enqueue(makeTicket(owner, 3, 0, "UR", 1));
    pq.enqueue(makeTicket(owner, 4, 0, "UR", 5));

    cout << "FIFO queue     (expect: 1 2): ";
    q.print();
    cout << endl;
    cout << "Priority queue (expect: 4 3): ";
    pq.print();

    q.clear();
    pq.clear();
    deleteAll(owner);
}

int main()
{
    int failed = 0;

    testEntities(failed);
    testFIFO(failed);
    testWaitingUR(failed);
    testFreeTechs(failed);
    testCompleted(failed);
    testNRCancel(failed);
    testBusyList(failed);
    testCleanup(failed);
    testPrint();

    cout << "\n==============================" << endl;
    if (failed == 0)
        cout << "ALL TESTS PASSED" << endl;
    else
        cout << failed << " TEST(S) FAILED" << endl;

    return 0;
}
