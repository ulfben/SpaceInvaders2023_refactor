#pragma once
#include <cassert>
#include <cstdint>
#include <limits>
#include <compare>
#include <chrono>
#include <utility>
// PCG32 - Permuted Congruential Generator
// Based on the minimal C implementation by Melissa O'Neill (https://www.pcg-random.org)
// C++ implementation by Ulf Benjaminsson
// see: https://github.com/ulfben/cpp_prngs/
//
// Satisfies 'UniformRandomBitGenerator' requirements - compatible with std::shuffle, 
// std::sample, and most std::*_distribution classes.

struct PCG32{
    using u64 = std::uint64_t;
    using u32 = std::uint32_t;
    using result_type = u32;
    using state_type = u64;
    static constexpr u64 PCG32_DEFAULT_SEED = 0x853c49e6748fea9bULL;
    static constexpr u64 PCG32_DEFAULT_STREAM = 0xda3e39cb94b95bdbULL;
    static constexpr u64 PCG32_MULT = 6364136223846793005ULL;

    constexpr PCG32() noexcept : state(PCG32_DEFAULT_SEED), inc(PCG32_DEFAULT_STREAM){}

    //seed and a sequence selection constant (a.k.a. stream id).
    constexpr PCG32(u64 seed_, u64 seq_ = 1) noexcept{
        seed(seed_, seq_);
    }

    constexpr void seed(u64 seed_, u64 seq_ = 1) noexcept{
        state = 0U;
        inc = (seq_ << 1u) | 1u; //ensure inc is odd
        next();
        state += seed_;
        next();
    }

    constexpr result_type next() noexcept{
        const auto oldstate = state;
        state = oldstate * PCG32_MULT + (inc | 1);
        const auto xorshifted = ((oldstate >> 18u) ^ oldstate) >> 27u;
        const auto rot = oldstate >> 59u;
        return narrow_cast<u32>((xorshifted >> rot) | (xorshifted << ((~rot + 1) & 31)));
    }

    constexpr result_type next(u32 bound) noexcept{
        //highly performant Lemire's algorithm (Debiased Integer Multiplication) after research by Melissa O'Neill
        // https://www.pcg-random.org/posts/bounded-rands.html        
        constexpr auto to64 = [](auto x) noexcept { return static_cast<u64>(x); };
        u64 result = to64(next()) * to64(bound);
        if(u32 lowbits = narrow_cast<u32>(result); lowbits < bound){
            u32 threshold = u32{0} - bound;   // Calculate 2^32 - bound for rejection sampling
            if(threshold >= bound){
                threshold -= bound;
                if(threshold >= bound)
                    threshold %= bound; 
            }
            while(lowbits < threshold){
                result = to64(next()) * to64(bound);;
                lowbits = narrow_cast<u32>(result);
            }
        }
        return result >> 32;
    }

    constexpr bool coinToss() noexcept{
        return next() & 1; //checks the least significant bit
    }

    //an overload for size_t because we often want to pull random indexes of containers
    constexpr result_type next(size_t bound) noexcept{
        if(bound <= max()){
            return next(narrow_cast<u32>(bound));
        }
        const size_t mid = bound / 2;
        if(const bool use_upper = coinToss()){
            return narrow_cast<result_type>(mid + next(bound - mid));
        }
        return narrow_cast<result_type>(next(mid));
    }

    //generate float in [0, 1)
    constexpr float normalized() noexcept{
        return 0x1.0p-32f * next(); //again, courtesy of Melissa O'Neill. See: https://www.pcg-random.org/posts/bounded-rands.html
        //0x1.0p-32 is a binary floating point constant for 2^-32 that we use to convert a random 
        // integer value in the range [0..2^32) into a float in the unit interval 0-1        
        //A less terrifying, but also less efficient, way to achieve the same goal  is:         
        //    return static_cast<float>(std::ldexp(next(), -32));
        //see https://mumble.net/~campbell/tmp/random_real.c for more info.        
    }

    //generate float in [-1, 1)    
    constexpr float unit_range() noexcept{
        return 2.0f * normalized() - 1.0f;
    }

    template<std::floating_point F>
    constexpr F between(F min, F max) noexcept{
        assert(min < max && "pcg32::between(min, max) called with inverted range.");
        return min + (max - min) * normalized();
    }

    template<std::integral I>
    constexpr I between(I min, I max) noexcept{
        using UI = std::make_unsigned_t<I>;
        static_assert(std::numeric_limits<UI>::max() <= max(),
            "PCG32::between() only supports types up to PCG32::result_type in size");
        assert(min < max && "pcg32::between(min, max) called with inverted range.");
        UI range = static_cast<UI>(max - min);
        return min + static_cast<I>(next(narrow_cast<result_type>(range)));
    }

    constexpr void set_state(u64 new_state, u64 new_inc) noexcept{
        state = new_state;
        inc = new_inc | 1u;  // ensure inc is odd
    }
    constexpr std::pair<u64, u64> get_state() const noexcept{
        return {state, inc};
    }
    static constexpr PCG32 from_state(u64 saved_state, u64 saved_inc) noexcept{
        PCG32 rng;
        rng.set_state(saved_state, saved_inc);
        return rng;
    }

    // operators and standard interface
    constexpr result_type operator()() noexcept{
        return next();
    }
    constexpr result_type operator()(u32 bound) noexcept{
        return next(bound);
    }
    static result_type constexpr min() noexcept{
        return 0;
    }
    static result_type constexpr max() noexcept{
        return std::numeric_limits<result_type>::max();
    }
    constexpr auto operator<=>(const PCG32& other) const noexcept = default;
private:
    u64 state{0}; // RNG state.  All values are possible.
    u64 inc{1}; // controls which RNG sequence (stream) is selected. Must *always* be odd.
};

// Utility functions for seeding Pseudo Random Number Generators
// for more examples, see https://github.com/ulfben/cpp_prngs/blob/main/seed.hpp
namespace seed {
    using u64 = std::uint64_t;
    using u32 = std::uint32_t;

    constexpr u64 splitmix64(u64 x) noexcept{
        x += 0x9e3779b97f4a7c15;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
        x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
        return x ^ (x >> 31);
    }

    inline u64 from_time() noexcept{
        const auto now = std::chrono::high_resolution_clock::now().time_since_epoch().count();
        return splitmix64(now);
    }

    constexpr u32 to_32(u64 seed) noexcept{
        return narrow_cast<u32>(seed ^ (seed >> 32)); //XOR-fold to preserve entropy
    }
}