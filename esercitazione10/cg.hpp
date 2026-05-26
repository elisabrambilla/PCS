#include <iostream>
#include <string>
#include <Eigen/Dense>


struct Output {
    std::string convergenza;
    int iter;
    Eigen::VectorXd sol;
    double residuo;
};

Output cg(Eigen::MatrixXd& A, Eigen::VectorXd& b, const double r_tol, const unsigned int it_max) {
    const int n = b.size();
    Eigen::VectorXd x = Eigen::VectorXd::Zero(n);
    Eigen::VectorXd res = b - A * x;
    const double r0 = res.norm();
    Eigen::VectorXd p = res; 
    int it = 0;
    Eigen::VectorXd res_prec = Eigen::VectorXd::Zero(n);

    while (it < it_max && res.norm() > r_tol * r0) {
        const double alpha = ((p.transpose()*res)/(p.transpose() * A * p)).value() ;
        x = x + alpha * p;
        res = b - A * x;
        const double beta = ((p.transpose() * A * res)/(p.transpose() * A * p)).value() ;
        p = res - beta * p;
        it++;
    }

    std::string conv;
    if (it < it_max) {
        conv = "SI";
    }
    else {conv = "NO"; }

    Output s = {.convergenza = conv, .iter = it, .sol = x, .residuo = res.norm()};

    return s;
}