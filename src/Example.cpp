
#include "Socket.h"
#include <string>


int main()
{
	// create socket object
	Socket s = Socket();

	// resolve your socket domain
	unsigned int ip_addr = s.resolve_domain("sub.domain.tld");

	// init connection to ip and port
	s.init_connection(ip_addr, 9999);
	
	// send data as a string
	std::string data = "Hello world!";
	s.send_str(data);

	// send data as a char array
	const char* charArray = "Hello again!";
	s.send_data(charArray);

	// send raw data
	byte rawData[4] = { 0x1, 0x2, 0x3, 0x4 };
	s.send_data((const char*)rawData, 4);

	// receive into a buffer with a fixed len
	int len = 256;
	char* buf = (char*)malloc(len);
	s.recv_data(buf, len);

	// just read a string
	std::string output = s.recv_string();

	// close the socket
	s.close_connection();

}