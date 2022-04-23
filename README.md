# C Socket Programming: Simple Server and Client

Distributed automation systems / Faculty of Electrical Engineering (University in Tuzla)

## Client and Server

The goal of this repository is to implement a simple client-server application that transfers an arbitrary file from client to server. The application message has a header and a body. The header carries information about the length of the file name in bytes and the file name. The body of the message is the data from the file.
The client forms and sends the specified message. The server accepts the specified message and saves the received file under the same name to disk.

Since both the server and the client are located on the same computer, they need to be implemented in separate folders (because the files have the same name). You need to run the server application first.

### Server

Server app -> fileserver.c

Compile with:

```
gcc fileserver.c -o fileserver
```

and run with:

```
./fileserver
```

### Client

Client app -> fileclient.c

Compile with:

```
gcc fileclient.c -o fileclient
```

and run with:

```
./fileclient  first-test-file
```
