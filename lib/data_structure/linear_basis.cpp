// @aliases LinearBasis
// XOR linear basis over 60-bit values.
//   linear_basis b; b.insert(x); b.contains(x); b.max_xor(val); b.min_xor(); b.size(); b1 + b2
class linear_basis {
private:
    using i64 = int64_t;
    static constexpr int max_bits = 60;
    array<i64, max_bits> basis_{};
    int sz_{0};

public:
    linear_basis() = default;

    bool insert(i64 x) {
        for (int i = max_bits - 1; i >= 0; i--) {
            if (!((x >> i) & 1)) continue;

            if (!basis_[i]) {
                basis_[i] = x;
                sz_++;
                return true;
            }

            x ^= basis_[i];
        }
        return false;
    }

    i64 max_xor(i64 val = 0) const {
        i64 res = val;
        for (int i = max_bits - 1; i >= 0; i--)
            if ((res ^ basis_[i]) > res) res ^= basis_[i];
        return res;
    }

    i64 min_xor() const {
        for (int i = 0; i < max_bits; i++)
            if (basis_[i]) return basis_[i];
        return 0;
    }

    bool contains(i64 k) const {
        for (int i = max_bits - 1; i >= 0; i--) {
            if (!((k >> i) & 1)) continue;
            if (!basis_[i]) return false;
            k ^= basis_[i];
        }
        return true;
    }

    linear_basis &operator+=(const linear_basis &other) {
        for (int i = 0; i < max_bits; ++i)
            if (other.basis_[i]) insert(other.basis_[i]);
        return *this;
    }

    friend linear_basis operator+(linear_basis lhs, const linear_basis &rhs) {
        lhs += rhs;
        return lhs;
    }

    int size() const { return sz_; }
};
