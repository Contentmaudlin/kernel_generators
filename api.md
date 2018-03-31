
## Lookback
Implemented as stateful variables inside the generator. Can't `yield()` old values.

move semantics

## generator and iterator

Ranging over generator should yield the same values in different threads instead of starting from the end on the second range-over.

Generator is a factory method for an iterator. Iterator is what actually yields the values. Multiple iterators is multiple states over the same generator.

When we're iterating over values, we only have the semantics of an `InputIterator`.  

*Sequential:*
```
fibGn gn{3};
for (auto &x : gn) {
 // work 
}
```
*Threaded:*
```
fibGn gn{3, thread_num{5}};
// now gn generates in a threaded fashion
```

## threading

Since it's not practical to asynchronously keep generating, the generator should asynchronously generate `n` values for `thread(n)`. `thread(0)` is completely unbuffered.

## OO 

```
class fibGn : Gn {
	lb<int, 3> x;

	fibGn(int _x, thread_num N = 0) : Gn{N} {}

	/* defaults to no threading if no thread_num given
	 * invoke constructor of super class with N=0
	 */
	fibGn(int _x) : x(_x) {} 

														
}

fibGn gn{3}; // no threading

fibGn gn{3, thread_num{5}};
```
