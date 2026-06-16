#pragma once
#include "grafi.hpp"
#include <numeric> //per std::acumulate
#include "eigen3/Eigen/Eigen"

/* FUNZIONE LETTURA INPUT */
template<typename T>
struct str_input {
	undirected_graph<T> grafo;
	std::map<undirected_edge<T>, std::pair<int, double>> mappa_res;
	std::map<undirected_edge<T>, std::pair<int, double>> mappa_gen;
	std::vector<std::string> nomi_res;
	bool input_corretto;
};

template<typename T>
str_input<T> lettura(const std::string& nome_file) {
		str_input<T> dati;
		std::ifstream ifs(nome_file);
		if (ifs.is_open()){
			std::string tipo;
			double valore;
			T nodo_from;
			T nodo_to;
			int num_res = 0;
			int num_gen = 0;
			// con questa struttura si evitano problemi di righe vuote o di spazi eccessivi, esce dal ciclo solo nel caso in cui si arrivi a fine file o nel caso in cui i dati non siano quelli richiesti
			while (ifs >> tipo >> valore >> nodo_from >> nodo_to) {
				auto found_res = dati.mappa_res.find(undirected_edge(nodo_from,nodo_to));
				auto found_gen = dati.mappa_gen.find(undirected_edge(nodo_from,nodo_to));
				// controllo che l'arco non fosse già stato occupato in precedenza e che l'arco non sia un self-loop
				if (found_res==dati.mappa_res.end() && found_gen==dati.mappa_gen.end() && nodo_from!=nodo_to) {
					if (tipo[0]=='R') {
						dati.mappa_res[undirected_edge(nodo_from,nodo_to)]={num_res, valore};
						dati.nomi_res.push_back(tipo);
						num_res+=1;
					}
					else {
						// per i generatori è fondamentale la direzione, siccome undirected_edge ordina sempre da piu basso a piu alto, nel caso in cui il generatore sia ordinato allo stesso modo ci salviamo nella mappa il suo opposto, questo perchè abbiamo che il + del generatore è collegato al nodo_from
						if (nodo_to<nodo_from) {
							dati.mappa_gen[undirected_edge(nodo_from,nodo_to)]={num_gen,valore};
						}
						else {
							dati.mappa_gen[undirected_edge(nodo_from,nodo_to)]={num_gen,-valore};
						}
					num_gen+=1;
					}
					dati.grafo.add_edge(undirected_edge(nodo_from,nodo_to));					
				}
				else {
					if (nodo_from==nodo_to) {
						std::cout << "L'arco: " << undirected_edge(nodo_from,nodo_to) << " è un self-loop e non viene accettato, questa riga viene saltata.\n";
					}
					else {
						std::cout << "Nell'arco: " << undirected_edge(nodo_from,nodo_to) << " era già stato inserito un valore, questa riga viene saltata, gli archi non possono essere sovrascritti ne possono avere piu di un singolo elemento associato.\n";
					}
				}
			}
			// controllo che tutti i nodi abbiano almeno 2 vicini
			dati.input_corretto = true;
			for (const T& nodo : dati.grafo.all_nodes()) {
				if (dati.grafo.neighbours(nodo).size()<2) {
					dati.input_corretto=false;
					std::cout << "L'input di testo non è accettabile perchè non tutti i nodi hanno almeno 2 vicini\n";
					return dati;
				}
			}
			return dati;
		}
		else {
			std::cout << "Il file non si è aperto correttamente \n";
			dati.input_corretto = false;
			return dati;
		}
}

template <typename T>
struct str_archi_percorso {
	std::list<undirected_edge<T>> lista_archi;
	std::map<T,T> mappa_predecessori;
};

/* ALGORITMI CICLI */
/* ALGORITMO CICLI (non necessariamente minimi)*/
template<typename T>
bool recursive_find_path(const undirected_graph<T>& grafo,const T& u,const T& v, std::list<T>& lista_cammino, std::set<T>& reached) {
	reached.insert(u);
	lista_cammino.push_back(u);
	if (u==v) {
		return true;
	}
	std::set<T> la=grafo.neighbours(u);
	for (const T& n : la) {
		if (!reached.contains(n)) {
			if (recursive_find_path(grafo, n, v, lista_cammino, reached)) {
				return true;
			}
		}
	}
	lista_cammino.pop_back();
	return false;
}

/* algoritmo find_path tra u e v */
template <typename T> 
std::list<T> find_path(const undirected_graph<T>& grafo,const T& u,const T& v) {
	std::list<T> lista_cammino;
	std::set<T> reached;
	// qua ignoriamo il valore di ritorno, perche serve soltanto all'interno della ricorsione
	recursive_find_path(grafo, u, v, lista_cammino, reached);
	return lista_cammino;
}

template <typename T>
std::list<str_archi_percorso<T>> trova_cicli(const undirected_graph<T>& grafo) {
	std::list<std::list<T>> lista_cicli;
	// prima calcoliamo grafo T con dfs e poi il coalbero C con sottrazione
	lifo<T> cont_lifo;
	// come sorgente per il dfs prendiamo semplicemente il primo nodo che ci torna il metodo all_nodes
	undirected_graph<T> grafo_T=graph_visit(grafo, grafo.all_nodes().front(), cont_lifo);
	undirected_graph<T> grafo_C=grafo-grafo_T;
	// calcolo i cicli per ogni arco di C
	for (const undirected_edge<T>& u_e : grafo_C.all_edges()) {
		std::list<T> lista_cammino=find_path(grafo_T, u_e.from(), u_e.to());
		lista_cicli.push_back(lista_cammino);
	}
	/* costruzione output */
	// vogliamo una lista contentente i cicli, dove i cicli(str_archi_percorso) sono composti da lista archi e mappa predecessori
	std::list<str_archi_percorso<T>> lista_archi_percorso;
	for (std::list<T>& ciclo : lista_cicli) {
		str_archi_percorso<T> archi_percorso;
		T primo_abs=ciclo.front();
		T from_=primo_abs;
		ciclo.pop_front();
		while (!ciclo.empty()) {
			T to_=ciclo.front();
			ciclo.pop_front();
			archi_percorso.lista_archi.push_back(undirected_edge(from_,to_));
			archi_percorso.mappa_predecessori[to_]=from_;
			from_=to_;
		}
		// aggiungiamo l'arco che chiude il ciclo
		archi_percorso.lista_archi.push_back(undirected_edge(primo_abs,from_));
		archi_percorso.mappa_predecessori[primo_abs]=from_;
		lista_archi_percorso.push_back(archi_percorso);
	}
	return lista_archi_percorso;
}

/* ALGORITMO DE PINA CICLI MINIMI*/

template <typename T>
std::vector<int> cicli_Ci(const undirected_graph<T>& grafo, std::vector<int>& Si) {
	// inizializziamo il grafo G1 con gli archi richiesti(la duplicazione dei nodi è considerata come inserimento di nodi opposti a quelli presenti)
	undirected_graph<T> G1;
	int pos=0;
	for (const undirected_edge<T>& u_e : grafo.all_edges()) {
		if (Si[pos]==1) {
			G1.add_edge(undirected_edge(u_e.from(),-u_e.to()));
			G1.add_edge(undirected_edge(-u_e.from(),u_e.to()));
		}
		else {
			G1.add_edge(undirected_edge(u_e.from(),u_e.to()));
			G1.add_edge(undirected_edge(-u_e.from(),-u_e.to()));
		}
		pos+=1;
	}
	std::vector<int> Ci(grafo.all_edges().size(),1);
	// ci creiamo la copia dei nodi per poter ciclare su ognuno di essi e controllare allo stesso tempo se abbiamo gia trovato un ciclo minimo
	std::vector<T> vec_nodi = grafo.all_nodes();
	size_t i = 0;
	// cicliamo su tutti i nodi
	// il controllo su Ci è fatto perchè se ho trovato un ciclo con 3 archi questo sarà necessariamente un ciclo minimo quindi posso uscire dal while
	while (i<vec_nodi.size() && std::accumulate(Ci.begin(), Ci.end(), 0)!=3) {
		T v = vec_nodi[i];
		/* calcolo cammino minimo tra v e -v */
		std::vector<undirected_edge<T>> cammino=dijkstra_cammino_minimo(G1,v,-v);
		std::vector<int> Cu(grafo.all_edges().size(),0);
		// calcoliamo Cu
		for (const undirected_edge<T>& arco : cammino) {
			//dobbiamo considerare che con i nodi negativi gli archi possono essere -3<-->-2 e quindi dobbiamo usare l'abs+undirected_edge che ci restituisce l'arco ordinato
			undirected_edge<T> arco_abs = undirected_edge(std::abs(arco.from()), std::abs(arco.to()));
			int j = *(grafo.edge_number(arco_abs));
			Cu[j]+=1;
		}
		for (int& val : Cu) {
			val = val % 2;
		} 
		// aggiorniamo Ci solo se ha meno "1" del precedente best
		if (std::accumulate(Cu.begin(), Cu.end(), 0)<std::accumulate(Ci.begin(), Ci.end(), 0) && std::accumulate(Cu.begin(), Cu.end(), 0)!=0) {
			Ci=Cu;
		}
		i++;
	}
	return Ci;
}

/* ALGORITMO DE PINA */
template <typename T>
std::vector<std::vector<int>> dePina(const undirected_graph<T>& grafo, std::vector<std::vector<int>>& S, const size_t& k) {
	// creiamo matr_C che contiene tutti i cicli minimi (k cicli), ogni ciclo vettore di dimensione #archi tot e 0/1 se arco fa parte del ciclo o meno
	std::vector<std::vector<int>> matr_C(k,std::vector<int>(S[0].size(),0));
	for (size_t i=0; i<k; i++) {
		std::vector<int> C;
		C=cicli_Ci(grafo,S[i]);
		for (size_t j=i+1; j<k; j++) {
			int somma = 0;
			for (size_t pos=0; pos<C.size(); pos++) {
				somma+=C[pos]*S[j][pos];
			}
			if ((somma%2)==1) {
				for (size_t pos=0; pos<C.size(); pos++) {
					S[j][pos]=S[j][pos] xor S[i][pos];
				}
			}
		}
		matr_C[i]=C;
	}
	return matr_C;
}

template <typename T>
std::list<str_archi_percorso<T>> cicli_minimi(const undirected_graph<T>& grafo) {
	// prima calcoliamo grafo T con dfs e poi il coalbero C con sottrazione
	lifo<T> cont_lifo;
	// come sorgente per il dfs prendiamo semplicemente il primo nodo che ci torna il metodo all_nodes
	undirected_graph<T> grafo_T=graph_visit(grafo, grafo.all_nodes().front(), cont_lifo);
	undirected_graph<T> grafo_C=grafo-grafo_T;
	size_t k =  grafo_C.all_edges().size();
	size_t m = grafo.all_edges().size();
	// creiamo la base S
	std::vector<std::vector<int>> S(k,std::vector<int>(m,0));
	// assegniamo un singolo 1 in ogni componente di Si nella posizione dell'arco in G, se l'arco è presente anche in C
	int pos=0;
	for (const undirected_edge<T>& e : grafo_C.all_edges()) {
		S[pos][*grafo.edge_number(e)]=1;
		pos+=1;
	}
	std::vector<std::vector<int>> matr_C = dePina(grafo, S, k);
	/* costruzione output */
	// ci costruiamo la lista degli archi a partire dalla matrice C
	std::list<str_archi_percorso<T>> lista_archi_percorso;
	for (const std::vector<int>& C : matr_C) {
		str_archi_percorso<T> archi_percorso;
		// inizializziamo a 0 tanto tutti i nodi sono positivi
		for (size_t i=0; i<C.size(); i++) {
			if (C[i]==1) {
				// aggiungiamo l'arco "minore" (quindi lista_archi sarà ordinata) a lista archi
				archi_percorso.lista_archi.push_back(*grafo.edge_at(i));
			}
		}
		lista_archi_percorso.push_back(archi_percorso);
	}
	// usando la lista degli archi ci costruiamo l'ordine di percorrenza del ciclo(mappa_predecessori), che abbiamo deciso partire dal nodo minore verso il suo vicino più piccolo.
	for (str_archi_percorso<T>& archi_percorso : lista_archi_percorso) {
		// partiamo per tutti i cicli trovati dal nodo minore del ciclo.
		undirected_edge<T> primo_arco = archi_percorso.lista_archi.front();
		archi_percorso.mappa_predecessori[primo_arco.to()]=primo_arco.from();
		T attuale = primo_arco.to();
		T prec = primo_arco.from();
		for (size_t passo=0; passo<archi_percorso.lista_archi.size(); passo++) {
			for (const undirected_edge<T>& arco : archi_percorso.lista_archi) {
				// vogliamo partire dal nostro attuale ma non arrivare al suo predecessore
				// separiamo due if per sapere se si tratta di to o di from
				if (arco.from()==attuale && arco.to()!=prec) {
					archi_percorso.mappa_predecessori[arco.to()]=attuale;
					prec=arco.from();
					attuale=arco.to();
				}
				if (arco.to()==attuale && arco.from()!=prec) {
					archi_percorso.mappa_predecessori[arco.from()]=attuale;
					prec=arco.to();
					attuale=arco.from();
				}
			}
		}
	}
	return lista_archi_percorso;
}

struct str_B_V {
	Eigen::MatrixXd B;
	Eigen::VectorXd V;
};

/* COSTRUZIONE MATRICE B E VETTORE V */
// notiamo che per la costruzione il costo computazionale si riduce se cicliamo una sola volta per la costruzione di entrambe, questo perchè in ogni arco esiste uno e un solo elemento tra resistenza e generatore
// B ha dimensione #res,#cicli, V ha dimensione #cicli
template <typename T>
str_B_V B_V(const std::list<str_archi_percorso<T>>& lista_archi_percorso, const std::map<undirected_edge<T>, std::pair<int, double>>& mappa_res, const std::map<undirected_edge<T>, std::pair<int, double>>& mappa_gen){
	str_B_V B_V;
	B_V.B = Eigen::MatrixXd::Zero(mappa_res.size(), lista_archi_percorso.size());
	B_V.V = Eigen::VectorXd::Zero(lista_archi_percorso.size());
	int colonna_ciclo = 0;
	for (const str_archi_percorso<T>& archi_percorso : lista_archi_percorso) {
		for (const undirected_edge<T>& arco : archi_percorso.lista_archi) {
			// controllo se l'arco è un resistenza o un generatore
			auto found=mappa_res.find(arco);
			// se arco è resistenza allora aggiorno B
			if (found!=mappa_res.end()) {
				// controlliamo se la direzione dell'arco è la stessa della direzione di percorrenza del ciclo
				if (arco.from()==archi_percorso.mappa_predecessori.at(arco.to())) {
					B_V.B(mappa_res.at(arco).first,colonna_ciclo)=1;
				}
				else {
					B_V.B(mappa_res.at(arco).first,colonna_ciclo)=-1;
				}
			}
			// se non è resistenza è necessariamente generatore e aggiorno V
			else {
				// controlliamo se la direzione dell'arco è la stessa della direzione di percorrenza del ciclo
				// il generatore è gia stato orientato correttamente durante la lettura
				if (arco.from()==archi_percorso.mappa_predecessori.at(arco.to())) {
					B_V.V(colonna_ciclo)+=mappa_gen.at(arco).second;
				}
				else {
					B_V.V(colonna_ciclo)-=mappa_gen.at(arco).second;
				}
			}
		}
		colonna_ciclo+=1;
	}
	return B_V;
}


