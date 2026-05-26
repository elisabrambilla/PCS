#include <iostream>
#include <Eigen/Dense>
#include <string>
#include "cg.hpp"

int main()
{
    const double tol = 1.0e-15;
    const unsigned int it_max = 200;
    const double res_tol = 1.0e-8; // relativa

    // Verifichiamo la correttezza dell'algorito del Gradiente Coniugato con sistemi di dimensioni diverse a piacere
    std::vector< unsigned int > dim = { 3, 5, 10, 20, 100}; 

    for ( unsigned int n : dim) {
        Eigen::MatrixXd B = Eigen::MatrixXd::Random(n,n);
            if (std::abs(B.determinant()) < tol) {
                std::cout << "Errore: la matrice B" << n << " è singolare"<< std::endl;
                return -1;
            }
            Eigen::MatrixXd A = B.transpose() * B;
            Eigen::VectorXd x_ex = Eigen::VectorXd::Ones(n);
            Eigen::VectorXd b = A * x_ex;

            Output cg_ = cg( A, b, res_tol, it_max);

            
            std::cout << "Sistema di dimensione " << n << ", A * x = b: " << std::endl;
            std::cout << "Convergenza: " << cg_.convergenza << std::endl;
            std::cout << "Numero di iterazioni: " << cg_.iter << std::endl;
            std::cout << "Soluzione: \n " << cg_.sol << std::endl;
            std::cout << "Residuo (b - Ax).norm(): " << cg_.residuo << std::endl;
    }

    
}

/* L'algoritmo converge */