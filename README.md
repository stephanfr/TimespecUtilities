# TimespecUtilities
Utilities to adapt struct timespec to C++ code

# Literals

The _s 'seconds' and '_ms' milliseconds suffix operator is defined for struct timespec.  Additionally, addition, subtraction and multiplication operators are defined as well.  Negative times are not permitted and any negative times will be set to zero.

# Examples

````
    const struct timespec five_seconds = 5_s;
    const struct timespec one_and_one_half_seconds = 1.5_s;
    const struct timespec five_hundred_milliseconds = 500_ms;

    const struct timespec six_and_one_half_seconds = five_seconds + one_and_one_half_seconds;
    const struct timespec three_and_one_half_seconds = five_seconds - one_and_one_half_seconds;

    const struct timespec ten_seconds = five_seconds * 2;
    const struct timespec fifty_milliseconds = five_hundred_milliseconds * 0.1;
````
