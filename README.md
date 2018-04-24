# Kernel Generators

A C++ library/framework for defining "kernel generators", functions which generate values from a custom-defined data stream, process them and `yield()` them when necessary. The programmer can define any kind of data source (an algorithm, a file, a network socket etc.), process what's generated from the data source and yield those values. 

Kernel generators are locally stateful (i.e. subsequent `yield()` calls produce different results), and are also concurrent (as values arrive to a network socket, the kernel generator will concurrently read and store them, making them available to `yield()`. 

Moreover, this library also allows one to chain `map()` or `filter()` calls to generators to give different behavior to existing generators.

Finally, one can also define a kernel generator with a **lb_core**. This will make the kernel generator store a circular buffer of its local state, allowing easy acccess to a previous yield.
