#include <iostream> 
#include <concepts>
#pragma once
#include <numeric>

template<typename T>
class undirected_edge{
    T a; // nodo di partenza
    T b; // nodo di arrivo

void ordina() {
    if (b < a) {
        std::swap(a,b);
    }
}
public:
    // costruttore di default
    undirected_edge()
        :a(T{0}), b(T{1})
        {}
    // costruttore user-defined
    undirected_edge(const T& nodo1, const T& nodo2)
        :a(nodo1),b(nodo2) {
            ordina();
        }
  
    T from() const {return a;}
    T to() const {return b;}

    bool operator<(const undirected_edge& other) {
        if (a < other.a) {
            return true;
        }
        else if (a == other.a && b < other.b ) {
            return true;
        }
        else {return false; }
    }

    bool operator==(const undirected_edge& other) {
        if (a == other.a && b == other.b) {
            return true;
        }
        else {return false; }
    }


};

template <typename T>
std::ostream&
operator<<(std::ostream& os, const undirected_edge<T>& arco)
{
    os << arco.from() << " -- " << arco.to();
    return os;
}

