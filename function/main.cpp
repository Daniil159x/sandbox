#include <iostream>
#include <string>

class BaseMyFoo {
protected:

    union functor_t {
        void *dyn_func;
        char locale_func[sizeof(void*)];
    } m_functor;

    template <class Foo>
    void Init(Foo &&f) {
        if constexpr(is_locale_storage<Foo>){
            std::cout << "\nSTATIC ALLOC" << std::endl;
            new (m_functor.locale_func) Foo(std::forward<Foo>(f));
        }
        else {
            std::cout << "\nDYNAMIC ALLOC" << std::endl;
            m_functor.dyn_func = static_cast<void*>( new Foo(std::forward<Foo>(f)) );
        }
        m_deleter = &BaseMyFoo::Deleter<Foo>;
    }

    // ----- static ------

    template <class Foo>
    constexpr static bool is_locale_storage = sizeof(Foo) <= sizeof(functor_t);

    template <class Ret, class ...Args>
    using invoke_t = Ret( *)(functor_t &, Args&& ...);

    template <class Ret, class Foo, class ...Args>
    static Ret Invoking(functor_t &func, Args && ...args) {
        if(is_locale_storage<Foo>){
            return (*static_cast< Foo* >(static_cast< void* >(func.locale_func)) )(std::forward<Args>( args )...);
        }
        else {
            return (*static_cast< Foo* >(func.dyn_func) )(std::forward<Args>(args)...);
        }
    }

private:
    using deletet_t = void(*)(functor_t &func);

    template <class Foo>
    static void Deleter(functor_t &func) {
        if( is_locale_storage<Foo> ) {
            static_cast< Foo* >(static_cast< void* >(func.locale_func))->~Foo();
        }
        else {
            delete static_cast< Foo* >(func.dyn_func);
        }
    }
    deletet_t m_deleter;

public:

    ~BaseMyFoo()
    {
        m_deleter( m_functor );
    }
};

template <class Signature>
class MyFoo;

template <class Ret, class ...Args>
class MyFoo<Ret (Args...)> : private BaseMyFoo {
public:

    template <class Func>
    MyFoo(Func &&f) {
        m_invoker = &BaseMyFoo::Invoking<Ret, Func, Args...>;
        BaseMyFoo::Init( std::forward<Func>( f ) );
    }

    Ret operator () ( Args&& ... args ) {
        return m_invoker( m_functor, std::forward<Args>( args )... );
    }

private:
    BaseMyFoo::invoke_t<Ret, Args...> m_invoker;
};



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