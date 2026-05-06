#include <iostream>
#include <vector>
#include "sort.hpp"
#include "timecounter.h"
#include "randfiller.h"
#include <algorithm>

int main()
{
    // Cerchiamo ora il valore k tale che per n<k sono più veloci gli algoritmi quadratici (Bubble,Insertion o Selection), per n>=k sono più veloci quelli logaritmici (Merge o Quick)
    // in questo caso m = dim(vec) va da 1 a 200
    
    // Creazione dei 100 vettori
    const int n = 100;
    randfiller rf;
    const int m = 300; // 200 dimensioni diverse
    
    std::vector<double> duration_bubblesort(m);
    std::vector<double> duration_insertionsort(m);
    std::vector<double> duration_selectionsort(m);
    std::vector<double> duration_mergesort(m);
    std::vector<double> duration_quicksort(m);
    std::vector<double> duration_mysort(m);
    std::vector<double> duration_stdsort(m);


    std::vector<int> v_dim; // vettore delle contenente le 100 dimensioni possibili dei vec
    v_dim.resize(m);
    for (int i=0; i<m; i++)
    {
        v_dim[i]=i+2; // dimensioni da 2 a 101
    }

    

    for (int i=0; i<m; i++) { // per ogni dimensione elemento di vi
        int dim = v_dim[i]; 

        std::vector<std::vector<int>> vecvec; // vecvec è il vettore contenente 100 vettori della stessa dimensione dim
        vecvec.resize(n); 

        // creo 100 vettori vec per dimensione
        for (int j=0; j<n; j++) 
        {
            vecvec[j].resize(dim);
            rf.fill(vecvec[j],-10000,10000); // riempio il vec con valori casuali  
        }

        std::vector<std::vector<int>> input_bubblesort = vecvec; // copia per bubble-sort
        std::vector<std::vector<int>> input_insertionsort = vecvec; // copia per insertion-sort
        std::vector<std::vector<int>> input_selectionsort = vecvec; // copia per selection-sort
        std::vector<std::vector<int>> input_mergesort = vecvec; // copia per merge-sort
        std::vector<std::vector<int>> input_quicksort = vecvec; // copia per quick-sort
        std::vector<std::vector<int>> input_mysort = vecvec; // copia per my_sort
        std::vector<std::vector<int>> input_stdsort = vecvec; // copia per std::sort

    
        timecounter tc;

        // Bubble-sort
        tc.tic();
        for (int j=0; j<n; j++)
        {
            bubble_sort(input_bubblesort[j]);
        }
        duration_bubblesort[i]=tc.toc()/n; // inserisco il valore medio per quella specifica dimensione del vettore
    
        // Insertion-sort
        tc.tic();
        for (int j=0; j<n; j++)
        {
            insertion_sort(input_insertionsort[j]);
        }
        duration_insertionsort[i]=tc.toc()/n; // inserisco il valore medio per quella specifica dimensione del vettore
    
        // Selection-sort
        tc.tic();
        for (int j=0; j<n; j++)
        {
            selection_sort(input_selectionsort[j]);
        }
        duration_selectionsort[i]=tc.toc()/n; // inserisco il valore medio per quella specifica dimensione del vettore
    
        // Merge-sort
        tc.tic();
        for (int j=0; j<n; j++)
        {
            merge_sort(input_mergesort[j],0, dim-1);
        }
        duration_mergesort[i]=tc.toc()/n; // inserisco il valore medio per quella specifica dimensione del vettore
    
        // Quick-sort
        tc.tic();
        for (int j=0; j<n; j++)
        {
            quick_sort(input_quicksort[j],0, dim-1);
        }
        duration_quicksort[i]=tc.toc()/n; // inserisco il valore medio per quella specifica dimensione del vettore

        // My-sort
        tc.tic();
        for (int j=0; j<n; j++)
        {
            my_sort(input_mysort[j],0, dim-1);
        }
        duration_mysort[i]=tc.toc()/n; // inserisco il valore medio per quella specifica dimensione del vettore
    
        // Std-sort
        tc.tic();
        for (int j=0; j<n; j++)
        {
            std::sort(input_stdsort[j].begin(), input_stdsort[j].end());
        }
        duration_stdsort[i]=tc.toc()/n; // inserisco il valore medio per quella specifica dimensione del vettore
    

    }

    // Confronto tempi per ogni dimensione da 2 a 200 (escludo 1 che è banale), tenendo conto che i più veloce tra i quadratici è il'Insertion-sort, mentre tra i logaritmici è il quick
    // Obiettivo: trovare k tale che per n<k è più veloce insertion di quicksort
        
    for (int i=0; i<m; i++) // nella singola dimensione i
    {
        double d_b = duration_bubblesort[i];
        double d_i = duration_insertionsort[i];
        double d_s = duration_selectionsort[i];
        double d_m = duration_mergesort[i];
        double d_q = duration_quicksort[i];
        double d_MY = duration_mysort[i];


        std::string best_alg = "Bubble Sort";
        double best_time = d_b;

        if (best_time > d_i ) {
            best_alg = "Insertion Sort";
            best_time = d_i;
        }
        if (best_time > d_s) {
            best_alg = "Selection Sort";
            best_time = d_s;
        }
        if (best_time > d_m) {
            best_alg = "Merge Sort";
            best_time = d_m;
        }
        if (best_time > d_q) {
            best_alg = "Quick Sort";
            best_time = d_q;
        }
        if (best_time > d_MY) {
            best_alg = "My Sort";
            best_time = d_MY;
        }
        std::cout << "Dim = " << i+2 << "\t" << "Algoritmo: " << best_alg << "\t" << "Tempo: " << best_time << "\n";
    }

    // Mostriamo i tempi medi dei vari algoritmi per vettori di dimensioni 2^2,2^4,...,2^13

    std::cout << "Vettori di dimensioni:";
    for (int i=0; i<20; i++)
    {
        std::cout << v_dim[i*15] << "\t";
    }
    std::cout<<"\n";

    std::cout << "Bubble-sort: ";
    for (int j=0; j<20; j++) std::cout << duration_bubblesort[j*15] << "\t"; 
    std::cout << std::endl;

    std::cout << "Insertion-sort: ";
    for (int j=0; j<20; j++) std::cout << duration_insertionsort[j*15] << "\t"; 
    std::cout << std::endl;

    std::cout << "Selection-sort: ";
    for (int j=0; j<20; j++) std::cout << duration_selectionsort[j*15] << "\t"; 
    std::cout << std::endl;

    std::cout << "Merge-sort: ";
    for (int j=0; j<20; j++) std::cout << duration_mergesort[j*15] << "\t"; 
    std::cout << std::endl;    

    std::cout << "Quick-sort: ";
    for (int j=0; j<20; j++) std::cout << duration_quicksort[j*15] << "\t"; 
    std::cout << std::endl;

    std::cout << "My-sort: ";
    for (int j=0; j<20; j++) std::cout << duration_mysort[j*15] << "\t"; 
    std::cout << std::endl;

    std::cout << "Std sort: ";
    for (int j=0; j<20; j++) std::cout << duration_stdsort[j*15] << "\t"; 
    std::cout << std::endl;
}