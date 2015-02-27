// Definition of the Socket class

#ifndef _Socket_class_
#define _Socket_class_


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <string>
#include <arpa/inet.h>

#include "epoll.h"
//using namespace std;

const int MAXHOSTNAME = 200;
const int MAXCONNECTIONS = 5;
const int MAXRECV = 500;
const int MAXBUF =  400;
const unsigned char REMOTE_ACK = 0x0A;


class SocketIf
{
	public:
		virtual int onDataReceived(unsigned char * buffer, int size)=0;
		virtual int clientDisconnected(){}
};

class Socket: public EpollIf
{
	public:
		Socket();
		Socket(SocketIf *sif);
		virtual ~Socket();

		// Server initialization
		bool create();
		bool bind ( const int port );
		bool listen() const;
		bool accept ();
		void close();	
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
		int getfd(){return m_dsock;}
		void registerInterface(SocketIf * sif)
		{
			m_sockif = sif;
		}
		void epoll_cb();
	private:

		int m_sock;
		int m_dsock;
		sockaddr_in m_addr;
		SocketIf * m_sockif;	
};


#endif
