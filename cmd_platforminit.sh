git submodule update --init --recursive
cmake --preset debug
cmake --build --preset debug
./build/debug/bin/sandbox
