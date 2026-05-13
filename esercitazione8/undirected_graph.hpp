#include <iostream> 
#include <concepts>
#pragma once
#include <numeric>
#include <map>
#include <vector>
#include <set>
#include <algorithm>
#include "undirected_edge.hpp"

template<typename T>
class undirected_graph {
private:
    std::map<T,std::set<T>> la; // mappa chiave - valore: nodo - set dei vicini (lista di adiacenza)
public: 
    // Costruttore di default
    undirected_graph()=default;

    // Costruttore di copia
    undirected_graph(const undirected_graph& other) : la(other.la) {}

    std::set<T> neighbours(const T& nodo) const {
        return la.at(nodo); // at è meglio di [] perchè gestisce l'out of range, [] crea una nuova chiave se non esiste il nodo
    }

    void add_edge(const undirected_edge<T>& nuovo ) {
        la[nuovo.from()].insert(nuovo.to()); // se non esiste la chiave del nodo in questione, [] la crea
        la[nuovo.to()].insert(nuovo.from());
    }

    std::vector<undirected_edge<T>> all_edges() const {
        std::vector<undirected_edge<T>> archi;
        for (const auto& [key,value] : la) { // scorriamo su elementi key(nodo)-value(vicini) della mappa
            for (T vicino : value)  // scorriamo sugli elementi del set di vicini del singolo nodo (key)
            {
                if (key <= vicino) { // questo if mi evita doppioni nel vettore di archi
                    undirected_edge<T> arco(key,vicino);
                    archi.push_back(arco);
                }
            }
        }
        std::sort(archi.begin(),archi.end()); // probabilmente superfluo 
        return archi;
    }

    std::vector<T> all_nodes() const {
        std::vector<T> nodi;
        for (const auto& [key,value] : la) {
            nodi.push_back(key);
        }
        return nodi;
    }

    int edge_number(undirected_edge<T> arco) const {
        std::vector<undirected_edge<T>> archi = all_edges();
        int n = 0;
        for (int k = 0; k < archi.size(); k++) {
            if (archi[k]==arco) {
                n = k+1; // numerazione degli archi da 1 in avanti
            }
        }
        return n;
    }

    undirected_edge<T> edge_at(int n) {
        if ( n==0 ) {return undirected_edge<int> (0,0); }
        else {
            std::vector<undirected_edge<T>> archi = all_edges();
            if (n > archi.size()) {
                return undirected_edge<int> (0,0);
            }
            else {
                return archi[n-1];
            }
        }
        
    }

    auto operator-(const undirected_graph& other) const {
        std::vector<undirected_edge<T>> archi1 = all_edges();
        std::sort(archi1.begin(),archi1.end());
        std::vector<undirected_edge<T>> archi2 = other.all_edges();
        std::sort(archi2.begin(),archi2.end());

        std::vector<undirected_edge<T>> diff;

        std::set_difference(archi1.begin(), archi1.end(), archi2.begin(), archi2.end(), std::inserter(diff, diff.begin())); // inizio e fine dei vector da confrontare, dove mettere l'output)

        return diff;

    }

    

};

