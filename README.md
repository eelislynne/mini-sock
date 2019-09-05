# mini-sock
A tiny send and receive socket client header library for C++

Allows tcp connections to sockets in other languages. Not a websocket client. Check src/Example.cpp for instructions.

Supports:
  * Sending & receiving (obvs)
  * IPv4 addresses and domains
 
 Does not support:
  * IPv6
  * Non-blocking operation
  
This is not supposed to be a robust solution. Just something for very basic live communication.
Made this for sending log files live to a NodeJS server.
