#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <list>
#include <algorithm> /* serve per stampa set (for_each) */
#include <optional> /* serve per casi in cui le funzioni dichiarate possono aver problemi se chiamate */
#include <fstream>
#include "eigen3/Eigen/Eigen"

/* classe archi */
// template <typename T> fa si che T possa essere di qualsiasi tipo
template<typename T> 
class undirected_edge
{
	T from_;
	T to_;
	
	void ordina()
	{
		if (from_>to_)
		{
			std::swap(from_,to_);
		}
	}
	public:
	/* Costruttore di default */
	undirected_edge()
		: from_(T{1}), to_(T{2})
	{}

	/* Costruttore user-defined */
	undirected_edge(const T& from, const T& to)
		: from_(from), to_(to)
	{
		ordina();
	}
	
	/* Restituiscono i valori di from e to */
	T from() const { return from_; }
	T to() const { return to_; }
	
	/* operatore minore per un arco */
	bool operator<(const undirected_edge& other) const {
		if (from_<other.from() || (from_==other.from() && to_<other.to()))
		{
			return true;
		}
		else {
			return false;
		}
	}
	
	/* operatore uguale per un arco */
	bool operator==(const undirected_edge& other) const {
		if (from_==other.from() && to_==other.to())
		{
			return true;
		}
		else {
			return false;
		}
	}
	
};



/* classe grafo */
template<typename T> 
class undirected_graph
{
	std::map<T,std::set<T>> mappa_adiacenza;	/* mappa di adiacenza, nodo-->set adiacenza*/
	std::vector<undirected_edge<T>> vettore_archi; /* vettore in cui mi salvo gli archi del grafo */
	// abbiamo deciso di usare un vettore ordinato perchè abbiamo supposto che sia molto piu richiesto trovare l'indice di un arco(basso costo computazionale con vettore ordinato), o chiamare la funzione all_edges(), la quale altrimenti avrebbe richiesto di ricrearsi vec_archi ogni volta
	
	public:
	/* Costruttore di default */
	undirected_graph()
		: mappa_adiacenza()
	{}
	
	/* Costruttore user-defined con mappa*/
	// questo è per poter passare una mappa e trattarla come grafo
	undirected_graph(const std::map<T, std::set<T>>& mappa) 
	: mappa_adiacenza(mappa) 
	{
		// appena creo la mappa mi creo anche il vettore di archi
		for (auto it = mappa_adiacenza.begin(); it != mappa_adiacenza.end(); ++it) {
			T from_=(it->first);
			// it è l'iteratore, it-->first mi resituisce chiave(nodo), it-->second il valore(set)
			// con questo check il vettore archi non avrà bisogno di essere ordinato e non conterrà doppioni
			for (const T& x : it->second) {
					if (from_<=x) {
						vettore_archi.push_back(undirected_edge<T>(from_,x));
					}
				}
			}
	}
	
	/* Costruttore di copia se passato un grafo*/
	// la copia è automatizzata grazie all'utilizzo di map e set
	undirected_graph(const undirected_graph& other) 
	: mappa_adiacenza(other.mappa_adiacenza), vettore_archi(other.vettore_archi)
	
	{}
	
	/* restituzione di neighbours */
	// qua non usiamo std::optional perche non è un problema se ci viene restituito un set vuoto nel caso in cui il nodo richiesto non appartenga al grafo
	std::set<T> neighbours(const T& nodo) const {
		auto iter=mappa_adiacenza.find(nodo);
		if (iter!=mappa_adiacenza.end()) {
			return iter->second;
		}
		else {
			return {};
		}
	}
		
	/* aggiunta di arco */
	// è stata permessa l'aggiunta di archi tra nodi non già esistenti nel grafo per comodità: se si aggiunge arco fra nodi non esistenti nel grafo viene automaticamente creato anche il nodo. Questo per fare in modo da poter creare un grafo inizializzandolo e poi aggiungendo gli archi, e i conseguenti nodi alla lista di adiacenza, senza dover prima creare manualmente la mappa di adiacenza con tutti i nodi richiesti 
	undirected_graph& add_edge(const undirected_edge<T>& arco) {
		// faccio questo processo solo se l'arco non esisteva già
		// lower bound ha poca complessità computazionale
		auto pos = std::lower_bound(vettore_archi.begin(), vettore_archi.end(), arco);
		if (pos == vettore_archi.end() || *pos != arco) {
			mappa_adiacenza[arco.from()].insert(arco.to());
			mappa_adiacenza[arco.to()].insert(arco.from());
			// avendo iterato con lower bound ho già la posizione giusta in cui inserire l'arco.
			vettore_archi.insert(pos, arco);
		}
		return *this;
	}
	
	/* restituzione di tutti gli archi */
	const std::vector<undirected_edge<T>>& all_edges() const {
		return vettore_archi; 
	}
	
	/* restituzione di tutti i nodi */ 
	std::vector<T> all_nodes() const {
		std::vector<T> vec_nodi;
		for (auto it = mappa_adiacenza.begin(); it != mappa_adiacenza.end(); ++it) {
			vec_nodi.push_back(it->first);
		}
		return vec_nodi; 
	}
		
	/* restituzione numero arco all'interno di un grafo */
	// uso optional per gestire il caso in cui il nodo richiesto non appartenga al grafo
	std::optional<int> edge_number (const undirected_edge<T>& arco) const {
		// uso lower bound perchè il mio arco è ordinato quindi mi permette meno iterazioni
		auto trovato=std::lower_bound(vettore_archi.begin(), vettore_archi.end(), arco);
		if (trovato!=vettore_archi.end() && *trovato==arco) {
			// il controllo 2 va aggiunto perche lower bound stima dove dovrebbe essere l'arco se facesse parte del grafo, ma nel caso in cui non fa parte del grafo potrebbe puntare ad un altro arco
			int indice = std::distance(vettore_archi.begin(), trovato);
			return indice;
		}
		else {
			return std::nullopt;
		}
	}
	
	/* restituzione oggetto arco */
	std::optional<undirected_edge<T>> edge_at(const size_t pos) const {
		// controllo che la posizione sia minore del numero di archi che ho
		if (pos<vettore_archi.size()) {
			return vettore_archi[pos];
		}
		else {
			return std::nullopt;
		}
	}
	
	/* operatore remove_edge */
	undirected_graph& remove_edge(const undirected_edge<T>& arco) {
	  if (mappa_adiacenza.contains(arco.from()) && mappa_adiacenza.contains(arco.to())) {
		  // faccio questo processo solo se l'arco esisteva già
		  auto trovato=std::lower_bound(vettore_archi.begin(), vettore_archi.end(), arco);
		  if (trovato!=vettore_archi.end() && *trovato==arco) {
			  // lo elimino dal vettore, gli elementi rimangono ordinati
			  vettore_archi.erase(trovato);
			  // lo elimino anche dalla mappa di adiacenza
			  mappa_adiacenza[arco.from()].erase(arco.to());
			  mappa_adiacenza[arco.to()].erase(arco.from());
		  }
	  }
	  return *this;
	}
	
	/* operatore - */ 
	// qua non usiamo optional, anche se i grafi non hanno gli stessi nodi, togliamo comunque gli archi presenti nel grafo che sottraggo
	// il const prima delle graffe impone che this NON venga modificato
	undirected_graph<T> operator-(const undirected_graph& u_g) const {
		undirected_graph nuovo_grafo(*this);
		// elimino dalla copia del grafo gli archi presenti anche nel precedente
		for (const undirected_edge<T>& u_e : u_g.vettore_archi) {
			nuovo_grafo.remove_edge(u_e);
		} 
		return nuovo_grafo;
	}
};


template<typename T>
class fifo
{
	std::list<T> lista_fifo;
	
	public:
	fifo()
	: lista_fifo()
	{}
	
	void put(const T& oggetto) {
		lista_fifo.push_back(oggetto);
	}
	
	// qua non usiamo std::optional perche nella nostra funzione non sara mai possibile usare get in caso di contenitore vuoto perche facciamo un check prima di utilizzarlo
	T get() {
		T x = lista_fifo.front();
		lista_fifo.pop_front();
		return x;
	}
	
	bool empty() const {
		return lista_fifo.empty();
	}
};


template<typename T>
class lifo
{
	std::list<T> lista_lifo;
	
	public:
	lifo()
	: lista_lifo()
	{}
	
	void put(const T& oggetto) {
		lista_lifo.push_front(oggetto);
	}
	
	// qua non usiamo std::optional perche nella nostra funzione non sara mai possibile usare get in caso di contenitore vuoto perche facciamo un check prima di utilizzarlo
	T get() {
		T x = lista_lifo.front();
		lista_lifo.pop_front();
		return x;
	}
	
	bool empty() const {
		return lista_lifo.empty();
	}
};

template<typename T>
undirected_graph<T> graph_visit(const undirected_graph<T>& grafo, const T sorgente, auto contenitore) {
	undirected_graph<T> grafo_risultato;
	// creo il set reached in cui inserisco i nodi già visitati
	std::set<T> reached;
	contenitore.put(sorgente);
	reached.insert(sorgente);
	while (!contenitore.empty()) {
		T u = contenitore.get();
		std::set<T> la = grafo.neighbours(u);
		for (const T& vicino : la) {
			if (!reached.contains(vicino)) {
				contenitore.put(vicino);
				grafo_risultato.add_edge(undirected_edge(u,vicino));
				reached.insert(vicino);
			}
		}
	}
	return grafo_risultato;
};

/* cammino minimo nodo partenza/arrivo con dijkstra_bfs */
template<typename T>
std::vector<undirected_edge<T>> dijkstra_cammino_minimo(const undirected_graph<T>& grafo, T r, T arrivo) {
	std::vector<undirected_edge<T>> cammino_minimo;
	std::map<T,T> pred; // predecessori
	std::map<T,int > dist; // distanza da radice
	fifo<T> q; // coda
	
	dist[r]=0; // distanza della radice da sè stessa
	q.put(r);

	while (!q.empty()) {
		T u = q.get();
		// controllo se sono arrivato al mio punto di arrivo
		if (u == arrivo) {
			T from_=u;
			while (from_!=r) {
				cammino_minimo.push_back(undirected_edge(from_,pred[from_]));
				from_=pred[from_];
			} 
			return cammino_minimo;
		}
		//  non è più necessario il controllo sulle distanze perchè a partire dalla radice visiti sempre tutti i suoi vicini (dist = 1) e così via con tutti i nodi (dist = 2, 3 ..), quindi è impossibile trovare un cammino più breve per w 
		for (const T& w : grafo.neighbours(u)) { 
			if (dist.find(w) == dist.end()) {
				dist[w] = dist[u] + 1; // aggiorno/aggiungo la lunghezza del percorso r -> w tramite u nella mappa delle distanze (minime)
				pred[w] = u; // aggiorno/aggiungo u come predecessore di w 
				q.put(w); // aggiungo w alla coda 
			}
		}
	}
	return cammino_minimo;
}

/*FUNZIONE GRADIENTE CONIUGATO */
struct str_grad_ris {
	Eigen::VectorXd x;
	bool risultato;
	int n_iter;
	Eigen::VectorXd res;
};


// per tolleranza e iterazioni massime se non passate in input gli assegno i valori predefiniti
str_grad_ris gradiente_coniugato(const Eigen::MatrixXd& A,const Eigen::VectorXd& b, Eigen::VectorXd x0=Eigen::VectorXd(),const double& res_tol=1.0e-15, const int& iter_max=10000) {
	if (x0.size() == 0) {
		x0 = Eigen::VectorXd::Zero(A.rows());
	}
	Eigen::VectorXd r0= b-A*x0;
	Eigen::VectorXd p0=r0;
	int k=0;
	Eigen::VectorXd res_0 = r0;
	while (r0.norm()>(res_tol * res_0.norm()) && k<iter_max) {
		double ak=((p0.transpose()*r0)/(p0.transpose()*A*p0)).value();
		x0+=ak*p0;
		r0=b-A*x0;
		double betak=((p0.transpose()*A*r0)/(p0.transpose()*A*p0)).value();
		p0=r0-betak*p0;
		k+=1;
	}
	/* verifica convergenza */
	bool risultato = false;
	if (r0.norm()<=(res_tol * res_0.norm())) {
		risultato = true;
	}
	str_grad_ris grad_ris;
	grad_ris.x = x0;
	grad_ris.risultato = risultato;
	grad_ris.n_iter = k;
	grad_ris.res = r0;
	
	return grad_ris;
}

/* funzione per fare immagine grafo con GRAPHVIZ */

template<typename T>
void grafo_dot(const undirected_graph<T>& grafo, const std::string& filename) {
	std::ofstream os(filename);
	os << "graph G {\n";
	for (const undirected_edge<T>& arco : grafo.all_edges()) {
		os << "  " << arco.from() << " -- " << arco.to() << ";\n";
	} 
	os << "}\n";
}

/* OPERATORI DI STAMPA PER ARCO, GRAFO E SET */

template<typename T>
std::ostream&
operator<<(std::ostream& os, const undirected_edge<T>& u_e)
{
	os << u_e.from() << "--" << u_e.to();
	return os;
}

template<typename T> 
std::ostream&
operator<<(std::ostream& os, const undirected_graph<T>& grafo)
{
	std::vector<T> vec_nodi;
	vec_nodi = grafo.all_nodes();
	os << "Nodi: ";
	for (const T& nodo : vec_nodi) {
		os << nodo << ", ";
	}
	std::vector<undirected_edge<T>> vettore_archi;
	vettore_archi = grafo.all_edges();
	os << "  Archi:  ";
	for (const undirected_edge<T>& u_e : vettore_archi) {
		os << u_e << "; ";
	}
	os << "\n";
	return os;
}

// scrittura operatore stampa per un set, presa da cppreference std::set
template<typename T>
std::ostream& operator<<(std::ostream& out, const std::set<T>& set)
{
    if (set.empty())
        return out << "{}";
    out << "{ " << *set.begin();
    std::for_each(std::next(set.begin()), set.end(), [&out](const T& element)
    {
        out << ", " << element;
    });
    return out << " }";
}