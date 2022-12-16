#include<iostream>
#include<tuple>

template<typename A, typename B>
using Hom = std::function<B(A)>;

template<typename T, typename R>
using Cont = Hom<Hom<T,R>,R>;

template<typename T, typename R>
Cont<T,R> eta(T x){
    return [x](Hom<T,R> f){return f(x);};
}

template<typename T, typename R>
Cont<T,R> mu(Cont<Cont<T,R>,R> f){
    return [&f](Hom<T,R> g){
        return f(eta(g));
    };
}

template<typename A, typename B, typename C, typename D>
Hom<A,Hom<Hom<B,C>,D>> mac(Hom<Hom<Hom<A,B>,C>,D> f){
    return [&f](A x){
        return [x,&f](Hom<B,C> g){
            Hom<Hom<A,B>,C> h = [x,&g,&f](Hom<A,B> k){return g(k(x));};
            return f(h);
        };
    };
}

int fact2(int n){
    if(n == 0){
        return 1;
    } else {
        return fact2(n-1) * n;
    }
}


Cont<int,int> fact(int n){
    return [n](Hom<int,int> f){
        if(n == 0){
            return f(1);
        } else {
            return fact(n-1)([n,&f](int m){return f(m * n);});
        }
    };
}


int main(){

    Hom<int, int> twice = [](int x){return 2*x;};
    Hom<int, int> id = [](int x){return x;};
    std::cout << eta<int,int>(5)(twice) << std::endl;
    std::cout << fact(10)(id) << std::endl;

    Hom<int, int> f = fact2;
    Cont<Hom<int,int>,int> cf = eta<Hom<int,int>, int>(f);
    Hom<int, Cont<int,int>> cf2 = mac(cf);
    std::cout << cf2(10)(id) << std::endl;
    return 0;
}
