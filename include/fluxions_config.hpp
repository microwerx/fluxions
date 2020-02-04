#ifndef FLUXIONS_CONFIG_HPP
#define FLUXIONS_CONFIG_HPP

#if _MSC_VER >= 1800
#define FLUXIONS_CPP11
#elif __cplusplus >= 201103L
#define FLUXIONS_CPP11
#endif

#if _MSC_VER >= 1900
#define FLUXIONS_CPP14
#elif __cplusplus >= 201402L
#define FLUXIONS_CPP11
#define FLUXIONS_CPP14
#endif

#if _MSC_VER >= 1900
#define FLUXIONS_CPP14
#elif __cplusplus >= 201703L
#define FLUXIONS_CPP17
#endif

#ifdef FLUXIONS_CPP11
//#define
//#define constexpr
#endif

#endif
