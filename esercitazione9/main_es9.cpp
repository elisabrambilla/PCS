#include <iostream>
#include <vector>
#include <algorithm>
#include <list>
#include "grafi.hpp"


int main()
{   
    undirected_graph<int> grafo;

    grafo.add_edge(undirected_edge<int>(1,2));
    grafo.add_edge(undirected_edge<int>(1,3));
    grafo.add_edge(undirected_edge<int>(1,4));
    grafo.add_edge(undirected_edge<int>(1,6));
    grafo.add_edge(undirected_edge<int>(2,4));
    grafo.add_edge(undirected_edge<int>(2,5));
    grafo.add_edge(undirected_edge<int>(2,7));
    grafo.add_edge(undirected_edge<int>(3,6));
    grafo.add_edge(undirected_edge<int>(4,6));
    grafo.add_edge(undirected_edge<int>(4,7));
    grafo.add_edge(undirected_edge<int>(5,7));
    grafo.add_edge(undirected_edge<int>(6,7));
    grafo.add_edge(undirected_edge<int>(6,8));
    grafo.add_edge(undirected_edge<int>(7,9));
    grafo.add_edge(undirected_edge<int>(8,9));

    fifo<int> queue;
    lifo<int> stack;

   undirected_graph<int> bfs_albero = graph_visit( grafo, 1, queue); // BFS
   std::vector<undirected_edge<int>> v1 = bfs_albero.all_edges();
   std::cout<< "BFS : archi ";
   for (int i=0; i< v1.size(); i++) {
    std::cout<< v1[i] << "\t";
   }
   std::cout << std::endl;

   undirected_graph<int> dfs_albero = graph_visit( grafo, 1, stack); // DFS
   std::vector<undirected_edge<int>> v2 = dfs_albero.all_edges();
   std::cout<< "DFS : archi ";
   for (int i=0; i< v2.size(); i++) {
    std::cout<< v2[i] << "\t";
   }
   std::cout << std::endl;
   
   undirected_graph<int> rec_dfs = recursive_dfs(grafo, 1); // dfs_ricorsivo
   std::vector<undirected_edge<int>> v3 = rec_dfs.all_edges();
   std::cout<< "DFS ricorsiva : archi ";
   for (int i=0; i< v3.size(); i++) {
    std::cout<< v3[i] << "\t";
   }
   std::cout << std::endl;

   undirected_graph<int> djk = dijkstra_al(grafo, 1); // algoritmo di Dijkstra
   std::vector<undirected_edge<int>> v4 = djk.all_edges();
   std::cout<< "Dijkstra : archi ";
   for (int i=0; i< v4.size(); i++) {
    std::cout<< v4[i] << "\t";
   }
   std::cout << std::endl;
   
}