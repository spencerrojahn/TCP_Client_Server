# TCP_Client_Server
Create a TCP client-server connection in C. Where data is only sent and received from client to server.

## How to run:
* *gcc -o server server.c*
* *gcc -o client client.c*
* *./server*
  * start the server on the localhost
* *./client* \[*server_host*\]
  * run a client on the given server (in this example our 'localhost')
  * server and client must be run in different processes

## Details:
* Any messages entered through client will appear on server
  * Messages cannot be sent from server to client (due to the unidirectional connection)
