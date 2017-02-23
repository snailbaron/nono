#include <vector>
#include <stack>
#include <iostream>
#include <stdexcept>
#include <cstdint>
#include <ostream>
#include <limits>
#include <string>

template <class Base>
struct Range {
    constexpr Range(
            Base min = std::numeric_limits<Base>::min,
            Base max = std::numeric_limits<Base>::max)
        : min(min), max(max) {}

    const Base min;
    const Base max;
};

template <class Base>
constexpr bool inRange(Base value, Range<Base> range)
{
    return value >= range.min && value <= range.max;
}

template <const char* Name, class Base, Range<Base> Limits>
class Strict {
public:
    explicit Strict(Base value)
        : _value(value)
    {
        if (!inTypeRange(value)) {
            throw std::runtime_error(
                std::string(Name) + ": value " + std::to_string(value) +
                    " not in range");
        }
    }

private:
    static constexpr bool inTypeRange(Base value)
    {
        return value >= std::numeric_limits<Base>::min() &&
            value <= std::numeric_limits<Base>::max();
    }

    Base _value;
};


template <uint32_t Limit>
class Uint32 {
public:
    explicit Uint32(uint32_t value = 0)
        : _value(value)
    {
        if (_value > Limit) {
            throw std::runtime_error("Uint32: value too big");
        }
    }

    template <uint32_t L, uint32_t R>
    friend Uint32<L+R> operator+(Uint32<L> left, Uint32<R> right);

    friend std::ostream& operator<<(std::ostream& os, Uint32<Limit> &v)
    {
        os << "Uint32<" << Limit << ">(" << v._value << ")";
        return os;
    }

private:
    uint32_t _value;
};

template <uint32_t L, uint32_t R>
Uint32<L+R> operator+(Uint32<L> left, Uint32<R> right)
{
    return Uint32<L+R>(left._value + right._value);
}




int main()
{
    auto x = Uint32<10>(5u);
    auto y = Uint32<20>(15u);

    auto z = x + y;

    std::cout << z << std::endl;
}