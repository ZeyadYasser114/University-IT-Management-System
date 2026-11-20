## Track 2: LinkedPriorityQueue.h

**Status:** Core queue done and tested with integers. Comparators written.

**What was built**
- Template `LinkedPriorityQueue<T>` as a sorted singly linked list.
- `enqueue` walks to the correct position, O(n). `dequeue` and `peek` operate on the head, O(1).
- Ordering is injected through a `std::function` comparator. It returns true when the first argument must come strictly before the second.
- `head`, `count` and `comp` are `protected` and the destructor is `virtual`, so `BusyList` can derive from it.
- Three comparators in the same header: `urOrder` (PR descending, QT ascending, RID ascending), `freeTechOrder` (TID ascending), `completedOrder` (FT ascending, RID ascending).

**Decisions**
- Header-only, because it is a template.
- No `using namespace std;` in the header, to avoid name collisions at merge. `std::` is written explicitly.
- Equal items insert after existing equals. All three comparators resolve ties on unique keys, so ordering never depends on arrival order.
- The queue deletes nodes only. The stored `Ticket*` and `Technician*` are owned and deleted by the caller.

**Bugs fixed during development**
- `isEmpty` used `=` instead of `==`.
- `dequeue` assigned `head->item` to `head` instead of `head->next`.
- `peek` was declared as `peak`, and `clear` was declared `const`.
- `Node` was defined twice, and the `template` line was attached to the wrong class.

**Testing**
- Integer queue with a duplicate value: sorted order, count, peek, drain and `isEmpty` all correct.
- Comparator test with tickets and technicians: record the result here after running it.

**Open items**
- Field names `rid`, `qt`, `pr`, `ft`, `tid` to be confirmed with Youssef Haitham.
- `Node<T>` interface to be confirmed with Omar.
- `BusyList` tiebreak to be decided by Youssef Ahmed.
- Before merge: delete the temporary `Node.h`, `Ticket.h`, `Technician.h` and `test.cpp`, then compile against the real files.