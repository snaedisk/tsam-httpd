## HTTP server - Hypertext Transport Protocol

Computer Networks

Programming Assignment 2

Date: 09/10/2017

Authors: Aníta, Fanney og Þ. Snædís

Readme file for the implementation on assingnment to create a HTTP server. 

1. The HTTP server serves content using the Hypertext Transfer protocol and the client we use is a web browser. The implementation of this project was made using elements from the **glib library.** The project is runned through ru.skel.is  ......


##   Implementation   ##


Reserved addresses for this project:

- Port nr: 1088

- Server:127.0.0.1


## Description of functions

**httpd.c:** 
  
  Protocol to transfer a request to read or write a file, .......
  
=======================================================

**void cat()**
	
	--> sends the file content out on the socket. cat is a standard Unix utility that reads files sequentially, writing them to standard output.
	
**void bad_request()**

	--> informed the client about a bad request from the socket.
  
**void not_found_request()**

	--> sends a 404 not found error message.
  
**void unimplemented_request()**

	--> if the request is unimplemented the client will be informed about that
  
**void ok_request()**

	--> Informes the client that the request is valid and gives you information about that.
	
**void serve_file()**

	--> sends the file to the client and reports if there are errors.
  
**void write_logfile()**

	--> sends information to our logfile.log.
  
**int main()**
 
 	--> the main ....
