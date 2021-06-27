#pragma once

#include <assert.h>
#include <time.h>

#include <climits>

#include <iostream>

namespace SEFUtility::timespec
{
    constexpr long ONE_SECOND_IN_NANOSECONDS = 1000000000;

    namespace internal
    {
        void handle_overflow(struct timespec& value)
        {
            if (value.tv_nsec >= ONE_SECOND_IN_NANOSECONDS)
            {
                value.tv_sec += value.tv_nsec / ONE_SECOND_IN_NANOSECONDS;
                value.tv_nsec = value.tv_nsec % ONE_SECOND_IN_NANOSECONDS;
            }
        }

        void handle_underflow(struct timespec& value)
        {
            if(( value.tv_sec >= 0 ) && ( value.tv_nsec >= 0 ))
            {
                return;
            }
            
            if(( value.tv_sec <= 0 ) && ( value.tv_nsec <= 0 ))
            {
                value = {0,0};
                return;
            }

            if (value.tv_nsec < 0)
            {
                value.tv_sec += ( value.tv_nsec / ONE_SECOND_IN_NANOSECONDS ) - 1;
                value.tv_nsec = ONE_SECOND_IN_NANOSECONDS + (value.tv_nsec % ONE_SECOND_IN_NANOSECONDS);
            }
            else
            {
                value.tv_sec += ( value.tv_nsec / ONE_SECOND_IN_NANOSECONDS );
                value.tv_nsec = value.tv_nsec % ONE_SECOND_IN_NANOSECONDS;
            }

            if ((value.tv_sec < 0) || ((value.tv_sec == 0) && (value.tv_nsec < 0 )))
            {
                value = {0, 0};
            }
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
            static_cast<__time_t>(floor(secs)), static_cast<long>((secs - floor(secs)) * ONE_SECOND_IN_NANOSECONDS)
        };

        return value;
    }

    struct timespec operator+(struct timespec ts1, struct timespec ts2)
    {
        struct timespec value
        {
            ts1.tv_sec + ts2.tv_sec, ts1.tv_nsec + ts2.tv_nsec
        };

        internal::handle_overflow(value);

        return value;
    }

    struct timespec operator-(struct timespec ts1, struct timespec ts2)
    {
        struct timespec value
        {
            ts1.tv_sec - ts2.tv_sec, ts1.tv_nsec - ts2.tv_nsec
        };

        internal::handle_underflow(value);

        return value;
    }

    struct timespec operator*(struct timespec ts1, unsigned long scalar)
    {
        struct timespec value
        {
            static_cast<__time_t>(ts1.tv_sec * scalar) , static_cast<long>( ts1.tv_nsec * scalar )
        };

        internal::handle_overflow( value );

        return value;
    }

    struct timespec operator*(struct timespec ts1, double multiplier)
    {
        assert( multiplier >= 0  );

        struct timespec value
        {
            static_cast<__time_t>(ts1.tv_sec * multiplier) , static_cast<long>(ts1.tv_nsec * multiplier )
        };

        internal::handle_overflow( value );

        return value;
    }

}  // namespace SEFUtility::timespec
