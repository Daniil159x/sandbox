#include <iostream>
#include <string>

#include "function.hpp"



std::string ToStr( int a, double b, char delim ) {
    return std::to_string( a ) + delim + std::to_string( b );
}

struct BigFunctor {
    char name[255] = "hello world, !!!!!!!!!!!!!!!!";

    std::string operator ()(size_t idx, size_t count) {
        return{ name + idx, name + idx + count };
    }
};

int main() {
    std::cout << std::boolalpha;

    std::cout << "\nlamda:" << std::endl;
    {
        MyFoo<int( int )> foo = []( int a ) -> int {
            std::cout << a << std::endl;
            return a * a * a * a;
        };
        std::cout << "2 ^ 4 = " << foo( 2 ) << std::endl;
    }

    std::cout << "\nfunction:" << std::endl;
    {
        MyFoo<std::string( int, double, char )> foo = &ToStr;
        std::cout << "strings(2^4.5) = " << foo( 2, 4.5, '^' ) << std::endl;
    }

    std::cout << "\nfunctor:" << std::endl;
    {
        MyFoo<bool( int, int )> foo = std::less<>();

        std::cout << "9 < 5 ?" << foo( 9, 5 ) << std::endl;
    }

    std::cout << "\nbig functor:" << std::endl;
    {
        MyFoo<std::string( size_t, size_t )> foo = BigFunctor();

        std::cout << "pos from 3 to 8 = " << foo( 3, 5 ) << std::endl;
    }


    std::cin.get();
    return 0;
}