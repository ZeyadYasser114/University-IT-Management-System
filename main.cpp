#include <iostream>
#include "ProblemConfig.h"
#include "Location.h"
#include "Ticket.h"
#include "Technician.h"
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
   
    cout << "--- ProblemConfig ---" << endl;
    ProblemConfig pcfg(5, 'H', 30);
    check("code = 5", pcfg.getCode() == 5);
    check("category = H", pcfg.getCategory() == 'H');
    check("duration = 30", pcfg.getDuration() == 30);

    
    cout << "--- Location ---" << endl;
    Location loc(1, "Library");
    check("LID = 1", loc.getLID() == 1);
    check("code = Library", loc.getCode() == "Library");

   
    cout << "--- Ticket ---" << endl;
    Ticket t(101, 10, 1, "NR", 5, 2);
    check("RID = 101", t.getRID() == 101);
    check("ST default = -1", t.getST() == -1);

    t.setST(15);
    t.setFT(45);
    check("WT = ST - QT = 5", t.getWaitingTime() == 5);
    check("Resolution = FT - QT = 35", t.getResolutionTime() == 35);

    
    cout << "--- Technician ---" << endl;
    Technician tech(1, "ST");
    check("new technician is free", tech.isBusy() == false);

    tech.assignTicket(&t);
    check("busy after assign", tech.isBusy() == true);
    check("current ticket is t", tech.getCurrentTicket() == &t);

    tech.releaseTicket();
    check("busyTime = 30", tech.getBusyTime() == 30);
    check("free after release", tech.isBusy() == false);

    tech.releaseTicket();   
    check("double release is safe", tech.getBusyTime() == 30);

    
    cout << "--- Pointers ---" << endl;
    Ticket* p = new Ticket(102, 20, 2, "SR", 3, 1);
    Technician* tp = new Technician(2, "GT");

    tp->assignTicket(p);
    check("technician holds the same ticket", tp->getCurrentTicket() == p);

    p->setST(25);
    p->setFT(40);
    tp->releaseTicket();
    check("pointer is nullptr after release", tp->getCurrentTicket() == nullptr);

    delete p;
    delete tp;

    cout << "--- Done ---" << endl;
    return 0;
}