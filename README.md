quaternion
==========

Brief: Implementation of a generic, unit-tested [quaternion](http://en.wikipedia.org/wiki/Quaternion) with no dependencies, other than the c++11 STL. Includes functions for multiplication, rotation, addition, integration (euler and runge kutta), and conversion to/from matrices.

### Usage:

Simply include `quaternion.hpp` in your project and enjoy.

### Building tests:

In order to build tests, define the `GTEST_DIR` environment variable as the location of your [google-test](https://code.google.com/p/googletest/) installation, and run cmake. For example:

```bash
    mkdir build ; cd build
    export GTEST_DIR=/path/to/gtest
    cmake .. ; make
    ./quat_test
```

### Bug reporting:

Report bugs by opening an issue on github.

### License:

quaternion is [MIT licensed](http://opensource.org/licenses/MIT).

### Authors:

G. Cross and [C. Qu](https://github.com/versatran01).
