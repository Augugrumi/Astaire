# astaire

[![Build Status](https://travis-ci.org/Augugrumi/astaire.svg?branch=master)](https://travis-ci.org/Augugrumi/astaire)

## How to build

You need CMake to build this program. In the root of the repository:
```
mkdir build/;
cd build;
cmake -DCMAKE_BUILD_TYPE=[Debug|Release] -DJNI=[True|False] ..;
make;
```

Table of flags:

| Name             | Default   | Options          | Description                                                        |
|------------------|-----------|------------------|--------------------------------------------------------------------|
| CMAKE_BUILD_TYPE | `Debug`   | `Debug`/`Relase` | Set the build type. Debug offers more logs and debug info          |
| JNI              | `False`   | `True`/`False`   | Set Java/C++ integration. This option allows to use Java handlers  |

# License

GPLv3
