# Kernel Generators

A C++ library/framework for defining "kernel generators", functions which generate values from a custom-defined data stream, process them and yield them when necessary. The programmer can define any kind of data source (an algorithm, a file, a network socket etc.), process what's generated from the data source and yield those values. 

Kernel generators are locally stateful (i.e. subsequent yielding calls produce different results), and also support concurrency (as values arrive to a network socket, the kernel generator will concurrently read and store them, making them available to yield). 

This library also allows one to chain `map()` or `filter()` calls to generators to give different behavior to existing generators.

Check the full API [here](https://github.com/Contentmaudlin/kernel_generators/wiki/Kernel-Generators-API).
