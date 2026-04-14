#include <iostream>
#include <vector>
#include "sort.hpp"
#include "timecounter.h"
#include "randfiller.h"
#include <algorithm>

int main() 
{
    randfiller rf;
    std::cout << "Tempi di ordinamento di vettori di dimensione: ";
    std::vector<std::vector<int>> vec;
    for (int i = 2; i <= 13; i++){ // 2^i è la dimensione dell'i-esimo vettore vi di vec
        std::vector<int> vi(pow(2,i));
        rf.fill(vi,-10000,10000);
        std::cout << vi.size() << "\t";
        vec.push_back(vi); // ci inserisce al fondo il nostro nuovo vettore
    }
    
    std::cout << std::endl;

    timecounter tc;
    std::vector<int> input_bubblesort; // copia per bubble-sort
    std::vector<int> input_insertionsort; // copia per insertion-sort
    std::vector<int> input_selectionsort; // copia per selection-sort
    std::vector<int> input_stdsort; // copia per std::sort()

    std::vector<double> duration_bubblesort;
    std::vector<double> duration_insertionsort;
    std::vector<double> duration_selectionsort;
    std::vector<double> duration_stdsort;
    
    for (int j=0; j<vec.size(); j++) {
        // durata bubble-sort
        input_bubblesort = vec[j]; 
        tc.tic();
        bubble_sort(input_bubblesort);
        duration_bubblesort.push_back(tc.toc());
        
        // durata insertion-sort
        input_insertionsort = vec[j];
        tc.tic();
        insertion_sort(input_insertionsort);
        duration_insertionsort.push_back(tc.toc());
        
        // durata selection-sort
        input_selectionsort = vec[j];
        tc.tic();
        selection_sort(input_selectionsort);
        duration_selectionsort.push_back(tc.toc());
        
        // durata metodo sort()
        input_stdsort = vec[j];
        tc.tic();
        std::sort(input_stdsort.begin(),input_stdsort.end());
        duration_stdsort.push_back(tc.toc());
    }
    
    
    std::cout << "Bubble-sort: ";
    for (int j=0; j<vec.size(); j++) std::cout << duration_bubblesort[j] << "\t"; 
    std::cout << std::endl;

    std::cout << "Insertion-sort: ";
    for (int j=0; j<vec.size(); j++) std::cout << duration_insertionsort[j] << "\t"; 
    std::cout << std::endl;

    std::cout << "Selection-sort: ";
    for (int j=0; j<vec.size(); j++) std::cout << duration_selectionsort[j] << "\t"; 
    std::cout << std::endl;

    std::cout << "Std sort: ";
    for (int j=0; j<vec.size(); j++) std::cout << duration_stdsort[j] << "\t"; 
    std::cout << std::endl;

}
