
Nicholas LaRosa
CSE 30264, Program 4

Files:

server6.c

	Source code for UDP server, written in C, compiled with the following command:
	gcc server6.c -o server6

server6
		
	Executable version of the IP agnostic UDP server, used with the following command:
	./server6

	IP agnostic UDP server will listen on port 9979 of whichever host it is executed from. End the server with CTRL^C.

client6.c
	
	Source code for the IP agnostic UDP client, written in C, compiled with:
	gcc client6.c -o client6

client6

	Executable version of the IP agnostic UDP client, used with the following command:
	./client6 <IP_Address_v4/6> <Port_Number> <File_to_Send>

NOTE - Extra credit attempted!

File1.txt
File2.txt
File3.txt

        Test files to be sent to the server at port 9979 of machine running server

Result1.txt
Result2.txt
Result3.txt

        Result files from the corresponding test files
        The results were retrieved from the server at port 9979 of machine running server
        The server successfully inverted the sent buffer and appended a timestamp

