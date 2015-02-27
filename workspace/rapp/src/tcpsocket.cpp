// Implementation of the Socket class.


#include "tcpsocket.h"
#include "string.h"
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include "types.h"
#include <iostream>
using namespace std;
Socket::Socket() :
	m_sock ( -1 ), m_dsock ( -1 )
{

	syslog(LOG_DEBUG, "%s ENTRY",__func__);
	memset ( &m_addr,
			0,
			sizeof ( m_addr ) );

}

Socket::Socket(SocketIf *sif) :
	m_sockif(sif), m_sock ( -1 ), m_dsock ( -1 )
{
	syslog(LOG_DEBUG, "%s ENTRY",__func__);
	memset ( &m_addr,
			0,
			sizeof ( m_addr ) );
}

Socket::~Socket()
{
	syslog(LOG_DEBUG, "%s ENTRY",__func__);
	if ( is_valid() )
	{
		close ();
	}
}

void Socket::close()
{
	syslog(LOG_DEBUG, "%s ENTRY",__func__);
	if(-1 != m_dsock) ::close(m_dsock);	
}
bool Socket::create()
{
	syslog(LOG_DEBUG, "%s ENTRY",__func__);
	m_sock = socket ( AF_INET,
			SOCK_STREAM,
			0 );

	if ( ! is_valid() )
		return false;
	int on = 1;
	if ( setsockopt ( m_sock, SOL_SOCKET, SO_REUSEADDR, ( const char* ) &on, sizeof ( on ) ) == -1 )
	{

		syslog(LOG_ERR," Socket::create() setsocketopt failed");
		return false;
	}


	return true;

}



bool Socket::bind ( const int port )
{

	syslog(LOG_DEBUG, "%s ENTRY",__func__);
	if ( ! is_valid() )
	{
		syslog(LOG_ERR," Socket::bind() not valid sock fd");
		return false;
	}



	m_addr.sin_family = AF_INET;
	m_addr.sin_addr.s_addr = INADDR_ANY;
	m_addr.sin_port = htons ( port );

	int bind_return = ::bind ( m_sock, ( struct sockaddr * ) &m_addr,
			sizeof ( m_addr ) );


	if ( bind_return == -1 )
	{
		syslog(LOG_ERR," Socket::bind() failed invalid fd");
		return false;
	}

	return true;
}


bool Socket::listen() const
{
	syslog(LOG_DEBUG, "%s ENTRY",__func__);
	if ( ! is_valid() )
	{
		syslog(LOG_ERR," Socket::Listen() failed invalid fd");
		return false;
	}

	int listen_return = ::listen ( m_sock, MAXCONNECTIONS );


	if ( listen_return == -1 )
	{
		syslog(LOG_ERR," Socket::Listen() ::listen() failed ");
		return false;
	}

	return true;
}


bool Socket::accept ( )
{
	syslog(LOG_DEBUG, "%s ENTRY",__func__);
	int addr_length = sizeof ( m_addr );
	m_dsock = ::accept ( m_sock, ( sockaddr * ) &m_addr, ( socklen_t * ) &addr_length );
	if ( m_dsock <= 0 )
	{
		syslog(LOG_ERR," Socket::accept() ::accept() failed ");
		return false;
	}
	else
	{
		Epoll *epoll_ = Epoll::getInstance();
		initialize(getfd());
		epoll_->addFd(this);
		return true;
	}
}


int Socket::send ( unsigned char *buffer, int len ) const
{
	syslog(LOG_DEBUG, "%s ENTRY",__func__);
	return ::send ( m_dsock,buffer ,len, MSG_NOSIGNAL );
}

int Socket::sendL(unsigned char *buffer, int len, int maxbuf)const
{
	syslog(LOG_DEBUG, "%s ENTRY",__func__);
	int i,bytes =0;
	Header headerAck;
	unsigned char ack;
	for(i =0; (i+maxbuf)< len; i = i+maxbuf)
	{
		if ((bytes += this->send(&buffer[i], maxbuf)) == -1) 
		{
			syslog(LOG_ERR,"failed");
			return -1;
		}
		//syslog(LOG_ERR,"I");
		if (this->recv(&ack, 1) == -1) 
		{
			syslog(LOG_ERR,"Ack failed [recv]");
			return -1;
		}
		if( ack != REMOTE_ACK)
		{

			syslog(LOG_ERR,"Ack failed [REMOTE_ACK]");
			return -1;
		}
	}
	if ((bytes += 	this->send(&buffer[i], len - i)) == -1) 
	{
		syslog(LOG_ERR,"failed");
		return -1;
	}

	if (this->recv(&ack,1) == -1)
	{
		syslog(LOG_ERR,"Ack failed");
		return -1;
	}
	if( ack != REMOTE_ACK)
	{

		syslog(LOG_ERR,"Ack failed [REMOTE_ACK]");
		return -1;
	}
	syslog(LOG_DEBUG,"Socket::sendL()");
}

int Socket::recv ( unsigned char *buffer, int len) const
{

	syslog(LOG_DEBUG, "%s ENTRY",__func__);
	int status = ::recv ( m_dsock, buffer, len, 0 );

	if ( status == -1 )
	{
		syslog(LOG_ERR,"::recv failed errno: %d ",errno);
		return 0;
	}
	return status;
}


int Socket::recvL(unsigned char *buffer, int len, int maxbuf ) const
{
	syslog(LOG_DEBUG, "%s ENTRY",__func__);
	int i;
	int bytes = 0;
	unsigned char ack = REMOTE_ACK;
	for(i =0; (i+maxbuf)< len; i=i+maxbuf)
	{
		if ((bytes += 	this->recv(&buffer[i], maxbuf)) == -1) 
		{
			syslog(LOG_ERR,"failed");
			return -1;
		}

		if (this->send(&ack,1) == -1) 
		{
			syslog(LOG_ERR,"failed");
			return -1;
		}
	}
	if ((bytes += 	this->recv(&buffer[i], len - i)) == -1) 
	{
		syslog(LOG_ERR,"failed");
		return -1;
	}
	if (this->send(&ack,1) == -1) 
	{
		syslog(LOG_ERR,"failed");
		return -1;
	}
	return bytes;

}

void Socket::epoll_cb()
{
	syslog(LOG_DEBUG, "%s ENTRY",__func__);
	int bytes = 0;
	unsigned char buffer[MAXBUF] = {0};

	if ((bytes = 	this->recv(buffer, MAXBUF)) == -1) 
	{
		syslog(LOG_ERR,"recv failed in epoll_cb");
	}
	else if(bytes == 0)
	{
		syslog(LOG_ERR,"client disconnected");
		this->close();
		m_sockif->clientDisconnected();	
	}

	if(NULL != m_sockif)
	{
		m_sockif->onDataReceived(buffer, bytes);	
	}
}

bool Socket::connect ( const std::string host, const int port )
{
	syslog(LOG_DEBUG, "%s ENTRY",__func__);
	if ( ! is_valid() ) return false;

	m_addr.sin_family = AF_INET;
	m_addr.sin_port = htons ( port );

	int status = inet_pton ( AF_INET, host.c_str(), &m_addr.sin_addr );

	if ( errno == EAFNOSUPPORT ) return false;

	status = ::connect ( m_sock, ( sockaddr * ) &m_addr, sizeof ( m_addr ) );

	if ( status == 0 )
	{
		m_dsock = m_sock; 
		return true;
	}
	else
		return false;
}

int Socket::initServer(const int portnum)
{
	syslog(LOG_DEBUG, "%s ENTRY",__func__);
	if ( ! create() )
	{
		syslog(LOG_ERR,"Could not create server socket.");
		return -1;
	}

	if ( ! bind ( portnum ) )
	{
		syslog(LOG_ERR,"Could not bind to port.");
		return -1;
	}

	if ( ! listen() )
	{
		syslog(LOG_ERR,"Could not listen to socket.");
		return -1;
	}
	syslog(LOG_INFO,"Socket::initServer() success");
	return 0;
}


int Socket::initClient(const std::string ipAdd,const int portnum)
{
	syslog(LOG_DEBUG, "%s ENTRY",__func__);
	if ( ! create() )
	{
		syslog(LOG_ERR,"Could not create server socket.");
		return -1;
	}
	if ( ! connect ( ipAdd, portnum ) )
	{
		syslog(LOG_ERR, "Could not bind to port.");
		return -1;
	}
	Epoll *epoll_ = Epoll::getInstance();
	if(epoll_ == NULL)
	{
		syslog(LOG_ERR, " Epoll::getInstance() failed");
		return -1;
	}
	initialize(getfd());
	epoll_->addFd(this);
	syslog(LOG_INFO,"Socket :: Init client success");
	return 0;
}

void Socket::set_non_blocking ( const bool b )
{

	syslog(LOG_DEBUG, "%s ENTRY",__func__);
	int opts;

	opts = fcntl ( m_sock, F_GETFL );

	if ( opts < 0 )
	{
		return;
	}

	if ( b )
		opts = ( opts | O_NONBLOCK );
	else
		opts = ( opts & ~O_NONBLOCK );

	fcntl ( m_sock, F_SETFL,opts );

}
