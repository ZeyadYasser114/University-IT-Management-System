#pragma once
#include <string>
using namespace std;

class Ticket
{
private:
    int rid;        
    int qt;         
    int lid;       
    string type;    
    int pc;         
    int pr;         
    int at;        
    int st;    
    int ft;         

public:
    Ticket(int r = 0, int q = 0, int l = 0, string t = "NR", int p = 0, int prio = 0);

   
    int getRID() const { return rid; }
    int getQT() const { return qt; }
    int getLID() const { return lid; }
    string getType() const { return type; }
    int getPC() const { return pc; }
    int getPR() const { return pr; }
    int getAT() const { return at; }
    int getST() const { return st; }
    int getFT() const { return ft; }

    
    void setAT(int value) { at = value; }
    void setST(int value) { st = value; }
    void setFT(int value) { ft = value; }

    
    int getWaitingTime() const;      
    int getResolutionTime() const;   
};