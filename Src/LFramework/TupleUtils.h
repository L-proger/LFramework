#ifndef TUPLEUTILS_H
#define TUPLEUTILS_H

#include <tuple>
#include <type_traits>

namespace LFramework {


template <class... Ts>
struct Tuple;

template <>
struct Tuple<> {};

template <class T, class... Ts>
struct Tuple<T, Ts...>{
    T first;
    Tuple<Ts...> rest;
    constexpr Tuple() = default;
    template <class U, class...Us, class = typename ::std::enable_if<!::std::is_base_of<Tuple,typename ::std::decay<U>::type>::value>::type>
    constexpr Tuple(U&& u, Us&&...tail) : first(::std::forward<U>(u)), rest(::std::forward<Us>(tail)...) {}
};

template <class T>
struct Tuple<T>{
    T first;
    constexpr Tuple() = default;
    template <class U, class = typename ::std::enable_if<!::std::is_base_of<Tuple, typename ::std::decay<U>::type>::value>::type>
    constexpr Tuple(U&& u) : first(::std::forward<U>(u)) {}
};

template<typename T>
struct IsTuple : std::false_type {};

template<typename ...T>
struct IsTuple<Tuple<T...>> : std::true_type {};


#pragma pack(push, 1)
template <class... Ts>
struct PackedTuple;

template <>
struct PackedTuple<> {};

template <class T, class... Ts>
struct PackedTuple<T, Ts...>{
    T first;
    Tuple<Ts...> rest;
    PackedTuple() = default;
    template <class U, class...Us, class = typename ::std::enable_if<!::std::is_base_of<PackedTuple,typename ::std::decay<U>::type>::value>::type>
    PackedTuple(U&& u, Us&&...tail) : first(::std::forward<U>(u)), rest(::std::forward<Us>(tail)...) {}
};

template <class T>
struct PackedTuple<T>{
    T first;
    PackedTuple() = default;
    template <class U, class = typename ::std::enable_if<!::std::is_base_of<PackedTuple, typename ::std::decay<U>::type>::value>::type>
    PackedTuple(U&& u) : first(::std::forward<U>(u)) {}
};
#pragma pack(pop)

template<typename T>
struct IsPackedTuple : std::false_type {};

template<typename ...T>
struct IsPackedTuple<PackedTuple<T...>> : std::true_type {};


namespace details {
    template<size_t Index, size_t CurrentIndex, typename...Types>
    auto& Get(Tuple<Types...>& t) {
        if constexpr (Index == CurrentIndex) {
            return t.first;
        }
        else if constexpr (sizeof...(Types) > 1) {
            return details::Get<Index, CurrentIndex + 1>(t.rest);
        }
        else {
            static_assert(sizeof(t) == 0, "Invalid tuple index");
        }
    }
}

template<typename T>
struct tupleSizeHelper {
    static constexpr size_t size = 0;
};

template<typename... T>
struct tupleSizeHelper<Tuple<T...>> {
    static constexpr size_t size = sizeof...(T);
};



template<typename T>
constexpr size_t tupleSize = tupleSizeHelper<T>::size;


template<size_t Index, typename...Types>
auto& Get(Tuple<Types...>& t) {
    return details::Get<Index, 0>(t);
}


namespace details {
    template<size_t Index, size_t CurrentIndex, typename...Types>
    auto& Get(PackedTuple<Types...>& t) {
        if constexpr (Index == CurrentIndex) {
            return t.first;
        }
        else if constexpr (sizeof...(Types) > 1) {
            return details::Get<Index, CurrentIndex + 1>(t.rest);
        }
        else {
            static_assert(sizeof(t) == 0, "Invalid tuple index");
        }
    }
}

template<size_t Index, typename...Types>
auto& Get(PackedTuple<Types...>& t) {
    return details::Get<Index, 0>(t);
}

struct TupleSkipType {};

struct TupleStepInType {};

namespace details {
    template<typename...A, typename...B>
    auto tupleCatImpl(Tuple<A...> done, Tuple<B...> t) {
        return Tuple<A..., B...>{};
    };


    template<typename A, typename B>
    using tupleCat = decltype(tupleCatImpl(std::declval<A>(), std::declval<B>()));

    template<typename F, typename...T, typename...U>
    auto tupleSelectImpl(F f, const Tuple<T...>& tuple, const Tuple<U...>& intermediate) {
        if constexpr(sizeof...(T) > 1) {
			using SelectedType = decltype(f(tuple.first));

            if constexpr (std::is_same_v<SelectedType, TupleStepInType>) {
                return tupleCat<decltype(tupleSelectImpl(f, tuple.first, Tuple<>())), decltype(tupleSelectImpl(f, tuple.rest, Tuple<U...>()))>{};
            }else if constexpr (std::is_same_v<SelectedType, TupleSkipType>) {
				return tupleSelectImpl(f, tuple.rest, Tuple<U...>());
			}
			else {
				return tupleSelectImpl(f, tuple.rest, Tuple<U..., SelectedType>());
			}
        }
        else {
            using SelectedType = decltype(f(tuple.first));
            if constexpr (std::is_same_v<SelectedType, TupleStepInType>) {
                return tupleSelectImpl(f, tuple.first, Tuple<>());
            }
            else if constexpr (std::is_same_v<SelectedType, TupleSkipType>) {
                return Tuple<U...>();
            }
            else {
                return Tuple<U..., SelectedType>();
            }
        }
    }

    template<typename F, typename...T>
    auto tupleSelectType_(F f, const Tuple<T...>& tuple) {
        return tupleSelectImpl(f, tuple, Tuple<>{});
    }




    template<typename A, typename B>
    using tupleAppendFront = tupleCat<Tuple<A>, B>;

    template<typename A, typename B>
    using tupleAppendBack = tupleCat<B, Tuple<A>>;


    template<typename ... T>
    struct tupleFlattenImpl {
        using type = Tuple<>;
    };

    template<typename D,  typename...T>
    struct tupleFlattenImpl<D, T...> {
        using type = tupleAppendFront<D, typename tupleFlattenImpl<T...>::type>;
    };

    template<typename D>
    struct tupleFlattenImpl<D> {
        using type = Tuple<D>;
    };

    template<typename...D, typename...T>
    struct tupleFlattenImpl<Tuple<D...>, T...> {
        using type = tupleCat<typename tupleFlattenImpl<D...>::type, typename tupleFlattenImpl<T...>::type>;
    };

    template<typename...T>
    auto tupleFlatten_(const Tuple<T...>& tuple) {
        return typename tupleFlattenImpl<T...>::type{};
    }
}



template<typename F, typename T>
struct tupleSelectTypeHelp {
    typedef decltype(details::tupleSelectType_(std::declval<F>(), std::declval<T>())) type;
};

template<typename F, typename T>
using tupleSelectType = typename tupleSelectTypeHelp<F, T>::type;


template<typename T>
using tupleFlatten = decltype(details::tupleFlatten_(std::declval<T>()));

template<typename Target, bool Recurse = false>
struct TupleTypeSelector {
    template<typename T>
    auto operator () (const T& t) {
        if constexpr (IsTuple<T>::value) {
            if constexpr (Recurse) {
                return TupleStepInType{};
            }
            else {
                return TupleSkipType{};
            }
        }
        else {
            if constexpr (std::is_base_of_v<Target, T>) {
                return T{};
            }
            else {
                return TupleSkipType{};
            }
        }
      
    }
};

namespace details {
    template<typename F, typename...T, typename...U>
    auto tupleSelectImpl(F f, PackedTuple<T...> tuple, PackedTuple<U...> intermediate) {
        if constexpr(sizeof...(T) > 1) {
            return tupleSelectImpl(f, tuple.rest, PackedTuple<U..., decltype(f(tuple.first))>());
        }
        else {
            return PackedTuple<U..., decltype(f(tuple.first))>();
        }
    }

    template<typename F, typename...T>
    auto tupleSelectType_(F f, PackedTuple<T...> tuple) {
        return tupleSelectImpl(f, tuple, PackedTuple<>{});
    }
}

template<typename F, typename T>
using packedTupleSelectType = decltype(details::tupleSelectType_(std::declval<F>(), std::declval<T>()));









template<typename T, typename F, int... Is>
void forEach(T&& t, F f, std::index_sequence<Is...>) {
    auto l = { (f(std::get<Is>(t)), 0)... };
}

template<typename T, typename F, int... Is>
void forEachIndexed(T&& t, F f, std::index_sequence<Is...>) {
    auto l = { (f(std::get<Is>(t), Is), 0)... };
}

template<typename... Ts, typename F>
void forEachInTuple(std::tuple<Ts...> const& t, F f) {
    forEach(t, f, std::index_sequence_for<Ts...>());
}

template<typename... Ts, typename F>
void forEachInTupleIndexed(std::tuple<Ts...> const& t, F f) {
    forEachIndexed(t, f, std::index_sequence_for<Ts...>());
}

template<typename Functor, int ID = 0, typename ... Types>
bool executeOnIndex(std::tuple<Types...>& t, int id, Functor f) {
    if constexpr (ID < sizeof...(Types)) {
        if (id == ID) {
            f(std::get<ID>(t));
            return true;
        }
        else {
            return executeOnIndex<Functor, ID + 1, Types...>(t, id, f);
        }
    }
    else {
        return false;
    }
}

}



#endif // TUPLEUTILS_H
