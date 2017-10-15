class LidarInput{
private:
  int socketID;

  void error(char err[]){
    std::cout << err << std::endl;
  }


  char hostname[] = "10.42.0.84";
  int portno = 4141;

public:


  LidarInput() {

      int sockfd, n;
      struct sockaddr_in serv_addr;
      struct hostent *server;

      socketID = socket(AF_INET, SOCK_STREAM, 0);
      if (socketID < 0)
          error("ERROR opening socket");


      server = gethostbyname(hostname);
      if (server == NULL) {
          fprintf(stderr,"ERROR, no such host\n");
          exit(0);
      }
      bzero((char *) &serv_addr, sizeof(serv_addr));

      serv_addr.sin_family = AF_INET;

      bcopy((char *)server->h_addr,
           (char *)&serv_addr.sin_addr.s_addr,
           server->h_length);
      serv_addr.sin_port = htons(portno);

      if (connect(socketID,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
          error("ERROR connecting");

  }

  void run(){

      char buffer[sizeof(Lidar::LidarPacket)];

      Lidar::PCDFile file("output.pcd");
      while(true){
        n = read(sockfd, buffer, sizeof(Lidar::LidarPacket));
        while(n != sizeof(Lidar::LidarPacket)){
          n += read(sockfd, buffer+n, sizeof(Lidar::LidarPacket)-n);
        }

        // std::ofstream output("output.dat", std::ofstream::binary);
        // output.write(buffer, sizeof(Lidar::LidarPacket));

        Lidar::LidarPacket* packet = Lidar::storeStructure(buffer);

        if(packet->magicNumber != Lidar::MAGIC_NUMBER){
          std::cout << buffer << std::endl << std::endl;
          std::cout << "Magic number did not match, " << packet->magicNumber
              << " vs " << Lidar::MAGIC_NUMBER << std::endl;
        }else{
          //std::cout << "Packet size: " << packet->messageSize << std::endl;
          for(int i = 0; i < 50; i++){


            for(int angle = 0; angle < 8; angle++){

              double vAngle = M_PI/2 - Lidar::VERTICAL_ANGLES[angle];
              double zAngle = packet->packetData.firingData[i].position*Lidar::ROT;

              double distance = packet->packetData.firingData[i].distances[0][angle] * 1e-5;

              double xPos = distance * sin(vAngle) * cos(zAngle);
              double yPos = distance * sin(vAngle) * sin(zAngle);
              double zPos = distance * cos(vAngle);

              file.writeLine((float)xPos, (float)yPos, (float)zPos);

              if(zAngle > 6)
                std::cout << zAngle << " (" << xPos << ", " << yPos << ", " << zPos << ")\n";
            }

          }
        }
      }

  }
}
