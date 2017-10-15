#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <iostream>
#include <string>
#include <sstream>


class GPSInput {
public:
  GPSInput(){

    int port = 3501;
    sockaddr_in serverAddress = {0};
    struct hostent *server;
    char hostname[] = "192.168.42.1";


    /* socket: create the socket */
    socketID = socket(AF_INET, SOCK_STREAM, 0);
    if (socketID < 0) {
      std::cout << "Failed to open socket\n";
      exit(0);
    }

    /* gethostbyname: get the server's DNS entry */
    server = gethostbyname(hostname);
    if (server == nullptr) {
      std::cout << "Failed to find host\n";
      exit(0);
    }

    /* build the server's Internet address */
    serverAddress.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serverAddress.sin_addr.s_addr, server->h_length);
    serverAddress.sin_port = htons(port);


    /* connect: create a connection with the server */
    if (connect(socketID, (const sockaddr*)&serverAddress, sizeof(sockaddr_in)) < 0){
      std::cout << "Failed to connect!\n";
      exit(0);
    }

  }

  void run(){

    static std::string line;
    char b;

    while(true){
      while(!read(socketID, &b, 1));
      line += b;
      if(b == '\n'){
        processLine(line);
        line = "";
      }
    }

  }



  /*

  1890 GPS Week
  464306.800 GPS Seconds of Week
  58.990662423 latitude(deg)
  14.265681149 longitude(deg)
  66.5928 height(m)
  2 Q where 2 means Float (,Q=1:fix,2:float,3:sbas,4:dgps,5:single,6:ppp)
  10 ns= of satellites
  0.0112 sdn(m)
  0.0073 sde(m)
  0.0162 sdu(m)
  0.0037 sdne(m)
  -0.0044 sdeu(m)
  -0.0045 sdun(m)
  1.80 age(s)
  1.1 ratio

  */
  void processLine(std::string line){
    std::stringstream ss(line);

    std::string GPSWeek;
    std::string GPSSeconds;
    std::string lat;
    std::string lon;
    std::string alt;

    ss >> GPSWeek >> GPSSeconds >> lat >> lon >> alt;

    std::cout << lat << ", " << lon << std::endl;

  }

private:
  int socketID;

};
