
#include <catch2/catch_all.hpp>

#include "TimespecUtilities.hpp"

#if __INTELLISENSE__
#pragma diag_suppress 2486
#endif

using namespace SEFUtility::timespec;

//  Test cases follow

TEST_CASE("TimeSpec tests", "[basic]")
{
    SECTION("Overflow and Underflow")
    {
        {
            struct timespec over_by_one_nano = {0, ONE_SECOND_IN_NANOSECONDS + 1};

            internal::handle_overflow(over_by_one_nano);

            REQUIRE(over_by_one_nano.tv_sec == 1);
            REQUIRE(over_by_one_nano.tv_nsec == 1);
        }

        {
            struct timespec under_by_one_nano = {0, ONE_SECOND_IN_NANOSECONDS - 1};

            internal::handle_overflow(under_by_one_nano);

            REQUIRE(under_by_one_nano.tv_sec == 0);
            REQUIRE(under_by_one_nano.tv_nsec == 999999999);
        }

        {
            struct timespec one_second = {0, ONE_SECOND_IN_NANOSECONDS};

            internal::handle_overflow(one_second);

            REQUIRE(one_second.tv_sec == 1);
            REQUIRE(one_second.tv_nsec == 0);
        }

        {
            struct timespec one_second = {0, (ONE_SECOND_IN_NANOSECONDS * 7) + 5};

            internal::handle_overflow(one_second);

            REQUIRE(one_second.tv_sec == 7);
            REQUIRE(one_second.tv_nsec == 5);
        }

        {
            struct timespec negative_one_nano = {0, -1};

            internal::handle_underflow(negative_one_nano);

            REQUIRE(negative_one_nano.tv_sec == 0);
            REQUIRE(negative_one_nano.tv_nsec == 0);
        }

        {
            struct timespec negative_one_nano = {1, -1};

            internal::handle_underflow(negative_one_nano);

            REQUIRE(negative_one_nano.tv_sec == 0);
            REQUIRE(negative_one_nano.tv_nsec == 999999999);
        }

        {
            struct timespec negative_one_nano = {1, -1000000001};

            internal::handle_underflow(negative_one_nano);

            REQUIRE(negative_one_nano.tv_sec == 0);
            REQUIRE(negative_one_nano.tv_nsec == 0);
        }

        {
            struct timespec negative_one_nano = {-1, 1000000001};

            internal::handle_underflow(negative_one_nano);

            REQUIRE(negative_one_nano.tv_sec == 0);
            REQUIRE(negative_one_nano.tv_nsec == 1);
        }

        {
            struct timespec negative_one_nano = {-5, 5000000005};

            internal::handle_underflow(negative_one_nano);

            REQUIRE(negative_one_nano.tv_sec == 0);
            REQUIRE(negative_one_nano.tv_nsec == 5);
        }

        {
            struct timespec negative_one_nano = {-5, 499999999};

            internal::handle_underflow(negative_one_nano);

            REQUIRE(negative_one_nano.tv_sec == 0);
            REQUIRE(negative_one_nano.tv_nsec == 0);
        }

        {
            struct timespec negative_one_nano = {-5, 5000000000};

            internal::handle_underflow(negative_one_nano);

            REQUIRE(negative_one_nano.tv_sec == 0);
            REQUIRE(negative_one_nano.tv_nsec == 0);
        }
    }

    SECTION("Literal Tests")
    {
        {
            const struct timespec test_time = 1_s;

            REQUIRE(test_time.tv_sec == 1);
            REQUIRE(test_time.tv_nsec == 0);
        }

        {
            const struct timespec test_time = 5_s;

            REQUIRE(test_time.tv_sec == 5);
            REQUIRE(test_time.tv_nsec == 0);
        }

        {
            const struct timespec test_time = 1.5_s;

            REQUIRE(test_time.tv_sec == 1);
            REQUIRE(test_time.tv_nsec == 500000000);
        }

        {
            const struct timespec test_time = 8.15_s;

            REQUIRE(test_time.tv_sec == 8);
            REQUIRE(test_time.tv_nsec == 149999999);
        }
    }

    SECTION("Addition Tests")
    {
        {
            const struct timespec test_time1 = 1_s;
            const struct timespec test_time2 = 1_s;

            auto sum_time = test_time1 + test_time2;

            REQUIRE(sum_time.tv_sec == 2);
            REQUIRE(sum_time.tv_nsec == 0);
        }

        {
            const struct timespec test_time1 = 1_s;
            const struct timespec test_time2 = 8.15_s;

            auto sum_time = test_time1 + test_time2;

            REQUIRE(sum_time.tv_sec == 9);
            REQUIRE(sum_time.tv_nsec == 149999999);
        }

        {
            const struct timespec test_time1 = 1.5_s;
            const struct timespec test_time2 = 2.5_s;

            auto sum_time = test_time1 + test_time2;

            REQUIRE(sum_time.tv_sec == 4);
            REQUIRE(sum_time.tv_nsec == 0);
        }
    }

    SECTION("Subtraction Tests")
    {
        {
            const struct timespec test_time1 = 7_s;
            const struct timespec test_time2 = 2_s;

            auto difference_time = test_time1 - test_time2;

            REQUIRE(difference_time.tv_sec == 5);
            REQUIRE(difference_time.tv_nsec == 0);
        }

        {
            const struct timespec test_time1 = 1.5_s;
            const struct timespec test_time2 = {1, 1};

            auto difference_time = test_time1 - test_time2;

            REQUIRE(difference_time.tv_sec == 0);
            REQUIRE(difference_time.tv_nsec == 499999999);
        }

        {
            const struct timespec test_time1 = 2_s;
            const struct timespec test_time2 = {1, 1};

            auto difference_time = test_time1 - test_time2;

            REQUIRE(difference_time.tv_sec == 0);
            REQUIRE(difference_time.tv_nsec == 999999999);
        }

        {
            const struct timespec test_time1 = 2_s;
            const struct timespec test_time2 = {2, 1};

            auto difference_time = test_time1 - test_time2;

            REQUIRE(difference_time.tv_sec == 0);
            REQUIRE(difference_time.tv_nsec == 0);
        }

        {
            const struct timespec test_time1 = 2_s;
            const struct timespec test_time2 = 2_s;

            auto difference_time = test_time1 - test_time2;

            REQUIRE(difference_time.tv_sec == 0);
            REQUIRE(difference_time.tv_nsec == 0);
        }
    }

    SECTION("Multiplication Tests")
    {
        {
            const struct timespec test_time1 = 7_s;

            auto multiplied_time = test_time1 * 2UL;

            REQUIRE(multiplied_time.tv_sec == 14);
            REQUIRE(multiplied_time.tv_nsec == 0);
        }
                
        {
            const struct timespec test_time1 = 7.5_s;

            auto multiplied_time = test_time1 * 2UL;

            REQUIRE(multiplied_time.tv_sec == 15);
            REQUIRE(multiplied_time.tv_nsec == 0);
        }
                
        {
            const struct timespec test_time1 = 10.2_s;

            auto multiplied_time = test_time1 * 0.1;

            REQUIRE(multiplied_time.tv_sec == 1);
            REQUIRE(multiplied_time.tv_nsec == 19999999);
        }
    }
}