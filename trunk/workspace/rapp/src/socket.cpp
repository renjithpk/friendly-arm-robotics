/*
 * TcpSock.cpp
 *
 *  Created on: Dec 24, 2014
 *      Author: renjith
 */

#include "TcpSock.h"

TcpSock::TcpSock()
{

}

TcpSock::~TcpSock()
{

}

int TcpSock::Connect(void * aArg)
{
        struct  sockaddr_in   serverAddr,  clientAddr;
        socklen_t             clientAddrLen = sizeof(clientAddr);
        if ((listenSock = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
            quit("socket() failed.", 1);
        }
            
        serverAddr.sin_family = PF_INET;
        serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
        serverAddr.sin_port = htons(listenPort);

        if (bind(listenSock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
                quit("bind() failed", 1);
        }
                
        if (listen(listenSock, 5) == -1) {
                quit("listen() failed.", 1);
        }
        
                
        /* start receiving images */
        while(1)
        {
	        cout << "-->Waiting for TCP connection on port " << listenPort << " ...\n\n";
	  	
		/* accept a request from a client */
	    if ((connectSock = accept(listenSock, (sockaddr*)&clientAddr, &clientAddrLen)) == -1) {
	                quit("accept() failed", 1);
	        }else{
		    	cout << "-->Receiving image from " << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) << "..." << endl;
		}
		
}

int TcpSock::Write(void* aArg)
{

}


int TcpSock::Read(void* aArg)
{

}


