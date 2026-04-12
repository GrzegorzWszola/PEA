#pragma once

#include <algorithm>

template<typename T>
class IDataStructure {
public:
    virtual void push(const T& item) = 0;
    virtual T pop() = 0;
    virtual bool empty() const = 0;
    virtual int getSize() const = 0;
    virtual ~IDataStructure() {}
};

// Do algorytmu DFS
template<typename T>
struct Stack : public IDataStructure<T> {
    T* data;
    int top;
    int capacity;

    Stack(int cap = 16) : capacity(cap), top(-1) { data = new T[cap]; }
    ~Stack() { delete[] data; }

    void push(const T& x) override {
        if (top + 1 == capacity) resize();
        data[++top] = x;
    }

    T pop() override {
        T item = data[top];
        data[top] = T();
        top--;
        return item;
    }

    bool empty() const override { return top == -1; }

    int getSize() const override { 
        return top + 1; 
    }

    private:
        void resize() {
            capacity *= 2;
            T* newData = new T[capacity];
            for (int i = 0; i <= top; i++)
                newData[i] = data[i];
            delete[] data;
            data = newData;
        }
};

template<typename T>
struct Queue : public IDataStructure<T> {
    T* data;
    int front, back, size, capacity;

    Queue(int cap = 16) : capacity(cap), front(0), back(0), size(0) {
        data = new T[cap];
    }
    ~Queue() { delete[] data; }

    void push(const T& n) override {
        if (size == capacity) resize();
        data[back] = n;
        back = (back + 1) % capacity;
        size++;
    }

    T pop() override {
        T n = data[front];
        data[front] = T();
        front = (front + 1) % capacity;
        size--;
        return n;
    }

    bool empty() const override { return size == 0; }

    int getSize() const override { 
        return this->size; 
    }

    private:
        void resize() {
            capacity *= 2;
            T* newData = new T[capacity];
            for (int i = 0; i < size; i++)
                newData[i] = data[(front + i) % (capacity / 2)];
            front = 0;
            back = size;
            delete[] data;
            data = newData;
        }
};

// Do algorytmu Best-First
template<typename T>
struct PriorityQueue : public IDataStructure<T>{
    T* data;
    int size;
    int capacity;

    PriorityQueue(int cap = 16) : capacity(cap), size(0) {
        data = new T[cap];
    }
    ~PriorityQueue() { delete[] data; }

    void push(const T& n) override {
        if (size == capacity) resize();
        data[size++] = n;
        // przesiewanie w górę
        int i = size - 1;
        while (i > 0) {
            int parent = (i - 1) / 2;
            if (data[parent].lb > data[i].lb) {
                std::swap(data[parent], data[i]);
                i = parent;
            } else break;
        }
    }

    T pop() override {
        T topItem = data[0];
        data[0] = data[size - 1]; // Przenosimy ostatni na szczyt
        data[size - 1] = T();     // Czyścimy stare miejsce ostatniego elementu
        size--;

        // Przesiewanie w dół (heapify down)
        int i = 0;
        while (true) {
            int left  = 2 * i + 1;
            int right = 2 * i + 2;
            int smallest = i;
            if (left  < size && data[left].lb  < data[smallest].lb) smallest = left;
            if (right < size && data[right].lb < data[smallest].lb) smallest = right;
            if (smallest == i) break;
            std::swap(data[smallest], data[i]);
            i = smallest;
        }
        return topItem;
    }

    bool empty() const override { return size == 0; }

    int getSize() const override { 
        return this->size;
    }

    private:
        void resize() {
            capacity *= 2;
            T* newData = new T[capacity];
            for (int i = 0; i < size; i++)
                newData[i] = data[i];
            delete[] data;
            data = newData;
        }
};
