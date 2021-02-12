//
//  main.cpp
//  Lab3Teori
//
//  Created by Fredrik Sellgren on 2020-03-03.
//  Copyright © 2020 Fredrik Sellgren. All rights reserved.
//
#include <iostream>
/*template<typename T, unsigned exp>
struct Pow
{
    static const long expretion = exp;
};

template<unsigned exp, typename T>
struct multi
{
    typedef Pow<exp*T::expretion> result;
};*/
/*

template<int Exp>
struct Pow{
    
};
template<>
struct Pow<0>{
    double exp(double x){
        return 1,0;
    }
};
*/
/*void h(int soruce, int target, int N)
{
    if(N == 0) return;
    
    h(soruce, 6 - target - soruce, N-1);
    std::cout << "Movign from " << soruce << "to" << target << std::endl;
    h(6-target-soruce, target, N-1);
}*/


/*template <typename T, T Mantissa, int Exp>
struct Pow
{
    static constexpr T result = Mantissa * Pow<T,Mantissa, Exp - 1>::result;
};


template <typename T, T Mantissa>
struct Pow<T, Mantissa, 0>
{
    static constexpr T result = 1;
};*/

template<int source, int target, int N>
struct h
{
    h() {
        std::cout << "move " << source << " to " << target << "\n";
    }
    static constexpr int aux = 6-target-source;
    h<source, aux, N-1> before;
    h<aux, target, N-1> after;
};

template<int source, int target>
struct h<source, target,0>
{
    // recursive case
};

 int main(){
     h<1, 3, 16> first;
     return 0;
 }
/*#include <iostream>

int main(int argc, const char * argv[]) {
    h(1,3,3);
    return 0;
}*/


