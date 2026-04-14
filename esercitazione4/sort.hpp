#pragma once
#include <vector>
#include <optional>

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
    for (int i=0; i <= vec.size()-2; i++) {
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