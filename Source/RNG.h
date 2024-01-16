#pragma once
#include <array>
#include <cassert>
#include <bitset>
#include <chrono> //for entropy in createSeeds
#include <concepts>
#include <cstdint>
#include <limits>
#include <span>
#include <thread> //for entropy in createSeeds
#include <ctime> //for entropy in createSeeds
#include <type_traits>

// The "xoshiro256** 1.0" generator.
// public interface, rejection sampling and seeding utilities by Ulf Benjaminsson (2023)
// https://ulfbenjaminsson.com/
// Based on C++ port by Arthur O'Dwyer (2021).
// https://quuxplusone.github.io/blog/2021/11/23/xoshiro/
// of the C version by David Blackman and Sebastiano Vigna (2018),
// https://prng.di.unimi.it/xoshiro256starstar.c

class RNG{
public:
    using u64 = std::uint_fast64_t;
    static constexpr std::size_t SEED_COUNT = 4;
    using State = std::array<u64, SEED_COUNT>;
    using Span = std::span<const u64, SEED_COUNT>;
    static constexpr auto USE_REJECTION_SAMPLING = false;
    // When enabled, all ranged functions will use rejection sampling to ensure a more uniform 
    // distribution of random numbers across large ranges. The concern with large ranges is that
    // methods like modulo reduction (randNum % range) might not evenly distribute numbers 
    // across the range, leading to bias. As a rule of thumb, if the range is more than half of 
    // the maximum output (e.g., more than 2^63), you might start to see the benefits of using 
    // rejection sampling to ensure uniformity.

    constexpr explicit RNG(u64 seed) noexcept{
        s[0] = splitmix64(seed);
        seed += 0x9E3779B97F4A7C15uLL;
        s[1] = splitmix64(seed);
        seed += 0x7F4A7C15uLL;
        s[2] = splitmix64(seed);
        seed += 0x9E3779B9uLL;
        s[3] = splitmix64(seed);
    }
    constexpr explicit RNG(double seed) noexcept
        : RNG(static_cast<u64>(seed)){}

    constexpr explicit RNG(Span seeds) noexcept{
        std::ranges::copy(seeds, s.begin());
    }

    static constexpr u64 max() noexcept{
        return std::numeric_limits<u64>::max();
    }

    constexpr u64 next() noexcept{
        return nextU64();
    }

    constexpr bool coinToss() noexcept{
        return next() & 1; //checks the least significant bit
    }

    template<std::floating_point Real>
    constexpr Real normalized() noexcept{
        return static_cast<Real>(next()) / static_cast<Real>(max());
    }

    template<std::floating_point Real>
    constexpr Real inRange(Real range) noexcept{
        return normalized<Real>() * range;
    }

    template<std::floating_point Real>
    constexpr Real inRange(Real from, Real to) noexcept{
        assert(from < to && "RNG: inverted range.");
        return from + normalized<Real>() * (to - from);
    }

    template<std::integral T>
    constexpr T inRange(T from, T to) noexcept{
        assert(from < to && "RNG: inverted range.");
        using UT = std::make_unsigned_t<T>;
        UT range = static_cast<UT>(to - from);
        return static_cast<T>(inRange(range)) + from;
    }

    template<std::integral T>
    constexpr T inRange(T range) noexcept{
        using UT = std::make_unsigned_t<T>;
        UT num = static_cast<UT>(inRange(static_cast<u64>(std::abs(range))));
        return (range < 0) ? -static_cast<T>(num) : static_cast<T>(num);
    }

    constexpr u64 inRange(u64 range) noexcept{
        if(range == 0){
            assert(false && "RNG::inRange called with empty range!");
            return 0;
        }
        if constexpr(USE_REJECTION_SAMPLING){
            return uniformRandom(range);
        }
        return next() / (max() / range);
    }

    constexpr Span state() const{
        return {s};
    }

    /* the jump() function is equivalent to 2^128 calls to next();
    it can be used to generate 2^128 non-overlapping subsequences
    for parallel computations. */
    constexpr void jump() noexcept{
        constexpr std::array<std::bitset<64>, SEED_COUNT> JUMP{
            0x180ec6d33cfd0abaULL, 0xd5a61266f0c9392cULL, 0xa9582618e03fc9aaULL, 0x39abdc4529b1661cULL
        };
        State temp{0};
        for(const auto& bits : JUMP){
            for(std::size_t b = 0; b < 64; ++b){
                if(bits.test(b)){
                    temp[0] ^= s[0];
                    temp[1] ^= s[1];
                    temp[2] ^= s[2];
                    temp[3] ^= s[3];
                }
                next();
            }
        }
        s = temp;
    }

    //convenience function to allow hashing from const values.
    static constexpr u64 splitmix64_hash(u64 x) noexcept{        
        return splitmix64(x);
    }
    
private:
    State s{};

    constexpr u64 nextU64() noexcept{
        const u64 result = rotl(s[1] * 5, 7) * 9;
        const u64 t = s[1] << 17;
        s[2] ^= s[0];
        s[3] ^= s[1];
        s[1] ^= s[2];
        s[0] ^= s[3];
        s[2] ^= t;
        s[3] = rotl(s[3], 45);
        return result;
    }

    static constexpr u64 rotl(u64 x, int k) noexcept{
        return (x << k) | (x >> (64 - k));
    }

    static constexpr u64 splitmix64(u64& x) noexcept{
        u64 z = (x += 0x9e3779b97f4a7c15uLL);
        z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9uLL;
        z = (z ^ (z >> 27)) * 0x94d049bb133111ebuLL;
        return z ^ (z >> 31);
    }

    //uses rejection sampling to ensure fair scaling. Costlier than inRange(u64)
    constexpr u64 uniformRandom(u64 range) noexcept{
        if(range == 0){
            assert(false && "RNG::uniformRandom called with empty range!");
            return 0;
        }
        const u64 rangeLimit = max() - range + 1;
        u64 n = next();
        while((n - (n % range)) >= rangeLimit){
            n = next();
        }
        return n % range;
    }
};

//Some strategies for seeding the full 256-bit state of xoshiro256.
// createSeeds uses date, time-since-launch, CPU time, thread ID, and a memory address as sources of entropy.
// each value is hashed using splitmix64. 
static typename RNG::State createSeeds() noexcept{
    using u64 = RNG::u64;
    using namespace std::chrono;
    const auto current_date = static_cast<u64>(system_clock::now().time_since_epoch().count());
    const auto uptime = static_cast<u64>(high_resolution_clock::now().time_since_epoch().count());
    const auto cpu_time = static_cast<u64>(std::clock());    
    const auto mixed_time = static_cast<u64>((uptime << 1) ^ current_date);
    const auto thread_id = static_cast<u64>(std::hash<std::thread::id>{}(std::this_thread::get_id()));    
    const int local{};
    const auto local_addr = static_cast<u64>(reinterpret_cast<std::uintptr_t>(&local));
    return {
        RNG::splitmix64_hash(mixed_time),
        RNG::splitmix64_hash(thread_id),
        RNG::splitmix64_hash(cpu_time),
        RNG::splitmix64_hash(local_addr),
    };
}

/* createSeedsB *additionally* mix those sources with entropy from std::random_device. This is likely significantly slow(er).
#include <random>
static typename RNG::State createSeedsB() {
    using u64 = RNG::u64;
    std::random_device rd;
    auto seeds = createSeeds();
    seeds[0] = static_cast<u64>(rd()) ^ seeds[0];
    seeds[1] = static_cast<u64>(rd()) ^ (seeds[0] << 1);
    seeds[2] = static_cast<u64>(rd()) ^ (seeds[1] << 1);
    seeds[3] = static_cast<u64>(rd()) ^ (seeds[2] << 1);
    return seeds;
}*/