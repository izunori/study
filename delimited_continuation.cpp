#include<iostream>
#include<tuple>
#include<vector>
#include<string>
#include<functional>


void print() {std::cout << std::endl;}

template <class Head, class... Tail>
void print(Head&& head, Tail&&... tail)
{
  std::cout << head << ",";
  print(std::forward<Tail>(tail)...);
}

template<typename A, typename B>
using Hom = std::function<B(A)>;

template<typename T, typename R>
using Cont = Hom<Hom<T,R>,R>;

template<typename A, typename B, typename R>
using KHom = Hom<A,Cont<B,R>>;

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

template<typename T, typename T2, typename R>
Hom<Cont<T,R>, Cont<T2,R>> contF(Hom<T,T2> f){
    return [f](Cont<T,R> g){
        return [f,g](Hom<T2,R> h){
            Hom<T,R> k = [f,h](T x){return h(f(x));};
            return g(k);
        };
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

template<typename T, typename T2, typename R>
Cont<T,R> call_cc(Hom<Hom<T,Cont<T2,R>>,Cont<T,R>> f){
//Cont<T,R> call_cc(Hom<KHom<T,T2,R>,Cont<T,R>> f){
    return [f](Hom<T,R> k) -> R{
        Hom<T, Cont<T2,R>> g = [&k](T t){
        //KHom<T,T2,R> g = [k](T t){
            return [k,t](Hom<T2,R> x){
                return k(t);
            };
        };
        return f(g)(k);
    };
}

Hom<int, int> mul(int n){
    return [n](int x){print("mul",n*x); return n * x;};
}

template<typename T2, typename R>
Cont<int,R> prod2(std::vector<int>& v, int i){
    return call_cc<int,T2,R>([i,&v](Hom<int,Cont<T2,R>> cc) -> Cont<int,R>{
            print(i,v[i]);
            if(i == v.size()) return eta<int,R>(1);
            //if(v[i] == 0) return cc(0);
            if(v[i] == 0) return [cc](Hom<int,R> f){return cc(0)([](T2 x){return std::vector<int>();});}; // Rですらある必要がないのはなぜ？
            return contF<int,int,R>(mul(v[i]))(prod2<T2,R>(v,i+1));
        });
}

template<typename T, typename T2, typename R>
KHom<T,T2,R> toKleisli(Hom<T,T2> f){
    return [&f](T t){
        return eta<T2,R>(f(t));
    };
}


template<typename R>
Cont<int,R> prod(std::vector<int>& v, int i){
    return call_cc<int,int,R>([i,&v](Hom<int,Cont<int,R>> cc) -> Cont<int,R>{
            print(i,v[i]);
            if(i == v.size()) return eta<int,R>(1);
            if(v[i] == 0) {print("find 0");return cc(0);};
            //Hom<int,int> ele = [&v,i](int x){print("mul",x);return x * v[i];};
            //return contF<int,int,R>(ele)(prod<R>(v,i+1));
            return contF<int,int,R>(mul(v[i]))(prod<R>(v,i+1));
        });
}

template<typename R>
Cont<int,R> impl(Hom<int,Cont<int,R>> cc, std::vector<int>& v, int i){
    print(i,v[i]);
    if(i == v.size()) return eta<int,R>(1);
    if(v[i] == 0){print("find 0");return cc(0);};
    return contF<int,int,R>(mul(v[i]))(impl<R>(cc,v,i+1));
}

template<typename R>
Cont<int,R> prod3(std::vector<int>& v, int i){
    return call_cc<int,int,R>([i,&v](Hom<int,Cont<int,R>> cc) -> Cont<int,R>{
            return impl<R>(cc,v,i);
        });
}

//template<typename R>
//Hom<int,Cont<int,R>> save;

template<typename R>
class Fact{
public:
    Hom<int,Cont<int,R>> save;
    
    Cont<int,R> fact(int n){
    print("fact", n);
    if(n == 1){
        return call_cc<int,int,R>([this](Hom<int,Cont<int,R>> cc)->Cont<int,R>{
                //this->save = cc;
                //this->save = [](int x){return eta<int,R>(10*x);};
                //std::cout << &(cc) << std::endl;
                //std::cout << &(this->save) << std::endl;
                return eta<int,R>(1);
                });
    } else {
        return contF<int,int,R>(mul(n))(fact(n-1));
    };
}
};

int main(){

    Hom<int, int> twice = [](int x){return 2*x;};
    Hom<int, int> id = [](int x){return x;};
    Hom<int, void> print_int = [](int x){std::cout << x << std::endl; return;};
    std::cout << eta<int,int>(5)(twice) << std::endl;
    std::cout << fact(10)(id) << std::endl;

    //Hom<int, int> f = fact2;
    //Cont<Hom<int,int>,int> cf = eta<Hom<int,int>, int>(f);
    //Hom<int, Cont<int,int>> cf2 = mac(cf);
    //std::cout << cf2(10)(id) << std::endl;

    std::vector<int> v = {1,2,3,0,5};
    print("pattern 1");
    Cont<int,int> c = prod<int>(v,0);
    std::cout << c(id) << std::endl;
    //Cont<int,void> c2 = prod<void>(v,0);
    //c2(print);
    print("pattern 2");
    Cont<int,void> c2 = prod2<std::string, void>(v,0);
    c2(print_int);

    print("pattern 3");
    Cont<int,void> c3 = prod3<void>(v,0);
    c3(print_int);

    //Fact<int> fact;
    //print(fact.fact(3)(id));
    //std::cout << &(fact.save) << std::endl;
    //std::cout << fact.save(2)(id) << std::endl;
    return 0;
}
