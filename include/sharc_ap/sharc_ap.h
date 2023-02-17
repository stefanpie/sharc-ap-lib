
// Sharc_ap is an simple and compact C++ library for fixed point data types and math functions.
// It is designed to be used in HLS synthesis and is compatible with Xilinx Vitid HLS.


// Implement a fixed point data type with a specified number of integer and fractional bits.
// The number of total bits is specifdeb by W and the number of fractional bits is specified by I.

template<int W, int I, typename base_type=int32_t>
class sharc_ap_fixed {
public:
    // bit array to hold the data

    // restric base type to only be of type int32 or int64 or int16 or int8
    static_assert(std::is_same<base_type, int8_t>::value, "base_type must be int32_t or int64_t");
    static_assert(std::is_same<base_type, int16_t>::value, "base_type must be int32_t or int64_t");
    static_assert(std::is_same<base_type, int32_t>::value, "base_type must be int32_t or int64_t");
    static_assert(std::is_same<base_type, int64_t>::value, "base_type must be int32_t or int64_t");

    // restrict the number of bits to be between 1 and 64
    static_assert(W > 0, "W must be greater than 0");
    static_assert(W <= 64, "W must be less than or equal to 64");

    // restrict the number of fractional bits to be between 0 and W
    static_assert(I >= 0, "I must be greater than or equal to 0");
    static_assert(I <= W, "I must be less than or equal to W");

    // attributes
    static const int width = W;
    static const int i_width = I;
    static const int f_width = W - I;
    base_type value = 0;

public:

    // constructor form int value
    sharc_ap_fixed(const int init_value) {
        value = init_value << f_width;
    }
    // constructor form double value
    sharc_ap_fixed(const double init_value) {
        value = (base_type)(init_value * (1 << f_width));
    }
    // constructor form float value
    sharc_ap_fixed(const float init_value) {
        value = (base_type)(init_value * (1 << f_width));
    }
    // constructor form sharc_ap_fixed value
    sharc_ap_fixed(const sharc_ap_fixed<W, I, base_type> &init_value) {
        value = init_value.value;
    }

    // assignment operator from int
    sharc_ap_fixed<W, I, base_type> &operator=(const int rhs) {
        value = rhs << f_width;
        return *this;
    }
    // assignment operator from double
    sharc_ap_fixed<W, I, base_type> &operator=(const double rhs) {
        value = (base_type)(rhs * (1 << f_width));
        return *this;
    }
    // assignment operator from float
    sharc_ap_fixed<W, I, base_type> &operator=(const float rhs) {
        value = (base_type)(rhs * (1 << f_width));
        return *this;
    }
    // assignment operator from sharc_ap_fixed
    sharc_ap_fixed<W, I, base_type> &operator=(const sharc_ap_fixed<W, I, base_type> &rhs) {
        value = rhs.value;
        return *this;
    }

    // unary minus operator
    sharc_ap_fixed<W, I, base_type> operator-() const {
        sharc_ap_fixed<W, I, base_type> result;
        result.value = -value;
        return result;
    }

    // addition operator
    sharc_ap_fixed<W, I, base_type> operator+(const sharc_ap_fixed<W, I, base_type> &rhs) const {
        sharc_ap_fixed<W, I, base_type> result;
        result.value = value + rhs.value;
        return result;
    }
    // subtraction operator
    sharc_ap_fixed<W, I, base_type> operator-(const sharc_ap_fixed<W, I, base_type> &rhs) const {
        sharc_ap_fixed<W, I, base_type> result;
        result.value = value - rhs.value;
        return result;
    }
    // multiplication operator
    sharc_ap_fixed<W, I, base_type> operator*(const sharc_ap_fixed<W, I, base_type> &rhs) const {
        sharc_ap_fixed<W, I, base_type> result;
        result.value = (base_type)(((int64_t)value * (int64_t)rhs.value) >> f_width);
        return result;
    }
    // division operator
    sharc_ap_fixed<W, I, base_type> operator/(const sharc_ap_fixed<W, I, base_type> &rhs) const {
        sharc_ap_fixed<W, I, base_type> result;
        result.value = (base_type)(((int64_t)value << f_width) / (int64_t)rhs.value);
        return result;
    }

    // addition assignment operator
    sharc_ap_fixed<W, I, base_type> &operator+=(const sharc_ap_fixed<W, I, base_type> &rhs) {
        value += rhs.value;
        return *this;
    }
    // subtraction assignment operator
    sharc_ap_fixed<W, I, base_type> &operator-=(const sharc_ap_fixed<W, I, base_type> &rhs) {
        value -= rhs.value;
        return *this;
    }
    // multiplication assignment operator
    sharc_ap_fixed<W, I, base_type> &operator*=(const sharc_ap_fixed<W, I, base_type> &rhs) {
        value = (base_type)(((int64_t)value * (int64_t)rhs.value) >> f_width);
        return *this;
    }
    // division assignment operator
    sharc_ap_fixed<W, I, base_type> &operator/=(const sharc_ap_fixed<W, I, base_type> &rhs) {
        value = (base_type)(((int64_t)value << f_width) / (int64_t)rhs.value);
        return *this;
    }

    // equality operator
    bool operator==(const sharc_ap_fixed<W, I, base_type> &rhs) const {
        return value == rhs.value;
    }
    // inequality operator
    bool operator!=(const sharc_ap_fixed<W, I, base_type> &rhs) const {
        return value != rhs.value;
    }
    // less than operator
    bool operator<(const sharc_ap_fixed<W, I, base_type> &rhs) const {
        return value < rhs.value;
    }
    // greater than operator
    bool operator>(const sharc_ap_fixed<W, I, base_type> &rhs) const {
        return value > rhs.value;
    }
    // less than or equal to operator
    bool operator<=(const sharc_ap_fixed<W, I, base_type> &rhs) const {
        return value <= rhs.value;
    }
    // greater than or equal to operator
    bool operator>=(const sharc_ap_fixed<W, I, base_type> &rhs) const {
        return value >= rhs.value;
    }

    // conversion to double
    operator double() const {
        return (double)value / (1 << f_width);
    }
    // conversion to float
    operator float() const {
        return (float)value / (1 << f_width);
    }
    // conversion to int
    operator int() const {
        return value >> f_width;
    }

    // print to stream
    void print(std::ostream &os) const {
        os << (double)*this;
    }
};

// implement pow
template<int W, int I, typename base_type>
sharc_ap_fixed<W, I, base_type> pow(const sharc_ap_fixed<W, I, base_type> &base, const sharc_ap_fixed<W, I, base_type> &exponent) {
    sharc_ap_fixed<W, I, base_type> result;
    
