#include <iostream>
#include "sort.hpp"
#include "randfiller.h"

int main() 
{
    bool sorted = true;
    randfiller rf;
    const int n = 100; // 100 dimensioni tra 1 e 1000 diverse, per ogni dimensione 100 vettori, elementi tra -10000 e 10000
    
    std::vector<int> vec; // vec è il vettore contenente le 100 dimensioni diverse
    vec.resize(n); // dim
    rf.fill(vec,1,1000); // ogni elemento è una dimensione tra 1 e 1000
        
    for (int i=0; i<vec.size(); i++) { // per ogni dimensione elemento di vec
        int dim = vec[i]; 
        for (int j=0; j<100; j++) { // creo 100 vettori di dimensione dim
            std::vector<int> vi;
            vi.resize(dim);
            rf.fill(vi,-10000,10000);
            bubble_sort(vi); // riordino il vettore j-esimo
            if (not is_sorted(vi)) {
                sorted=false;
            }
            
        }

    }

    // ordinamento si una stringa
    std::vector<std::string> v_string = {"armadio","letto","comodino","tavolo","sedia","scrivania","poltrona","libreria","scaffale","tappeto"};
    bubble_sort(v_string);
    if (not is_sorted(v_string)) {
        sorted=false;
    } 


    // verifica finale
    if (sorted) {
        //std::cout<<"Ha funzionato\n";
        return EXIT_SUCCESS;
    }
    else {return EXIT_FAILURE;}
}