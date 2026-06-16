#include<iostream>
#include<fstream>
#include<string>
#include<map>
#include <iomanip>
#include "funzioni_progetto.hpp"



int main() {
	std::string nome_file="netlist.txt";
	str_input<int> dati = lettura<int>(nome_file);
	if (dati.input_corretto==false) {
		std::cout << "Programma abortito\n";
		return 1;
	}
	
	std::list<str_archi_percorso<int>> lista_archi_percorso_c = trova_cicli(dati.grafo);

	std::list<str_archi_percorso<int>> lista_archi_percorso_cm = cicli_minimi(dati.grafo);

	
	// matrice R mxm con m il num di resistenze
	Eigen::MatrixXd R = Eigen::MatrixXd::Zero(dati.mappa_res.size(), dati.mappa_res.size());
	for (const auto& [key,value] : dati.mappa_res) {
		R(value.first,value.first) = value.second;
	}

	str_B_V B_V_c = B_V(lista_archi_percorso_c, dati.mappa_res, dati.mappa_gen);


	str_B_V B_V_cm = B_V(lista_archi_percorso_cm, dati.mappa_res, dati.mappa_gen);

	
	/* risoluzione per cicli minimi */ 
	std::cout << "\nRISULTATI CICLI MINIMI:\n";
	Eigen::MatrixXd matr_coeff_cm = B_V_cm.B.transpose()*R*B_V_cm.B;
	str_grad_ris grad_ris_cm = gradiente_coniugato(matr_coeff_cm, B_V_cm.V);
	if (grad_ris_cm.risultato) {
		std::cout << "Il gradiente coniugato converge in " << grad_ris_cm.n_iter << " iterazioni\n";
		Eigen::VectorXd int_cm = B_V_cm.B*grad_ris_cm.x;
		Eigen::VectorXd tens_cm = R*int_cm;
		for (Eigen::Index i=0; i<int_cm.size(); i++) {
			std::cout << dati.nomi_res[i] << ": " << "V = " << std::fixed << std::setprecision(2) << std::setw(6) << tens_cm(i) << " volts, I = " << std::setw(5) << int_cm(i) << " amps.\n";
		}
	}
	
	/* risoluzione per cicli casuali*/ 
	std::cout << "\nRISULTATI CICLI:\n";
	Eigen::MatrixXd matr_coeff_c = B_V_c.B.transpose()*R*B_V_c.B;
	str_grad_ris grad_ris_c = gradiente_coniugato(matr_coeff_c, B_V_c.V);
	if (grad_ris_c.risultato) {
		std::cout << "Il gradiente coniugato converge in " << grad_ris_c.n_iter << " iterazioni\n";
		Eigen::VectorXd int_c = B_V_c.B*grad_ris_c.x;
		Eigen::VectorXd tens_c = R*int_c;
		for (Eigen::Index i=0; i<int_c.size(); i++) {
			std::cout << dati.nomi_res[i] << ": " << "V = " << std::fixed << std::setprecision(2) << std::setw(6) << tens_c(i) << " volts, I = " << std::setw(5) << int_c(i) << " amps.\n";
		}
	}
	else {
		std::cout << "Il metodo del gradiente non ha raggiuno convergenza.";
	}
	return 0;
}


// Appendice: parti di stampa utili durante l'implementazione del codice:
	/* stampa grafo rappresentante il circuito */
	//std::cout << dati.grafo;
	//str_res_dijkstra<int> res_dijkstra = dijkstra(dati.grafo, 1);
	//		for (const auto [key, value] : res_dijkstra.mappa_predecessori) {
	//		std::cout << key << " " << value << "\n";
	//	}
	
	/* stampa dei prdecessori e archi nei cicli */
	//for (const str_archi_percorso<int>& archi_percorso : lista_archi_percorso_c) {
	//	std::cout << "CICLO:\n";
	//	for (const auto [key, value] : archi_percorso.mappa_predecessori) {
	//		std::cout << key << " predecessore: " << value << "\n";
	//	}		for (const undirected_edge<int>& arco : archi_percorso.lista_archi) {
	//		std::cout << arco << " ";
	//	}
	//	std::cout << "\n\n";
	//}
	
	/* stampa dei prdecessori e archi nei cicli minimi */
	//for (const str_archi_percorso<int>& archi_percorso : lista_archi_percorso_cm) {
	//	std::cout << "CICLO MINIMO:\n";
	//	for (const auto [key, value] : archi_percorso.mappa_predecessori) {
	//		std::cout << key << " predecessore: " << value << "\n";
	//	}
	//	for (const undirected_edge<int>& arco : archi_percorso.lista_archi) {
	//		std::cout << arco << " ";
	//	}
	//	std::cout << "\n\n";
	//}
	
	//std::cout << "Matrice R:\n" << R << "\n";
	
	//std::cout << "matrice B per cicli casuali V\n" << B_V_c.B << "\n"; 
	//std::cout << "vettore V per cicli casuali\n" << B_V_c.V << "\n";
	
	//std::cout << "matrice B per cicli minimi\n" << B_V_cm.B << "\n"; 
	//std::cout << "vettore V per cicli minimi\n" << B_V_cm.V << "\n";