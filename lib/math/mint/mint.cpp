// Modular integer. Templated on the modulus so it doesn't clash with the boilerplate's global `mod`.
//   using mint = modint<998244353>;  mint a = 5; a * a + 1; a.pow(e); a.inv(); a / b; cout << a; a.val
template <int MOD>
struct modint {
    static constexpr int mod = MOD;
    int val;

    modint(long long v = 0) {
        if (v < 0) v = v % MOD + MOD;
        if (v >= MOD) v %= MOD;
        val = v;
    }

    static int mod_inv(int a, int m = MOD) {
        long long g = m, r = a, x = 0, y = 1;
        while (r != 0) {
            long long q = g / r;
            g %= r; swap(g, r);
            x -= q * y; swap(x, y);
        }
        return x < 0 ? x + m : x;
    }

    explicit operator int() const { return val; }

    modint &operator+=(const modint &other) { val += other.val; if (val >= MOD) val -= MOD; return *this; }
    modint &operator-=(const modint &other) { val -= other.val; if (val < 0) val += MOD; return *this; }

    static unsigned fast_mod(uint64_t x, unsigned m = MOD) {
#if !defined(_WIN32) || defined(_WIN64)
        return x % m;
#endif
        unsigned x_high = x >> 32, x_low = (unsigned)x;
        unsigned quot, rem;
        asm("divl %4\n" : "=a"(quot), "=d"(rem) : "d"(x_high), "a"(x_low), "r"(m));
        return rem;
    }

    modint &operator*=(const modint &other) { val = fast_mod((uint64_t)val * other.val); return *this; }
    modint &operator/=(const modint &other) { return *this *= other.inv(); }

    friend modint operator+(const modint &a, const modint &b) { return modint(a) += b; }
    friend modint operator-(const modint &a, const modint &b) { return modint(a) -= b; }
    friend modint operator*(const modint &a, const modint &b) { return modint(a) *= b; }
    friend modint operator/(const modint &a, const modint &b) { return modint(a) /= b; }

    modint &operator++() { val = val == MOD - 1 ? 0 : val + 1; return *this; }
    modint &operator--() { val = val == 0 ? MOD - 1 : val - 1; return *this; }
    modint operator++(int32_t) { modint before = *this; ++*this; return before; }
    modint operator--(int32_t) { modint before = *this; --*this; return before; }
    modint operator-() const { return val == 0 ? 0 : MOD - val; }

    bool operator==(const modint &other) const { return val == other.val; }
    bool operator!=(const modint &other) const { return val != other.val; }

    modint inv() const { return mod_inv(val); }
    modint pow(long long p) const {
        assert(p >= 0);
        modint a = *this, result = 1;
        while (p > 0) {
            if (p & 1) result *= a;
            a *= a; p >>= 1;
        }
        return result;
    }

    friend ostream &operator<<(ostream &stream, const modint &m) { return stream << m.val; }
    friend istream &operator>>(istream &stream, modint &m) {
        long long x;
        stream >> x;
        m = modint(x);
        return stream;
    }
};
using mint = modint<998244353>;
