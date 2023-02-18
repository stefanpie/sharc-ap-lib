#pragma once

#include <cstdint>
#include <functional>
#include <limits>
#include <type_traits>

#define static_message(msg) _Pragma("message(\"" msg "\")")


namespace sharc_ap_lib
{

    // sharc_ap_fixed is an simple and compact C++ class that implements fixed-point data types.
    // It uses fixed sized integers to store the data and specifies the number of fractional bits.
    template <typename base_type = int32_t, typename intermediate_type = int64_t, const uint fraction_bits = 16>
    class sharc_ap_fixed
    {
        static_assert(std::is_integral<base_type>::value, "base_type must be an integral type");
        static_assert(fraction_bits > 0, "fraction_bits must be greater than zero");
        static_assert(fraction_bits <= sizeof(base_type) * 8 - 1, "base_type must at least be able to contain entire fraction, with space for at least one integral bit");
        static_assert(sizeof(intermediate_type) >= sizeof(base_type), "intermediate_type must be larger than base_type");
        static_assert(std::is_signed<intermediate_type>::value == std::is_signed<base_type>::value, "intermediate_type must have same signedness as base_type");        

    private:

    public:
        static constexpr uint FRACTION_BITS = fraction_bits;
        static constexpr base_type FRACTION_MULT = base_type(1) << fraction_bits;
        
        base_type v = 0;

        sharc_ap_fixed() = default;

        // constructor from integral types
        template <typename T, std::enable_if_t<std::is_integral<T>::value || std::is_floating_point<T>::value, int> = 0>
        constexpr sharc_ap_fixed(T value)
        {
            const bool is_integral = std::is_integral<T>::value;
            const bool is_floating_point = std::is_floating_point<T>::value;

            const bool is_valid_type = is_integral || is_floating_point;
            static_assert(is_valid_type, "sharc_ap_fixed can only be constructed from integral or floating point types");

            v = static_cast<base_type>(value * FRACTION_MULT);
        }

        // constructor from sharc_ap_fixed
        template <typename B, typename I, uint D>
        constexpr sharc_ap_fixed(const sharc_ap_fixed<B, I, D>& other)
        {   
            if (D == fraction_bits)
            {
                v = other.v;
            }
            else if(D < fraction_bits)
            {
                v = other.v << (fraction_bits - D);
            }
            else
            {
                v = other.v >> (D - fraction_bits);
            }
        }

        // cast operators
        template <typename T, std::enable_if_t<std::is_floating_point<T>::value, int> = 0>
        operator T() const
        {
            return static_cast<T>(v) / FRACTION_MULT;
        }

        template <typename T, std::enable_if_t<std::is_integral<T>::value, int> = 0>
        operator T() const
        {
            return static_cast<T>(v >> fraction_bits);
        }

        template <typename B, typename I, uint D>
        operator sharc_ap_fixed<B, I, D>() const
        {
            return sharc_ap_fixed<B, I, D>(*this);
        }

        // add
        sharc_ap_fixed& operator+=(const sharc_ap_fixed& other)
        {
            v += other.v;
            return *this;
        }

        // subtract
        sharc_ap_fixed& operator-=(const sharc_ap_fixed& other)
        {
            v -= other.v;
            return *this;
        }

        // multiply
        sharc_ap_fixed& operator*=(const sharc_ap_fixed& other)
        {   
            v = static_cast<base_type>((static_cast<intermediate_type>(v) * other.v) / FRACTION_MULT);
            return *this;
        }

        // divide
        sharc_ap_fixed& operator/=(const sharc_ap_fixed& other)
        {
            v = static_cast<base_type>((static_cast<intermediate_type>(v) * FRACTION_MULT) / other.v);
            return *this;
        }

    };

    // implement binary operators
    template <typename B, typename I, uint D>
    sharc_ap_fixed<B, I, D> operator+(const sharc_ap_fixed<B, I, D>& a, const sharc_ap_fixed<B, I, D>& b)
    {
        sharc_ap_fixed<B, I, D> result = a;
        result += b;
        return result;
    }

    template <typename B, typename I, uint D>
    sharc_ap_fixed<B, I, D> operator-(const sharc_ap_fixed<B, I, D>& a, const sharc_ap_fixed<B, I, D>& b)
    {
        sharc_ap_fixed<B, I, D> result = a;
        result -= b;
        return result;
    }

    template <typename B, typename I, uint D>
    sharc_ap_fixed<B, I, D> operator*(const sharc_ap_fixed<B, I, D>& a, const sharc_ap_fixed<B, I, D>& b)
    {
        sharc_ap_fixed<B, I, D> result = a;
        result *= b;
        return result;
    }

    template <typename B, typename I, uint D>
    sharc_ap_fixed<B, I, D> operator/(const sharc_ap_fixed<B, I, D>& a, const sharc_ap_fixed<B, I, D>& b)
    {
        sharc_ap_fixed<B, I, D> result = a;
        result /= b;
        return result;
    }

    // implement comparison operators
    template <typename B, typename I, uint D>
    bool operator==(const sharc_ap_fixed<B, I, D>& a, const sharc_ap_fixed<B, I, D>& b)
    {
        return a.v == b.v;
    }

    template <typename B, typename I, uint D>
    bool operator!=(const sharc_ap_fixed<B, I, D>& a, const sharc_ap_fixed<B, I, D>& b)
    {
        return a.v != b.v;
    }

    template <typename B, typename I, uint D>
    bool operator<(const sharc_ap_fixed<B, I, D>& a, const sharc_ap_fixed<B, I, D>& b)
    {
        return a.v < b.v;
    }

    template <typename B, typename I, uint D>
    bool operator<=(const sharc_ap_fixed<B, I, D>& a, const sharc_ap_fixed<B, I, D>& b)
    {
        return a.v <= b.v;
    }

    template <typename B, typename I, uint D>
    bool operator>(const sharc_ap_fixed<B, I, D>& a, const sharc_ap_fixed<B, I, D>& b)
    {
        return a.v > b.v;
    }

    template <typename B, typename I, uint D>
    bool operator>=(const sharc_ap_fixed<B, I, D>& a, const sharc_ap_fixed<B, I, D>& b)
    {
        return a.v >= b.v;
    }
    


using sharc_ap_fixed_16_8 = sharc_ap_fixed<int16_t, int32_t, 8>;
using sharc_ap_fixed_16_12 = sharc_ap_fixed<int16_t, int32_t, 12>;
using sharc_ap_fixed_32_16 = sharc_ap_fixed<int32_t, int64_t, 16>;
using sharc_ap_fixed_32_24 = sharc_ap_fixed<int32_t, int64_t, 24>;
using sharc_ap_fixed_64_32 = sharc_ap_fixed<int64_t, int64_t, 32>;
using sharc_ap_fixed_64_48 = sharc_ap_fixed<int64_t, int64_t, 48>;

}