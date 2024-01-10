#pragma once
#include <array>
#include <algorithm>
#include <cassert>
#include <concepts>
#include <limits>
#include <span>
#include <type_traits>
#include <chrono> //for entropy in createSeeds
#include <ctime> //for entropy in createSeeds
#include <thread> //for entropy in createSeeds
// The "xoshiro256** 1.0" generator.
// public interface, rejection sampling and seeding utilities by Ulf Benjaminsson (2023)
// https://ulfbenjaminsson.com/
// Based on C++ port by Arthur O'Dwyer (2021).
// https://quuxplusone.github.io/blog/2021/11/23/xoshiro/
// of the C version by David Blackman and Sebastiano Vigna (2018),
// https://prng.di.unimi.it/xoshiro256starstar.c
static_assert(sizeof(long long) == 8, "64-bit machines only");

class RNG{
public:
    using u64 = unsigned long long;
    static constexpr std::size_t SEED_COUNT = 4;
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

    constexpr explicit RNG(std::span<const u64, SEED_COUNT> seeds) noexcept{
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

    constexpr std::span<const u64, SEED_COUNT> state() const{
        return {s};
    }

private:
    std::array<u64, SEED_COUNT> s{};

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
static std::array<RNG::u64, RNG::SEED_COUNT> createSeeds() {
    using u64 = RNG::u64;
    using namespace std::chrono;
    auto current_date = system_clock::now().time_since_epoch().count();
    auto cpu_time = static_cast<u64>(std::clock());
    auto uptime = high_resolution_clock::now().time_since_epoch().count();
    auto mixed_time = (static_cast<u64>(uptime) << 1) ^ static_cast<u64>(current_date);
    int local{};
    return {
        std::hash<u64>()(mixed_time),
        std::hash<std::thread::id>()(std::this_thread::get_id()),
        std::hash<u64>()(cpu_time),
        std::hash<std::uintptr_t>()(reinterpret_cast<std::uintptr_t>(&local)),
    };
}

/* createSeedsB *additionally* mix those sources with entropy from std::random_device. This is likely significantly slow(er).
#include <random>
static std::array<RNG::u64, RNG::SEED_COUNT> createSeedsB() {
    using u64 = RNG::u64;
    std::random_device rd;
    auto seeds = createSeeds();
    seeds[0] = static_cast<u64>(rd()) ^ seeds[0];
    seeds[1] = static_cast<u64>(rd()) ^ (seeds[0] << 1);
    seeds[2] = static_cast<u64>(rd()) ^ (seeds[1] << 1);
    seeds[3] = static_cast<u64>(rd()) ^ (seeds[2] << 1);
    return seeds;
}*/