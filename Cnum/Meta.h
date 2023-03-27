#pragma once
#include <concepts>

template<typename T>
concept arithmetic = (std::integral<T> || std::floating_point<T>) && !std::same_as<T, char>;


template<typename T>
concept arrayLike_1d = requires(const T & t, const int i) { t.size();  t[i]; t.begin(); t.end();  requires arithmetic<std::decay_t<decltype(t[0])>>; };



