#ifndef DYNAMIC_BITSET_HPP
#define DYNAMIC_BITSET_HPP

#include <vector>
#include <iostream>
#include <stdexcept>
#include <functional>

class DynamicBitset {
private:
    std::vector<bool> bits;

public:
	DynamicBitset() = default;
    DynamicBitset(size_t size) : bits(size, false) {}

    bool get(size_t index) const {
        if (index >= bits.size()) throw std::out_of_range("Bit index out of range");
        return bits[index];
    }

    void set(size_t index, bool value) {
        if (index >= bits.size()) throw std::out_of_range("Bit index out of range");
        bits[index] = value;
    }

    size_t size() const {
        return bits.size();
    }

    void resize(size_t newSize) {
        bits.resize(newSize, false);
    }

    void clear() {
        std::fill(bits.begin(), bits.end(), false);
    }

    friend std::ostream& operator<<(std::ostream& os, const DynamicBitset& db) {
        for (bool bit : db.bits) {
            os << bit;
        }
        return os;
    }

    bool operator==(const DynamicBitset& other) const {
        return bits == other.bits;
    }

    DynamicBitset(const DynamicBitset&) = default;
    DynamicBitset& operator=(const DynamicBitset&) = default;
    DynamicBitset(DynamicBitset&&) noexcept = default;
    DynamicBitset& operator=(DynamicBitset&&) noexcept = default;

    struct Hash {
        std::size_t operator()(const DynamicBitset& db) const {
            std::size_t hash = 0;
            for (bool bit : db.bits) {
                hash = (hash << 1) ^ std::hash<bool>()(bit);
            }
            return hash;
        }
    };
};

namespace std {
    template <>
    struct hash<DynamicBitset> {
        std::size_t operator()(const DynamicBitset& db) const {
            return DynamicBitset::Hash()(db);
        }
    };
}

#endif