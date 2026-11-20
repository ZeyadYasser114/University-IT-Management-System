#pragma once
#include "LinkedQueue.h"        // From Omar
#include "ProblemConfig.h"      // From Youssef Haitham

class ConfigList : public LinkedQueue<ProblemConfig*>
{
public:
    ProblemConfig* findByCode(int code) const;   // O(n), does NOT remove the entry
};