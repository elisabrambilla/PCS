#pragma once
#include <vector>
#include <optional>
#include <string>

template<typename T>
bool is_sorted(const std::vector<T>& vec) // is_sorted() verifica l'ordinamento del vettore
{
    if (vec.size() == 0) { // se il vettore ha dimensione 0 allora è per definizione ordinato
    return true;
    }
    for (int i=1 ; i < vec.size(); i++) {
        if (vec[i-1]>vec[i]) {return false; // se trova un solo errore nell'ordinamento esce dal ciclo e restituisce false
        }
    }
    return true; // se non trova errori, restituisce true
}

// Bubble-Sort
template<typename T>
void bubble_sort(std::vector<T>& vec) {
    //if (vec.size()<2) {return;}
    for (int i=0; i < vec.size()-1; i++) {
        for (int j=vec.size()-1; j>i ; j--) {
            if (vec[j]<vec[j-1]) {std::swap(vec[j],vec[j-1]);}
        }
    }
}

// Insertion-Sort
template<typename T>
void insertion_sort(std::vector<T>& vec) {
    for ( int j=1; j<= vec.size()-1; j++) {
        T k=vec[j];
        int i=j-1;
        while (i>=0 && vec[i]>k) {
            vec[i+1]=vec[i];
            i = i-1;
        }
        vec[i+1]=k;
    }
}

// Selection-Sort
template<typename T>
void selection_sort(std::vector<T>& vec) {
    for (int i=0; i<vec.size()-1; i++) {
        int min=i;
        for (int j=i+1; j<vec.size(); j++) {
            if (vec[j]<vec[min]) {
                min=j;
            }
        }
        std::swap(vec[i],vec[min]);
    }
}

// Merge
template<typename T>
void merge(std::vector<T>& A,int p,int q,int r) // fonde due sottometà ordinate di A in un unico ordinato
{
    int n1 = q-p+1;
    int n2 = r-q;
    std::vector<T> L;
    L.resize(n1);
    std::vector<T> R;
    R.resize(n2);
    for (int i=0; i<n1; i++) 
    {
        L[i] = A[p+i];
    }
    for (int j=0; j<n2; j++)
    {
        R[j]=A[q+j+1];
    }
    
    int i=0;
    int j=0;
    int k=p;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            A[k] = L[i];
            i++; // solo se vince L[i] aumento l'indice i e posiziono L[i] in posizione k di A
        } else {
            A[k] = R[j];
            j++; // solo se vince R[j] (è più piccolo di L[i]) aumento l'indice j e posiziono R[j] in posizione k di A
        }
        k++;
    }
    while (i < n1)
    {
        A[k]=L[i];
        i++;
        k++;
    }
    while (j < n2)
    {
        A[k]=R[j];
        j++;
        k++;
    }


    }

// Merge-sort
template<typename T>
void merge_sort(std::vector<T>& A, int p, int r) // prende parametri il vettore, p indice di inizio, r di fine
{
    if (p<r)
    {
        int q = (p+r)/2; // funzione pavimento
        merge_sort(A,p,q); // chiamate ricorsive
        merge_sort(A,q+1,r);
        merge(A,p,q,r);
    }
}

// Partition
template<typename T>
int partition(std::vector<T>& A, int p, int r) // p = 0, r = A.size()-1
{
    T x = A[r];
    int i = p-1;
    for (int j=p; j<r; j++)
    {
        if (A[j]<= x) {
            i++;
            std::swap(A[i],A[j]);
        }
    }
    std::swap(A[i+1],A[r]);
    return i+1;
}

template<typename T>
void quick_sort(std::vector<T>& A, int p, int r)
{
    if (p<r)
    {
        int q = partition(A,p,r);
        quick_sort(A,p,q-1);
        quick_sort(A,q+1,r);
    }
}


// Insertion-Sort con parametri anche indici di inizio (0) e fine (vec.size()-1)
template<typename T>
void insertion_sort_index(std::vector<T>& vec, int p, int r) 
{
    for ( int j=p+1; j<= r; j++) {
        T k=vec[j];
        int i=j-1;
        while (i>=p && vec[i]>k) {
            vec[i+1]=vec[i];
            i = i-1;
        }
        vec[i+1]=k;
    }
}

// Quick-sort modificato decide se effettuare la chiamata ricorsiva a se stesso se la dimensione del vettore è > n0 = 200, altrimenti chiama Insertion_sort_index()
template<typename T>
void my_sort(std::vector<T>& A, int p, int r)
{
    if (p<r) {
        if ((r-p)<200) 
        { 
        insertion_sort_index(A,p,r);
        }
        else {
            int q = partition(A,p,r);
            my_sort(A,p,q-1);
            my_sort(A,q+1,r);
        }
    }
    
}



