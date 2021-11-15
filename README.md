# TCP_Client_Server
Create a unidirectional (client to server) TCP client-server connection in C.

## How to run:
* *gcc -o server server.c*
* *gcc -o client client.c*
* *./server*
  * start the server on the localhost
* *./client* \[*server_host*\]
  * run a client on the given server (in this example our localhost)

## Details:
* Any messages entered through client will appear on server
  * Messages cannot be sent from server to client (due to the unidirectional connection)
