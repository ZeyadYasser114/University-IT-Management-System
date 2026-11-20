## Track 2: LinkedPriorityQueue.h

**Status:** Complete. Compiles clean (`-Wall -Wextra`) against the real Node, Ticket, Technician and BusyList. `print()` on pointer queues is waiting on `operator<<` for the entity classes.

**What was built**
- Template `LinkedPriorityQueue<T>` as a sorted singly linked list, in one header file (templates must be defined in the header).
- `enqueue` walks to the correct position and links the node in, O(n). `dequeue` and `peek` operate on the head, O(1).
- Ordering is injected through a function pointer (`Comparator` typedef inside the class). It returns true when the first argument must come strictly before the second.
- `head`, `count` and `comp` are `protected` and the destructor is `virtual`, so `BusyList` derives from it and accesses the list directly.
- Standard signatures: `enqueue`, `dequeue(T&)`, `peek(T&) const`, `isEmpty`, `getCount`, `print`, `clear`.
- Three comparators in the same header, using the public getters:
  - `urOrder`: PR descending, then QT ascending, then RID ascending (Waiting_UR).
  - `freeTechOrder`: TID ascending (Free_ST and Free_GT).
  - `completedOrder`: FT ascending, then RID ascending (Completed_Tickets).

**Decisions**
- Header-only. No `using namespace std;` in the header, to avoid name collisions at merge. `std::` is written explicitly.
- Function pointer instead of `std::function`, to avoid any STL restriction. Lambdas that capture nothing and `static` functions still convert to it. Capturing lambdas do not.
- Node field is `data`, matching Omar's `Node.h`.
- Comparators use getters because the `Ticket` and `Technician` fields are private.
- Equal items insert after existing equals. All three comparators resolve ties on unique keys (RID or TID), so ordering never depends on arrival order.
- The queue deletes nodes only. The stored `Ticket*` and `Technician*` are owned and deleted by the caller.
- `print()` prints `*(cur->data)`, so it needs `operator<<` on the pointed-to class. It does not compile on non-pointer types such as `int`.
- `BusyList` breaks FT ties by smaller RID, which is Youssef Ahmed's decision and is already implemented.

**Bugs fixed during development**
- `isEmpty` used `=` instead of `==`.
- `dequeue` assigned `head->item` to `head` instead of `head->next`.
- `peek` was declared as `peak`, and `clear` was declared `const`.
- `Node` was defined twice, and the `template` line was attached to the wrong class.
- Integration: the first pushed version used `item`, `std::function` and direct field access. It did not compile against Omar's `Node.h` or the private fields in `Ticket` and `Technician`.

**Testing**
- Integer queue with a duplicate value: sorted order, count, peek, drain and `isEmpty` all correct.
- Compiled against the real repo files with no warnings.
- Waiting_UR drained in the order 4 2 3 1 5 (PR first, then QT, then RID).
- Completed_Tickets drained in the order 2 3 1 4 (FT first, then RID).
- Free_GT drained in the order 1 2 3 (TID ascending).
- `BusyList` constructs against the new header.

**How to build the queues**
- `LinkedPriorityQueue<Ticket*> waitingUR(urOrder);`
- `LinkedPriorityQueue<Ticket*> completed(completedOrder);`
- `LinkedPriorityQueue<Technician*> freeST(freeTechOrder);` and the same for `freeGT`.

**Open items**
- Youssef Haitham: add `operator<<` for `Ticket` and `Technician`.
- Omar: `NRQueue.h` calls `getID()`, which `Ticket` does not have (`getRID()`), and lacks `#pragma once`.
- Whoever builds the queues: pass the comparators above to the constructors. Waiting_UR, Free_ST, Free_GT and Completed_Tickets are not created anywhere in the repo yet.
- Confirm with the instructor whether STL is banned. The project docx and the Phase 1.2 PDF do not say so, and the Phase 1.1 report in the repo is empty.