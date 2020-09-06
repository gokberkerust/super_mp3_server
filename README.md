# Mp3 Download Server And Client

This a sample implementation of an application layer protocol to download mp3 files from server.  Server checks the master file and sends the name and the size(how many bytes) of the requested file. Client reads this packet, opens the file than sends an acknowledgment to the server, than server sends the file.

In this application;

* clients can browse the list of the mp3 files
* download them in their computers.
* Registered user may upload files to the server also.

<br/>

## Features

* Application works on TCP.
* Uses 256 bytes of TPDUs
* The server is designed to accept more than one connections between clients simultaneously. 
* Also client programs are designed to do multiple tasks per connection. So user does not need to start the program again and again. 


## Usage

Clients need to start the program with two arguments; the first argument is binded to the local host IP address and the second one is the port number.

To run the server:

```bash
gcc super_mp3_server.c -o server
./server
```

To run the client:

```bash
gcc client.c -o client
./client gokberk 5001
```


Server listens to connections at the port:5001 after the connection is established between client and server, client may query to server capabilities. 

Client sends this number to the server and corresponding service runs. For inquiring the largest MID and listing all the mp3 files, server reads the master file and response the client according to algorithm.

Downloading mp3 files is a little bit complex than the other services. Service needs another number(MID) to start the download. If the client gives a number that is not available, the mp3 file with the largest ID will be downloaded to the client.

## Disclaimer

This project is for my senior class Cmpe476 at boun, thus it has not been maintained. It could also be improved by getting the list of music files as an argument. 

Compiled with gcc on ubuntu 14.04 machine.

Registration of to the system is implemented in server side, however it isn't tested

 I used 8192 bytes to buffer the downloaded stream,to make it faster,I did not calculate the optimal TPDU size because TCP has its own flow control on it. After the download complete client may continue in the program.

 Additional rules: 

 * User should not interrupt the download service. 
 * At the server side, the mp3 files which are listed in the master.txt must be located at the same directory of the server program. 
 * the master.txt must be in the “ID NAME PATH” format and the uploaded mp3 files information must be added to the master.txt file by the required format.

