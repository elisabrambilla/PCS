#include <iostream>
#include "rational.hpp"

int main() {

    // Semplificazione
    rational<int> a(3,6);
    std::cout << "(3/6) semplificata è: " << a << "\n";

    // Operazioni base
    rational<int> b(2,3);

    std::cout << a << " + " << b << " = " << (a + b) <<"\n";
    std::cout << a << " - " << b << " = " << (a - b) <<"\n";
    std::cout << a << " * " << b << " = " << (a * b) <<"\n";
    std::cout << a << " / " << b << " = " << (a / b) <<"\n";

    // Inf e NaN
    rational<int> infinito(1,0);
    rational<int> m_inf(-1,0);
    rational<int> nan(0,0);
    rational<int> zero(0,1);

    std::cout << "Infinito: " << infinito << "\n";
    std::cout << "Not a Number: " << nan << "\n";

    
    std::cout << "Somma con infinito: " << a << " + Infinito = " << (a + infinito) << "\n";
    std::cout << "Somma con NaN: " << a << " + NaN = " << (a + nan) << "\n";
    std::cout << "Divisione per zero: " << a << " / 0 = " << (a/zero) << "\n";
    std::cout << "Divisione per infinito: " << a << " / Infinito = " << (a/infinito) << "\n";
    std::cout << "Divisione per NaN: " << a << " / NaN = " << (a/nan) << "\n";
    std::cout << "Zero diviso zero: 0 / 0 = " << (zero/zero) << "\n";

    std::cout << "Inf + (-Infinito) = " << infinito + m_inf << "\n";
    std::cout << "(-Inf) - (-Infinito) = " << m_inf - m_inf << "\n";

    int num, den ;
    std::cout << "Inserisci il numeratore: ";
    std::cin >> num ;
    std::cout << "Inserisci il denominatore: ";
    std::cin >> den ;
    rational<int> r(num,den);

    std::cout << "Il razionale è: " << r << "\n";
    std::cout << "Somma con infinito: " << r << " + Infinito = " << (r + infinito) << "\n";
    std::cout << "Somma con NaN: " << r << " + NaN = " << (r + nan) << "\n";
    std::cout << "Divisione per zero: " << r << " / 0 = " << (r/zero) << "\n";
    std::cout << "Divisione per infinito: " << r << " / Infinito = " << (r/infinito) << "\n";
    std::cout << "Divisione per NaN: " << r << " / NaN = " << (r/nan) << "\n";

    
    return 0;

}