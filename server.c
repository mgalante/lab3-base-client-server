// gcc server.c -Wall

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
// sockets => 
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netinet/tcp.h>


#define PORT 3456

int tcp_socket_server;
int udp_socket_server;
int unix_socket_server;


void logger(char *text) {
  printf("%s", text);
}


int start_tcp_server(){
  logger("starting TCP server");
  // Create the server-side socket
  tcp_socket_server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (tcp_socket_server < 0) {
    perror("TCP socket");
    return 0;
  }

  int flag = 1;
  int result = setsockopt( tcp_socket_server, IPPROTO_TCP, TCP_NODELAY, (char *)&flag, sizeof(int) );
  if (result < 0) {
    perror("TCP setsockopt(TCP_NODELAY)");
    return 0;
  }
  
  result = setsockopt( tcp_socket_server, SOL_SOCKET, SO_REUSEADDR, (char *)&flag, sizeof(int) );
  if (result < 0) {
    perror("TCP setsockopt(SO_REUSEADDR)");
    return 0;
  }

  result = setsockopt( tcp_socket_server, SOL_SOCKET, SO_KEEPALIVE, (char *)&flag, sizeof(int) );
  if (result < 0) {
    perror("TCP setsockopt(SO_KEEPALIVE)");
    return 0;
  }
  
  struct sockaddr_in address;
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);

  // Bind socket to the appropriate port and interface (INADDR_ANY)
  if ( bind( tcp_socket_server, (struct sockaddr *)&address, sizeof(address) ) < 0 ) {
    perror("TCP bind");
    return 0;
  }

  // Listen for incoming connection(s)
  if ( listen( tcp_socket_server, 1 ) < 0 ) {
    perror("TCP listen");
    return 0;
  }
  logger("TCP server started");
  return 1;
}

int start_udp_server(){
  logger("starting UDP server");
  udp_socket_server = socket(AF_INET, SOCK_DGRAM, IPPROTO_TCP);
  if (udp_socket_server < 0) {
    perror("TCP socket");
    return 0;
  }
  logger("UDP server started");
  return 1;
}

int start_unix_socket_server(){
  logger("starting Unix server");
  logger("Unix server started");
  return 1;
}

void stop_tcp_server(){
  close(tcp_socket_server);
}

void stop_udp_server(){
}

void stop_unix_socket_server(){

}

void accept_new_clients(){
  // int result = accept( tcp_socket_server, NULL, NULL );
  // if (result == -1)
  // { 
  //   // These are not errors according to the manpage.
  //   if ((errno == ENETDOWN || errno == EPROTO || errno == ENOPROTOOPT || errno == EHOSTDOWN || errno == ENONET || errno == EHOSTUNREACH || errno == EOPNOTSUPP || errno == ENETUNREACH))
  //     return result;
  //   else
  //     throw SocketCreationException ("TcpServer::accept > accept return error"); // Just to make the user code a little less messy.
  // }

  // return result;  
}

void listen_and_accept_new_clients(){
  //accept_new_clients();
}


// main function
int main(int argc, char *argv[]) {

  if (start_tcp_server() && start_udp_server() && start_unix_socket_server()) {
    logger("Server started successfully");
  } else {
    stop_tcp_server();
    stop_udp_server();
    stop_unix_socket_server();
    return EXIT_FAILURE;
  }

  listen_and_accept_new_clients();

  return EXIT_SUCCESS;
}
