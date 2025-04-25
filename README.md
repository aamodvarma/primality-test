# primality-test

The initial idea was to test the three different primality testing algos on extremely large numbers, from 10 digit numbers to 2000 digit numbers. However, it seems like even with higher k, all there perform almost identically. The code for this is in the C++.

The second idea was to more realistically see how long it would take to find primes of certain bit sizes using the three algos. This is more relevant as finding primes is important for RSA algos. The graphs are more clear in terms of what algo is better - seems like miller-rabin is generally better but not by a huge margin, it's possible for larger bit sizes the margin is higher. But it is also relevant to note that fermats just doesn't work with Carmichael numbers anyway so for the most part, considering all the cases, miller-rabin is better.

In the graphs, naive was cutoff at 50 bits because it took way to long.
