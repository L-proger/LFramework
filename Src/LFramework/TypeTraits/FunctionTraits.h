#pragma once

#include <cstddef>
#include <tuple>

namespace LFramework {

template<class F>
struct FunctionTraits;

template<class R, class... Args>
struct FunctionTraits<R(Args...)> {
    using ReturnType = R;

    static constexpr std::size_t Arity = sizeof...(Args);

    template <int N>
    struct ArgumentHelper {
        static_assert(N < Arity, "error: invalid parameter index.");
        using Type = typename std::tuple_element<N, std::tuple<Args...>>::type;
    };

    template <int N>
    using Argument = typename ArgumentHelper<N>::Type;
};

// function pointer
template<class R, class... Args>
struct FunctionTraits<R(*)(Args...)> : public FunctionTraits<R(Args...)> {
};

}
