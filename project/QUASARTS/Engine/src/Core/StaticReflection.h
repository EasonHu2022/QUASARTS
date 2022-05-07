#pragma once

#ifndef STATIC_REFLECTION
#define STATIC_REFLECTION

#define BRACKET_L (
#define BRACKET_R )

#include "ReflLoop.h"
#include "Logger/LogModule.h"
#define COUNT_ARGS(...) ARG_COUNTS(EXPAND(__VA_ARGS__))
#define EXPAND(...) _, __VA_ARGS__

#define COPY(x) x
#define ARG_COUNTS(...) COPY(RETURN_COUNT(__VA_ARGS__, 100, 99, 98, 97, 96, 95, 94, 93, 92, 91, 90, 89, 88, \
        87, 86, 85, 84, 83, 82, 81, 80, 79, 78, 77, 76, 75, 74, 73, 72, \
        71, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56, \
        55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, \
        39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, \
        23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, \
        6, 5, 4, 3, 2, 1, 0))

#define COUNT_LIST 100, 99, 98, 97, 96, 95, 94, 93, 92, 91, 90, 89, 88, \
        87, 86, 85, 84, 83, 82, 81, 80, 79, 78, 77, 76, 75, 74, 73, 72, \
        71, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56, \
        55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, \
        39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, \
        23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, \
        6, 5, 4, 3, 2, 1, 0 \

#define RETURN_COUNT(x100, x99, x98, x97, x96, x95, x94, x93, x92, x91, x90, x89, x88, \
        x87, x86, x85, x84, x83, x82, x81, x80, x79, x78, x77, x76, x75, x74, x73, x72, \
        x71, x70, x69, x68, x67, x66, x65, x64, x63, x62, x61, x60, x59, x58, x57, x56, \
        x55, x54, x53, x52, x51, x50, x49, x48, x47, x46, x45, x44, x43, x42, x41, x40, \
        x39, x38, x37, x36, x35, x34, x33, x32, x31, x30, x29, x28, x27, x26, x25, x24, \
        x23, x22, x21, x20, x19, x18, x17, x16, x15, x14, x13, x12, x11, x10, x9, x8, x7, \
        x6, x5, x4, x3, x2, x1, x0, count, ...) count \

#define PARE(...) __VA_ARGS__
#define PAIR(x) PARE x

#define EAT(...)
#define STRIP(x) EAT x

#define STRING(x) #x
#define INS(x) STRING(x)

#define PROPS_EACH(i, arg) \
PAIR(arg); \
template <typename T> \
struct INDEX<T, i> \
{ \
	T& obj;\
	INDEX(T& _obj) : obj(_obj) {} \
	auto value()->decltype(auto) \
	{ \
		return obj.STRIP(arg); \
	} \
    auto name()->decltype(auto) \
	{ \
		return INS BRACKET_L STRIP(arg) BRACKET_R ; \
	} \
};

#define DEFINE_PROPERTIES(...) \
public: \
    template <typename, size_t> \
    struct INDEX; \
    static constexpr size_t props_count = COUNT_ARGS(__VA_ARGS__); \
    PASTE(LOOP_, COUNT_ARGS(__VA_ARGS__)) BRACKET_L PROPS_EACH, 0, __VA_ARGS__ BRACKET_R \
private: \

template<typename T, typename Func, size_t... Is>
inline constexpr void forEach(T&& obj, Func&& func, std::index_sequence<Is...>) {
    (void(func(std::decay_t<T>::template INDEX<T, Is>(obj).name(),
        std::decay_t<T>::template INDEX<T, Is>(obj).value())), ...);
}

template<typename T, typename Func>
inline constexpr void foreach(T&& obj, Func&& func) {
    forEach(obj, func,
        std::make_index_sequence<std::decay_t<T>::props_count>{});
}

template<typename T>
void dump(T&& obj, const char* name = "") {

    if constexpr (std::is_class_v<std::decay_t<T>>)
    {
        foreach(obj, [](auto&& name, auto&& value)
        {
            QDEBUG(name);
            QDEBUG(std::to_string(value.x));           
        });
    }
    else
    {
       
    }
}

#endif // STATIC_REFLECTION
