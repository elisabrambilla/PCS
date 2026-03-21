# include <iostream>
# include <cmath>

int main()
{
    static const int N = 10;
    double arr[N] = {0.0,1.1,2.2,3.3,4.4,5.5,6.6,7.7,8.8,9.9};
    double minimo = arr[0]; // inizializzo il minimo
    double massimo = arr[0]; // inizializzo il massimo
    double somma = arr[0]; // inizializzo la somma

    for (int i=1; i<= N-1; i++){
        minimo = std::min(arr[i],minimo);
        massimo = std::max(arr[i],massimo);
        somma =  somma + arr[i];
    }

    double media = somma/N;

   
    double somma_dev = 0;
    for (int i=0; i<= N-1; i++){
        somma_dev = somma_dev + ((arr[i]-media)*(arr[i]-media))/N;
    }
    double dev_st = std::sqrt(somma_dev);

    std::cout << "Il minimo è: " << minimo << "\n";
    std::cout << "Il massimo è: " << massimo << "\n";
    std::cout << "La media è: " << media << "\n";
    std::cout << "La deviazione standard è: " << dev_st << "\n";

    return 0;
}