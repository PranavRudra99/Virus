cd source
make

In the source folder, there are 2 cpp files:
virus.cpp
host.cpp

Running make command in the source folder will generate the binaries for virus, host. The virus binary is executed as ./virus, with no arguments. This generates the initial seed.

Running:
./seed some_binary
Will infect the binary(some_binary). The infection will increase the size of some_binary by size of virus + 14 bytes.

size(infected_binary) = size(some_binary) + size(virus) + 14 bytes.

4 bytes of this is the offset: 0xdeadbeef.
The rest 10 bytes is a random hex of length 20 (20*4 = 80 bits = 10 bytes).
