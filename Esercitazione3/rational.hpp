#include <iostream> 
#include <concepts>
#pragma once
#include <numeric>

template<typename I> requires std::integral<I>
class rational{
    I num_;
    I den_;

I my_gcd(I a, I b) {
    if (a<0) {
        a=-a;
    }
    if (b<0) {
        b=-b;
    }
    while (b!=I{0}) { // Algoritmo di Euclide
        I resto = a % b;
        a = b;
        b = resto;
    }
    return a;
}

void simplify() {
        if (is_nan() || is_inf()) return;
        if (num_ == I{0}) { den_ = I{1}; return; }
        I mcd = my_gcd(num_, den_);
        num_ /= mcd;
        den_ /= mcd;
        if (den_ < I{0}) { num_ = -num_; den_ = -den_; }
    }

public:
    // costruttore di default
    rational()
        :num_(I{0}), den_(I{1})
        {}
    // costruttore user-defined
    rational(const I& p_num, const I& p_den)
        :num_(p_num),den_(p_den) {
            simplify();
        }


    // verifica se il numero è inf, NaN, finito
    bool is_inf() const { return num_ != I{0} && den_ == I{0}; }
    bool is_nan() const { return num_ == I{0} && den_ == I{0}; }
    bool is_finito() const { return den_ != I{0}; }


    // Restituisco i valori di numeratore e denominatore
    I num() const {return num_; }
    I den() const {return den_; }

    // incremento
    rational& operator+=(const rational& other) {
        if (is_nan() || other.is_nan()) {
            num_=I{0};
            den_=I{0};
        }
        else if (is_inf() && other.is_finito()) {
            if (num_>0) {
                num_=I{1};
                den_=I{0};}
            else {
                num_=I{-1};
                den_=I{0};}
        }
        else if (is_finito() && other.is_inf() ) {
            if (other.num_>0) {
                num_=I{1};
                den_=I{0};
            }
            else {
                num_=I{-1};
                den_=I{0};
            }
        }
        else if (is_inf() && other.is_inf()) {
            if ((num_>0 && other.num_>0) || (num_<0 && other.num_<0)) { // infiniti con stesso segno
                den_=I{0};
                if (num_>0) { // (+inf)+(+inf)
                    num_=I{1};
                }
                else { // (-inf)+(-inf)
                    num_=I{-1};
                }
            }
            else { // (+inf)+(-inf) o viceversa dà NaN
                num_=I{0};
                den_=I{0};
            }
        }
        else {
            num_ = num_*other.den_ + other.num_*den_ ;
            den_ = den_*other.den_ ;
            simplify();
        }
        return *this;
    }

    //somma
    rational operator+(const rational& other) const {
        rational ret = *this; // copia dell'oggetto a cui si riferisce il puntatore this
        ret += other; // ci sommo l'altro elemento
        return ret;
    }

    // decremento
    rational& operator-=(const rational& other) {
        if (is_nan() || other.is_nan()) {
            num_=I{0};
            den_=I{0};
        }
        else if (is_inf() && other.is_finito()) {
            if (num_>0) {
                num_=I{1};
                den_=I{0};}
            else {
                num_=I{-1};
                den_=I{0};}
        }
        else if (is_finito() && other.is_inf() ) {
            if (other.num_>0) {
                num_=I{-1};
                den_=I{0};
            }
            else {
                num_=I{1};
                den_=I{0};
            }
        }
        else if (is_inf() && other.is_inf()) {
            if ((num_>0 && other.num_<0) || (num_<0 && other.num_>0)) { // infiniti con segno diverso 
                den_=I{0};
                if (num_>0) { // (+inf)-(-inf)
                    num_=I{1};
                }
                else { // (-inf)-(+inf)
                    num_=I{-1};
                }
            }
            else { // (+inf)-(+inf) o (-inf)-(-inf) dà NaN
                num_=I{0};
                den_=I{0};
            }
        }
        else {
            num_ = num_*other.den_ - other.num_*den_ ;
            den_ = den_*other.den_ ;
            simplify();
        }
        return *this;
    }

    //sottrazione
    rational operator-(const rational& other) const {
        rational ret = *this; // copia dell'oggetto a cui si riferisce il puntatore this
        ret -= other; // ci sommo l'altro elemento
        return ret;
    }


    // *=
    rational& operator*=(const rational& other) {
        if (is_nan() || other.is_nan()) {
            num_=I{0};
            den_=I{0};
        }
        else { // gestisce in maniera corretta anche inf*inf (anche con segni diversi), finito*inf, inf*finito, inf*0, etc...
            num_ *= other.num_ ;
            den_ *= other.den_ ;
            simplify();
        }
        return *this;
    }

    // moltiplicazione
    rational operator*(const rational& other) const {
        rational ret = *this; // copia dell'oggetto a cui si riferisce il puntatore this
        ret *= other; // ci sommo l'altro elemento
        return ret;
    }

    // /=
    rational& operator/=(const rational& other) {
        if (is_nan() || other.is_nan()) { // divisione tra NaN
            num_=I{0};
            den_=I{0};
        }
        else if (other.is_inf()) {
            if (is_inf()) { // inf/inf dà NaN
                num_=I{0};
                den_=I{0};
            }
            else if (is_finito()) { // finito/inf dà zero
                num_=I{0};
                den_=I{1};
            }
        }
        else if (other.num_==I{0}) { // Divisione per zero dà NaN, + o - Inf
            if (num_== I{0}) {
                num_=I{0};
                den_=I{0};
            }
            else {
                if (num_>0) {
                    num_=I{1};
                    den_=I{0};
                }
                else {
                    num_=I{-1};
                    den_=I{0};
                }
            }
        }
        else { // finito/finito, in realtà gestisce bene anche NaN/NaN, inf/NaN, finito/NaN, inf/finito, -inf/finito etc..
            num_ *= other.den_ ;
            den_ *= other.num_ ;
            simplify();
        }
        return *this;
    }

    // divisione
    rational operator/(const rational& other) const {
        rational ret = *this; // copia dell'oggetto a cui si riferisce il puntatore this
        ret /= other; // ci sommo l'altro elemento
        return ret;
    }

};

// Overload
template<typename I>
std::ostream&
operator<<(std::ostream& os, const rational<I>& r)
{
    if (r.is_nan()) return os << "NaN";
    if (r.is_inf()) {
        if (r.num()>0) return os << "Inf";
        else return os <<"-Inf";
    } 
    
    os << "(" << r.num() << "/" << r.den() << ")";
    return os;
}
