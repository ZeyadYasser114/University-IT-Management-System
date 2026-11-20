#include "ConfigList.h"

ProblemConfig* ConfigList::findByCode(int code) const
{
    Node<ProblemConfig*>* cur = front;

    while (cur != nullptr)
    {
        if (cur->data->getCode() == code)
            return cur->data;       // read category and duration with getCategory() and getDuration()
        cur = cur->next;
    }
    return nullptr;                 // empty list or code not found
}