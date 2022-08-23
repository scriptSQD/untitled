# Untitled

This trivial naming was a result of my hurry to create a new project and start exploring.

Here I'll be testing out different C++ stuff, learning things, etc.

Below you can find some useful stuff related to the project.

### Content map:

- [Usage](#usage)
- [Details](#details)
- [Requirements](#requirements)
- [Setting up](#setting-up)
- [Debugging](#debugging)
- [Submodules](#submodules)

## Details

With the latest release, program now "introspects" the database and is able to view it all providing great UI controls
for that.
This starts to turn into DBMS, but will never become a fully-functional one. I will just give it a little more practice
and move on
to the next project.

HiDPI support is not yet implemented, so beware that on some monitors (spotted on Windows machines) the thing may appear
blurry and ugly.

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
