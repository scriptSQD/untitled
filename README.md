# Untitled

This trivial naming was a result of my hurry to create a new project and start exploring.

Here I'll be testing out different C++ stuff, learning things, etc.

Below you can find some useful stuff related to the project.

### Content map:

- [Usage](#usage)
- [Requirements](#requirements)
- [Setting up](#setting-up)
- [Debugging](#debugging)
- [Submodules](#submodules)

## Usage

The only real requirement is your database should have `sqd.untitled.manager` schema. Application will look for the one
and pull data from `school_info`, `students_info`, `grades_info`, `subjects_info` tables.

HiDPI support is not yet implemented, so beware that on some monitors the thing may appear blurry and ugly.

## Requirements

- Relatively new C++ compiler.
- C++17 Standard support
- CMake 3.12+
- PostgreSQL library in CMake search path

~~GCC doesn't work out because it doesn't support inline template specialization.
I gave it a few tries to optimize my code and make it compile on GCC, but it didn't seem to work. So only Clang/MSVC
support for now.~~

Compilers description:

1. GCC support is included too with function overrides instead of template specializations.
   Tested on version 12.1.1 from Gentoo Ebuild repos.

2. Clang works on 14.0.6 from Gentoo Ebuild repos as well.

## Setting up

```shell
# Clone
git clone --recursive git@github.com:scriptSQD/untitled
cd untitled

# Build
mkdir build && cd build
cmake ..
make

# Run
./untitled
```

## Debugging

I'm testing the code to build on both GCC 12 and Clang 14 compilers but mainly use LLDB for debugging.
For LLDB, you'd want to set "-fstandalone-debug" compiler flag for the strings to be displayed correctly.

## Submodules

I will be trying to split all advanced functionality to libraries and upload them separately as git modules.
Here are some of them:

- [Logger](https://github.com/scriptSQD/untitled-logger)
- [ArgParser](https://github.com/scriptSQD/untitled-argparser)
