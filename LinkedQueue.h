template <typename T>
class LinkedQueue {
private:
    Node<T>* front = nullptr;
    Node<T>* rear = nullptr;
    int count = 0;

public:
    bool isEmpty() const {
        return front == nullptr;
    }

    int getCount() const {
        return count;
    }

    void enqueue(T x) {
        Node<T>* n = new Node<T>(x, nullptr);
        if (isEmpty()) {
            front = rear = n;
        }
        else {
            rear->next = n;
            rear = n;
        }
        count++;
    }

    bool dequeue(T& outValue) {
        if (isEmpty()) return false;
        Node<T>* old = front;
        outValue = old->data;
        front = front->next;
        if (isEmpty()) rear = nullptr;
        delete old;
        count--;
        return true;
    }

    bool peek(T& outValue) const {
        if (isEmpty()) return false;
        outValue = front->data;
        return true;
    }

    void clear() {
        T dummy;
        while (!isEmpty()) {
            dequeue(dummy);
        }
    }

    ~LinkedQueue() {
        clear();
    }
};
