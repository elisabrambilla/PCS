#include <iostream>
#include <vector>
#include <algorithm>
#include "undirected_edge.hpp"
#include "undirected_graph.hpp"



// Testiamo la classe con un grafo composto da 4 nodi e 3 archi

int main()
{   
    undirected_graph<int> g1;

    g1.add_edge(undirected_edge<int>(1,2));
    g1.add_edge(undirected_edge<int>(1,3));
    g1.add_edge(undirected_edge<int>(3,4)); 
    // std::map<int,std::set<int>> m{{1,{2,3}},{2,{1}},{3,{1,4}},{4,{3}}};

    undirected_graph<int> g_copia(g1); // costruttore di copia

    undirected_graph<int> g2;

    g2.add_edge(undirected_edge<int>(1,2));
    g2.add_edge(undirected_edge<int>(1,3));
    g2.add_edge(undirected_edge<int>(1,4)); 



    // Tutti i nodi
    std::vector<int> nodi = g1.all_nodes();

    std::cout<< "Tutti i nodi: " ;
    for (int i=0; i<nodi.size(); i++) {
        std::cout << nodi[i] << "\t";
    }
    std::cout << std::endl;

    // Tutti gli archi
    auto archi = g1.all_edges();
    std:: cout << "Tutti gli archi: ";
    for (const auto& e : archi) { // scorro sugli archi nel vettore di archi
        std::cout << e.from() << " - " << e.to() << "\t";
    }
    std::cout << std::endl;

    // Vicini del nodo x
    for (int n : nodi) {
        std::cout << "I vicini di " << n << " sono : " ;
        for (int x : g1.neighbours(n)) {
            std::cout << x << " ";
        }
        std::cout<< "\n";
    }

    // Numerazione dell'arco (1,3)
    std::cout << "Arco (1,3) è in posizione: " << g1.edge_number(undirected_edge<int> (1,3)) << std::endl;

    // Arco in posizione 2
    std::cout << "Arco in posizione 2 è: " << g1.edge_at(2) << std::endl;
    // Arco in posizione 0
    std::cout << "Arco in posizione 0 è (errore): " << g1.edge_at(0) << std::endl;

    // Sottrazione insiemistica tra grafi
    auto differenza = g1-g2;
    
    std::cout << "g1 - g2 = ";
    for (auto k : differenza) {
        std::cout << k << " ";
    }
    std::cout << std::endl;

}