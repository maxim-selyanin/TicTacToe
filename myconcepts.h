#ifndef TICTACTOE_MYCONCEPTS_H
#define TICTACTOE_MYCONCEPTS_H

//#define ENABLE_CONCEPTS

#ifdef ENABLE_CONCEPTS

#include <type_traits>

template<typename T, typename U>
concept SameHelper = std::is_same_v<T, U>;

template<typename T, typename U>
concept same_as = SameHelper<T, U> && SameHelper<U, T>;

template<typename T>
concept IterableHelper = requires (T &t) {
    { t.begin() } -> same_as<typename T::iterator>;
    { t.end() } -> same_as<typename T::iterator>;
};

template<typename T>
concept Iterable = std::is_array_v<T> || IterableHelper<T>;

template<typename T>
concept Hashable = requires (const T &h) {
    typename T::HashKeyType;
    { h.getHash() } -> same_as<typename T::HashKeyType>;
};

template<typename T>
concept PlayingField = requires (T &field) {
    typename T::MoveDescriptorType;
    typename T::ValueType;
    typename T::ResultType;

    { field.makeMove(typename T::MoveDescriptorType{}) } -> same_as<bool>;
    { field.undoMove(typename T::MoveDescriptorType{}) } -> same_as<bool>;
    { T::noMove() } -> same_as<typename T::MoveDescriptorType>;
    { field.result() } -> same_as<typename T::ResultType>;
    { field.minimalValue() } -> same_as<typename T::ValueType>;
    { field.maximalValue() } -> same_as<typename T::ValueType>;
    { field.heuristicValue() } -> same_as<typename T::ValueType>;
    { field.getAvailableMoves(bool{}) } -> Iterable;
    std::is_same_v<
            typename decltype(field.getAvailableMoves(bool{}))::value_type,
            typename T::MoveDescriptorType
    > == true;
};

template<typename T>
concept HashableField = PlayingField<T> && Hashable<T>;

#else

#define HashableField typename

#endif

#endif //TICTACTOE_MYCONCEPTS_H
