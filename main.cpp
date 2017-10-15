#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <iostream>
#include <cstdint>
#include <cmath>
#include <fstream>
#include <thread>

#include "packet.h"
#include "PCDFile.h"
#include "GPSInput.h"

char hostname[] = "10.42.0.84";
int portno = 4141;



int main(){
  GPSInput gps;
  std::thread gpsThread([gps] {gps.run();});



  LidarInput lidar;
  std::thread lidarThread([lidar] {lidar.run();});

  return 0;
}
