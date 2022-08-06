# Untitled

This trivial naming was a result of my hurry to create a new project and start exploring.

Here I'll be testing out different C++ stuff, learning things, etc.

Below you can find some useful stuff related to the project.

## Requirements

- Relatively new Clang or MSVC compiler.
- CMake 3.7+
- C++17 Standard support

GCC doesn't work out because it doesn't support inline template specialization. 
I gave it a few tries to optimize my code and make it compile on GCC, but it didn't seem to work.

So only Clang/MSVC support for now.

## Setting up

```shell
# Clone
git clone git@github.com:scriptSQD/untitled
cd untitled

# Build
mkdir build && cd build
cmake ..
make

# Run
./untitled -h
# untitled - a scriptSQD's C++ program to learn and test new things.
# 
# Available arguments and usage patterns:
# Usage:
#     1. untitled
#     2. untitled -v
#     3. untitled -f /bin
#     4. untitled --df ~/untitled_log_2022.log
# Arguments:
#     --file, -f                  File path to check for existence.
#     --verbose, -v               Enable verbose mode (expanded output).
#     --help, -h                  Display help menu and exit.
#     --debug-to-file, --df       Print debug info to file specified file
#     --no-stdout, -n             Do not print debug to stdout if --debug-to-file is enabled.
```

## Submodules

I will be trying to split all advanced functionality to libraries and upload them separately as git modules.
Here are some of them:
- [Logger](https://github.com/scriptSQD/untitled-logger)
- [ArgParser](https://github.com/scriptSQD/untitled-argparser)