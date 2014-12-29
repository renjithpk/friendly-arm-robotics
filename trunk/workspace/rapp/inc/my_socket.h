// Definition of the Socket class

#ifndef Socket_class
#define Socket_class


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <string>
#include <arpa/inet.h>

#include <iostream>
using namespace std;

const int MAXHOSTNAME = 200;
const int MAXCONNECTIONS = 5;
const int MAXRECV = 500;
const int MAXBUF =  400;
const unsigned char REMOTE_ACK = 0x0A;


class Socket
{
 public:
  Socket();
  virtual ~Socket();

  // Server initialization
  bool create();
  bool bind ( const int port );
  bool listen() const;
  bool accept ( Socket& ) const;

  // Client initialization
  bool connect ( const std::string host, const int port );

  // Data Transimission
  int send (unsigned char *buffer, int len) const;
  int sendL(unsigned char *buffer, int len, int maxbuf = MAXBUF)const;
  int recv (unsigned char *buffer, int len) const;
  int recvL(unsigned char *buffer, int len, int maxbuf = MAXBUF ) const;


  void set_non_blocking ( const bool );

  bool is_valid() const { return m_sock != -1; }

  int initServer(const int portnum);
  int initClient(const std::string ipAdd,const int portnum);
 private:

  int m_sock;
  sockaddr_in m_addr;


};


#endif
