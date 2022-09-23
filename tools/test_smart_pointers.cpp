//
// Created by danil on 22.09.2022.
//

#include <iostream>
#include <memory>
#include <functional>

using namespace std;

template<class T>
void println(const T& msg) {
    cout << msg << endl;
}

class Box {
public:
    int x;

    explicit Box(int x) : x(x) {
        cout << "Default created " << this << endl;
    }

    Box(const Box& other) : x(other.x) {
        cout << "Copy created " << this << endl;
    }

    Box(const Box&& other) : x(other.x) {
        cout << "Move created " << this << endl;
    }

    ~Box() {
        cout << "Deleted " << this << endl;
    }
};

shared_ptr<Box> create(const std::function<Box()>&& provider) {
    return make_shared<Box>(Box(1));
}

class A {
public:
    A() {
        cout << "A created " << this << endl;
    }

    A(const A& other) {
        cout << "A copied from " << &other << " to " << this << endl;
    }

    A(const A&& other) noexcept {
        cout << "A moved from " << &other << " to " << this << endl;
    }

    ~A() {
        cout << "A deleted " << this << endl;
    }

    A& setX(int _x) {
        cout << "A set x = " << x << " to " << this << endl;
        this->x = _x;
        return *this;
    }

    int x = 0;
};

int main() {
    shared_ptr<Box>* p1, * p2;
    p1 = new shared_ptr<Box>(new Box(2));
    delete p1;
    println(p1->get()->x);
    println("end 1");

    auto ptr = create([]() {
        return Box(1);
    });
    ptr.reset();

    println("end 2\n");

    println("=== A ===");
    println("=== A1 ===");
    /// A created once
    A& a1 = A().setX(1).setX(2).setX(3);

    println("\n=== A2 ===");
    /// A created twice. Create -> set,set,set -> Copy -> Delete
    A a2 = A().setX(1).setX(2).setX(3);

    println("\n=== A3 ===");
    /// A created twice: 1 on stack, 1 on heap. Heap obj remains, MEM LEAK
    A a3 = (new A())->setX(1).setX(2).setX(3);

    println("\n=== A4 ===");
    /// A created once on heap, can be deleted from a4 address ptr
    A& a4 = (new A())->setX(1).setX(2).setX(3);
    delete &a4;

    println("\n=== Experiments finished, main destructors ===");
}