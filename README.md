
# Socket UDP/TCP [Stock Management]

# Language : C



# Socket UDP/TCP

TCP works in connected mode: he PREVENTS that the receiver will send (hence the connection establishment) and While sending the receiver sends acknowledgments( after ensuring the validity of data) .
So it is slow but reliable.

UDP works in unconnected mode: it scales without preventing the receiver, and the receiver does not confirm reception (and check for errors).
This is faster but less reliable.

UDP is sort of a lighter version of TCP.



# License
MIT License, Copyright (c) 2014 Adib HANNACHI
