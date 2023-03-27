#pragma once
#include <concepts>

template<typename T>
concept arithmetic = (std::integral<T> || std::floating_point<T>) && !std::same_as<T, char>;

template<typename T>
concept integer = std::integral<T> && !std::same_as<T, char>;



template<typename T>
concept ArrayLike_1d = requires(const T & t, const int i) { t.size();  t[i]; t.begin(); t.end(); t.at(i);  requires arithmetic<std::decay_t<decltype(t[0])>>; };


template<typename T>
concept iArrayLike_1d = requires(const T & t, const int i) { t.size();  t[i]; t.begin(); t.end(); requires integer<std::decay_t<decltype(t[0])>>; };
// t.at(i) cannot be added here and running ExtractAlong() for some reason

