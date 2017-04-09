# codename-ted/pirate panic
### For CMSC-23800: Game Construction, Spring '17 @UChicago

1. [Project Overview](#overview)
2. [Style Guide](#style-guide)
3. [Networking/Server](#server)

## Overview
We are working with the idea of sailboats racing to an objective.

One of the sailboats is a pirate ship who is tasked with stealing the booty of
the other sailboats. Each sailboat has to make it to the finish line with as
much booty as possible.

If a sailboat loses all of its booty, it also loses the race.

## Server
[Click here to see the technical description of how the server should work](https://docs.google.com/document/d/1ZTzbGsyE5LcycYMy2wDMdKz1A_tkYIssq6CrsBmdudE/edit)

Hit jabois up in the #server channel on slack if you have any Qs.

The server uses and requires [Boost](http://www.boost.org/), specifically
[Boost.Asio](http://www.boost.org/doc/libs/1_61_0/doc/html/boost_asio/overview.html).
The echo server and echo client binaries were compiled with boost_1_61_0.

I recommend taking a look at the [udp client tutorial](http://www.boost.org/doc/libs/1_63_0/doc/html/boost_asio/tutorial.html#boost_asio.tutorial.tutdaytime4)
and the [udp server tutorial]
(http://www.boost.org/doc/libs/1_63_0/doc/html/boost_asio/tutorial.html#boost_asio.tutorial.tutdaytime5)
to get an idea for how the game loop will work.

The registration stage makes sense to do via standard TCP/HTTP rather than the
UDP/Web Socket Protocol employed for the game loop, so it would also be a good
idea to take a look at the [tcp client tutorial](http://www.boost.org/doc/libs/1_63_0/doc/html/boost_asio/tutorial.html#boost_asio.tutorial.tutdaytime1)
and the [tcp server tutorial](http://www.boost.org/doc/libs/1_63_0/doc/html/boost_asio/tutorial.html#boost_asio.tutorial.tutdaytime2)


## Style Guide
Check `sample.cpp` and `sample.h` for generic style guidelines.
