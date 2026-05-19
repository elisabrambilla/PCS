#include <iostream> 
#include <concepts>
#pragma once
#include <numeric>
#include <map>
#include <vector>
#include <set>
#include <algorithm>
#include <list>
#include <queue>


// ARCHI

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


// GRAFI

template<typename T>
class undirected_graph {
private:
    std::map<T,std::set<T>> la; // mappa chiave - valore: nodo - set dei vicini (lista di adiacenza)
    //std::vector<undirected_edge<T>> archi; // lista degli archi

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

template<typename T>
class fifo {
    std::list<T> queue;
    
public: 
    // Costruttore di default
    fifo() = default;
    
    // Metodo empty()
    bool empty() const {
        return queue.empty();
    }
    
    // Metodo put()
    void put(const T& nuovo) {
        queue.push_back(nuovo); // inserisce un nuovo elemento in fondo alla coda
    }

    // Metodo get()
    T get() {
        T x = queue.front(); // salvo l'elemento in testa
        queue.pop_front(); // elimino il primo elemento della lista
        return x;
    }

    
};

template<typename T>
class lifo {
    std::list<T> stack;
    
public:
    // Costruttore di default
    lifo() = default;

    // Metodo empty()
    bool empty() const { 
        return stack.empty();
    }

    // Metodo put()
    void put(const T& nuovo) {
        stack.push_front(nuovo); // inserisce un nuovo elemento in cima alla pila
    }

    // Metodo get()
    T get() {
        T x = stack.front(); // salvo l'elemento in testa
        stack.pop_front(); // elimino il primo elemento della lista
        return x;
    }
};

template<typename T>
undirected_graph<T> graph_visit(const undirected_graph<T>& grafo, const T r, auto cont) {
    undirected_graph<T> albero;
    std::map<T,bool> reached; // mappa che a ogni chiave (nodo) associa il valore False, se non ancora visitato, True se visitato
    std::vector<T> nodi = grafo.all_nodes(); // è un vettore con i soli nodi 


    for (const T& k : nodi) {
        reached[k] = false; // Inizializzazione a 0 = False, ovvero nodo non raggiunto
    }

    cont.put(r);
    reached[r] = true;

    while (!cont.empty()) { // se non è vuota, ovvero se ho inserito una radice all'inizio, o se ho ancora elementi nel contenitore dopo
        T u = cont.get();

        for ( const T& w : grafo.neighbours(u)) { // scorro sui vicini di u nel grafo di partenza
            if (! reached[w]) { // equivale a reached[w] == false, w non è ancora stato raggiunto
                reached[w] = true;
                albero.add_edge(undirected_edge<T> (u,w)); // inserisco l'arco nell'albero
                cont.put(w); // aggiungo w nel contenitore
            }
        }
    }
    return albero;
} 


/* Su consiglio di Gemini creo una funzione di supporto all'interno della quale avviene l'effettiva chiamata ricorsiva:
in questo modo dfs_recursive risulta avere i soli due parametri del grafo e del nodo sorgente/radice,
mentre la dfs_recursive_status tiene memoria anche dello stato attuale (alla chiamata) della mappa dei raggiunti e dell'albero fin lì costruito */  

template<typename T>
void dfs_recursive_status(const undirected_graph<T>& grafo, const T u, std::map<T,bool>& reached,undirected_graph<T>& albero) {
    reached[u] = true;
    for (const T& w : grafo.neighbours(u)) {
        if (!reached[w]) {
            albero.add_edge(undirected_edge<T> (u,w));
            dfs_recursive_status(grafo, w, reached, albero);
        }
    }
}


template<typename T>
undirected_graph<T> recursive_dfs(const undirected_graph<T>& grafo, const T r) { 
    undirected_graph<T> albero;
    std::map<T,bool> reached;

    std::vector<T> nodi = grafo.all_nodes();
    for (const T& nodo : nodi) {
        reached[nodo] = false; // Inizializzo reached a tutti non raggiunti
    }

    dfs_recursive_status(grafo , r, reached, albero); 

    return albero;
}

template<typename T>
undirected_graph<T> dijkstra_al(const undirected_graph<T>& grafo, T r) {
    undirected_graph<T> albero;
    std::map<T,T> pred; // predecessori
    std::map<T,int > dist; // distanza da r
    std::vector<T> nodi=grafo.all_nodes(); 
    std::priority_queue<std::pair<int,T>, std::vector<std::pair<int,T>>, std::greater<std::pair<int,T>>> pq; // fatta da elementi coppia (distanza di u da r, u)

    dist[r]=0; // distanza della radice da sè stessa
    pq.push({dist[r],r});

    while (!pq.empty()) {
        auto [dist_u,u] = pq.top();
        pq.pop();

        if (dist_u > dist[u]) {
            continue; // ricomincia dall'elemento successivo nella priority queue, esce solo dall'iterazione, break invece definitivamente esce dal ciclo
        }

        if (u != r) {
            albero.add_edge(undirected_edge<T> (pred[u],u));
        }

        for (const T& w : grafo.neighbours(u)) {
            int peso = 1;
            if (dist.find(w) == dist.end() || dist[u] + peso < dist[w]) {
                dist[w] = dist[u] + 1; // aggiorno/aggiungo la lunghezza del percorso r -> w tramite u nella mappa delle distanze (minime)
                pred[w] = u; // aggiorno/aggiungo u come predecessore di w 
                pq.push({dist[w],w}); // aggiungo w e la sua distanza alla coda con priorità
            }
        }
    }
    return albero;

}

// Dijkstra appositamente per un grafo a pesi = 1 si trasforma in un BFS, che sfrutta la coda e il suo meccanismo di FIFO
template<typename T>
undirected_graph<T> dijkstra_fifo(const undirected_graph<T>& grafo, T r) {
    undirected_graph<T> albero;
    std::map<T,T> pred; // predecessori
    std::map<T,int > dist; // distanza da r
    std::vector<T> nodi=grafo.all_nodes(); 
    fifo<T> q; // coda

    dist[r]=0; // distanza della radice da sè stessa
    q.put(r);

    while (!q.empty()) {
        T u = q.get();

        if (u != r) {
            albero.add_edge(undirected_edge<T> (pred[u],u));
        }
        /*  non è più necessario il controllo sulle distanze perchè a partire dalla radice 
        visiti sempre tutti i suoi vicini (dist = 1) e così via con tutti i nodi (dist = 2, 3 ..), quindi è impossibile trovare un cammino più breve per w */
        for (const T& w : grafo.neighbours(u)) { 
            int peso = 1;
            if (dist.find(w) == dist.end()) {
                dist[w] = dist[u] + 1; // aggiorno/aggiungo la lunghezza del percorso r -> w tramite u nella mappa delle distanze (minime)
                pred[w] = u; // aggiorno/aggiungo u come predecessore di w 
                q.put(w); // aggiungo w alla coda 
            }
        }
    }
    return albero;

}
