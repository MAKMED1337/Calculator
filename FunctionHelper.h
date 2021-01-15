#pragma once
#include "Functions/IFunction.h"
#include "Functions/Constant.cpp"

#include <memory>
#include <optional>
#include <iomanip>
#include <sstream>

template <class T = type>
static std::optional<T> is_number(std::string_view s)
{
    std::string str(s);
    std::istringstream ss(str, std::ios::in);
    T res;

    if ((ss >> res) && ss.eof())
        return res;

    return std::nullopt;
}

static cfunc_ptr get_const(type t)
{
    return std::make_shared<Constant>(t);
}

template <class T>
static T log(T const& base, T const& value)
{
    return std::log(value) / std::log(base);
}

constexpr std::string_view WHITESPACE = " \n\r\t\f\v";

static std::string_view ltrim(std::string_view s, std::string_view whitespace = WHITESPACE)
{
    size_t start = s.find_first_not_of(whitespace);
    return (start != std::string::npos) ? s.substr(start) : "";
}

static std::string_view rtrim(std::string_view s, std::string_view whitespace = WHITESPACE)
{
    size_t end = s.find_last_not_of(whitespace);
    return (end != std::string::npos) ?  s.substr(0, end + 1) : "";
}

static std::string_view trim(std::string_view s, std::string_view whitespace = WHITESPACE)
{
    return rtrim(ltrim(s, whitespace), whitespace);
}