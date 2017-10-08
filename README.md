## HTTP server - Hypertext Transport Protocol

Computer Networks

Programming Assignment 2

Date: 09/10/2017

Authors: Aníta, Fanney og Þ. Snædís

Readme file for the implementation on assingnment to create a HTTP server. 

This is a implementation of a HTTP server written in C language as an assignment from Reykjavik University. This project is made to understand the http service process. The HTTP server serves content using the Hypertext Transfer protocol and for the client we use a web browser. The implementation of this project was made using elements from the **glib library.** The project is runned through ru.skel.is.

=======================================================

##   Implementation   ##

Reserved addresses for this project:

- Port nr: 1088

- Server:127.0.0.1


## Description of functions

**httpd.c:** 
  
  Protocol to transfer a request to read or write a file.

	
**char webpage[]**

	--> array that holds all the HTML elements for the browser.
	
**void write_logfile()**

	--> sends information to our logfile.log.
  
**int main()**
 
 	--> makes the client/socket connection and retrives the PORT number and a value to the IP number.
