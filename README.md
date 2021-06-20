# potioncracker
This is another project I've had laying around on my hard drive for a while.

It uses the Z3 library to crack what potion combinations someone could have based on the particles they emit.

I wrote it in a portable way, so it can be easily integrated into a client. I don't care who's client it is as long as they give credit.

This also isn't very well tested, but it worked fine on the test cases I tried.

## Missing features
* Multithreading
* Extensive testing
* Lookup tables for common potion combinations (e.g. golden apples)
* Ability to use additional heuristics apart from invisibility and glowing
* Effect blacklist
* Ability to iterate over all possibilities

You might have to also update the effects list. This code was written for 1.16, but should work on 1.17.