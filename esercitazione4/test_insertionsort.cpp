#include <iostream>
#include "sort.hpp"
#include "randfiller.h"

int main() 
{
    bool sorted = true;
    randfiller rf;
    const int n = 10; // 10 dimensioni tra 1 e 1000 diverse, per ogni dimensione 100 vettori, elementi tra -10000 e 10000
    
    std::vector<int> vi; // vi è il vettore contenente le 100 dimensioni diverse
    vi.resize(n); // dim
    rf.fill(vi,1,1000); // ogni elemento è una dimensione tra 1 e 1000
        
    for (int i=0; i<vi.size(); i++) { // per ogni dimensione elemento di vi
        int dim = vi[i]; 
        for (int j=0; j<100; j++) { // creo 100 vettori di dimensione dim
            std::vector<int> vec;
            vec.resize(dim);
            rf.fill(vec,-10000,10000);
            insertion_sort(vec); // riordino il vettore j-esimo
            if (not is_sorted(vec)) {
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

    if (sorted) {
        return EXIT_SUCCESS;
    }
    else {return EXIT_FAILURE;}
}