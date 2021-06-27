#pragma once

#include <assert.h>
#include <time.h>

#include <climits>

#include <iostream>

namespace SEFUtility::timespec
{
    constexpr long NANOSECONDS_PER_SECOND = 1000000000;
    constexpr long MILLISECOND_PER_SECOND = 1000;

    constexpr long NANOSECONDS_PER_MILLISECOND = 1000000;

    namespace internal
    {
        int64_t to_nanos( const struct timespec& value )
        {
            return ( value.tv_sec * NANOSECONDS_PER_SECOND ) + value.tv_nsec;
        }

        struct timespec from_nanos( int64_t     value )
        {
            int64_t     seconds = value / NANOSECONDS_PER_SECOND;

            return { seconds < LONG_MAX ? seconds : LONG_MAX, value % NANOSECONDS_PER_SECOND };
        }
    }  // namespace internal

    constexpr struct timespec operator""_s(unsigned long long secs)
    {
        assert(secs < LONG_MAX);

        struct timespec value
        {
            static_cast<__time_t>(secs), 0
        };

        return value;
    }

    constexpr struct timespec operator""_s(long double secs)
    {
        assert(secs < LONG_MAX);

        struct timespec value
        {
            static_cast<__time_t>(floor(secs)), static_cast<long>((secs - floor(secs)) * NANOSECONDS_PER_SECOND)
        };

        return value;
    }

    constexpr struct timespec operator""_ms(unsigned long long millisecs)
    {
        assert(millisecs < LONG_MAX);

        struct timespec value
        {
            static_cast<__time_t>(millisecs / MILLISECOND_PER_SECOND), static_cast<long>(( millisecs % MILLISECOND_PER_SECOND ) * NANOSECONDS_PER_MILLISECOND )
        };

        return value;
    }

    struct timespec operator+(struct timespec ts1, struct timespec ts2)
    {
        return internal::from_nanos( internal::to_nanos( ts1 ) + internal::to_nanos( ts2 ) );
    }

    struct timespec operator-(struct timespec ts1, struct timespec ts2)
    {
        int64_t result_nanos = internal::to_nanos( ts1 ) - internal::to_nanos( ts2 );

        result_nanos = std::max( result_nanos, static_cast<int64_t>(0) );

        return internal::from_nanos( result_nanos );
    }

    struct timespec operator*(struct timespec ts1, unsigned long scalar)
    {
        return internal::from_nanos( internal::to_nanos( ts1 ) * scalar );
    }

    struct timespec operator*(struct timespec ts1, double multiplier)
    {
        assert( multiplier >= 0  );

        return internal::from_nanos( internal::to_nanos( ts1 ) * multiplier );
    }

}  // namespace SEFUtility::timespec
