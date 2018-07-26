#ifndef FUNTION_HPP
#define FUNTION_HPP

#include <utility>

#include <iostream>

class BaseMyFoo {
protected:

    union functor_t {
        void *dyn_func;
        char locale_func[sizeof(void*)];
    } m_functor;

    template <class Foo>
    void Init(Foo &&f) {
        if constexpr(is_locale_storage<Foo>){
            new (m_functor.locale_func) Foo(std::forward<Foo>(f));
        }
        else {
            m_functor.dyn_func = static_cast<void*>( new Foo(std::forward<Foo>(f)) );
        }
        m_deleter = &BaseMyFoo::Deleter<Foo>;
    }

    BaseMyFoo() = default;
    BaseMyFoo(BaseMyFoo&& f) : m_functor(std::move(f.m_functor)), m_deleter(std::move(f.m_deleter))
    {
        f.m_deleter = nullptr;
        f.m_functor.dyn_func = nullptr;
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
        if(m_deleter != nullptr){
            m_deleter( m_functor );
        }
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

    MyFoo(MyFoo&& f) : BaseMyFoo(std::move(f)), m_invoker(std::move(f.m_invoker)) {
        f.m_invoker = nullptr;
    }

    Ret operator () ( Args&& ... args ) {
        return m_invoker( m_functor, std::forward<Args>( args )... );
    }

private:
    BaseMyFoo::invoke_t<Ret, Args...> m_invoker;
};


#endif // FUNTION_HPP
