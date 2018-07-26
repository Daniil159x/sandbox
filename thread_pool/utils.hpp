#ifndef UTILS_HPP
#define UTILS_HPP

#include <tuple>
#include <future>

#include <iostream>

namespace detali {
    // help for tuple
    template<int ...>
    struct index_sequence { };

    template<int N, int ...S>
    struct make_index : make_index<N-1, N-1, S...> { };

    template<int ...S>
    struct make_index<0, S...> {
        typedef index_sequence<S...> type;
    };

    // for invoking
    template <typename T>
    struct tag_invoker {};



    // copy task
    template <typename Func, typename Signature>
    class CopyLambdaCpp14;

    template <typename Func, typename RetT, typename ...Args>
    class CopyLambdaCpp14<Func, RetT(Args...)> {
    public:
        CopyLambdaCpp14(Func && foo, Args && ...args) : m_func(std::forward<Func>(foo)),
                                                        m_args{args...} {} // copy

        CopyLambdaCpp14(CopyLambdaCpp14&& o) = default;

        void operator() () noexcept {
            Invoke_(typename make_index<sizeof... (Args)>::type(), tag_invoker<RetT>());
        }

        std::future<RetT> GetFuture() {
            return m_return.get_future();
        }

        ~CopyLambdaCpp14() = default;

    private:
        Func                m_func;
        std::tuple<Args...> m_args;
        std::promise<RetT>  m_return;

        template <int ...Idx, class T>
        void Invoke_(index_sequence<Idx...>, tag_invoker<T>) noexcept {
            try {
                m_return.set_value( m_func( std::get<Idx>(m_args)... ) );
            } catch (...) {
                try {
                    m_return.set_exception(std::current_exception());
                } catch (...) { /* TODO */ }
            }
        }

        template <int ...Idx>
        void Invoke_(index_sequence<Idx...>, tag_invoker<void>) noexcept {
            try {
                m_func( std::get<Idx>(m_args)... );
                m_return.set_value();
            } catch (...) {
                try {
                    m_return.set_exception(std::current_exception());
                } catch (...) { /* TODO */ }
            }
        }
    };

    // ref task
    template <typename Func, typename Signature>
    class RefLambdaCpp14;

    template <typename Func, typename RetT, typename ...Args>
    class RefLambdaCpp14<Func, RetT(Args...)> {
    public:
        RefLambdaCpp14(Func && foo, Args && ...args) : m_func(std::forward<Func>(foo)),
                                                        m_args{std::forward<Args>(args)...} {} // forw ref

        RefLambdaCpp14(RefLambdaCpp14&&) = default;

        void operator() () noexcept {
            Invoke_(typename make_index<sizeof... (Args)>::type(), tag_invoker<RetT>());
        }

        std::future<RetT> GetFuture() {
            return m_return.get_future();
        }

        ~RefLambdaCpp14() = default;

    private:
        Func                m_func;
        std::tuple<Args&&...> m_args;
        std::promise<RetT>  m_return;

        template <int ...Idx, class T>
        void Invoke_(index_sequence<Idx...>, tag_invoker<T>) noexcept {
            try {
                m_return.set_value( m_func( std::get<Idx>(m_args)... ) );
            } catch (...) {
                try {
                    m_return.set_exception(std::current_exception());
                } catch (...) { /* TODO */ }
            }
        }

        template <int ...Idx>
        void Invoke_(index_sequence<Idx...>, tag_invoker<void>) noexcept {
            try {
                m_func( std::get<Idx>(m_args)... );
                m_return.set_value();
            } catch (...) {
                try {
                    m_return.set_exception(std::current_exception());
                } catch (...) { /* TODO */ }
            }
        }
    };
}

#endif // UTILS_HPP
