#include <cstdint>
#include <cmath>

namespace Lidar{
  uint32_t MAGIC_NUMBER = 0x75bd7e97;
  double ROT = (1/10400.0f) * 360.0f * (M_PI/180.0) ;


  const double VERTICAL_ANGLES[] = {
    -0.318505,
    -0.2692,
    -0.218009,
    -0.165195,
    -0.111003,
    -0.0557982,
    0.f,
    0.0557982
  };



  struct FiringData {
    uint16_t position;
    uint16_t reserved;
    uint32_t distances[3][8];
    uint8_t intensities[3][8];
    uint8_t status[8];
  };

  struct PacketData {
    FiringData firingData[50];

    uint32_t timestampS;
    uint32_t timestampNS;

    uint16_t apiVersion;
    uint16_t status;
  };

  struct LidarPacket {
    uint32_t magicNumber;
    uint32_t messageSize;

    uint32_t timestampS;
    uint32_t timestampNS;

    uint8_t apiVersionMajor;
    uint8_t apiVersionMinor;
    uint8_t apiVersionPatch;

    uint8_t packetType;

    PacketData packetData;
  };



    LidarPacket* storeStructure(char buffer[sizeof(LidarPacket)]){
        LidarPacket* packet = (LidarPacket*)buffer;

        for(int i = 0; i < 50; i++){
          //std::cout << packet->packetData.firingData[i].position << std::endl;
          packet->packetData.firingData[i].position = ntohs(packet->packetData.firingData[i].position);
          packet->packetData.firingData[i].reserved = ntohs(packet->packetData.firingData[i].reserved );
          for(int j = 0; j < 3; j++){
            for(int k = 0; k < 8; k++){
              packet->packetData.firingData[i].distances[j][k] = ntohl(packet->packetData.firingData[i].distances[j][k]);
            }
          }
        }

        packet->packetData.timestampS = ntohl(packet->packetData.timestampS);
        packet->packetData.timestampNS = ntohl(packet->packetData.timestampNS);
        packet->packetData.apiVersion = ntohs(packet->packetData.apiVersion);
        packet->packetData.status = ntohs(packet->packetData.status);

        packet->magicNumber = ntohl(packet->magicNumber);
        packet->messageSize = ntohl(packet->messageSize);
        packet->timestampS = ntohl(packet->timestampS);
        packet->timestampNS = ntohl(packet->timestampNS);
        return packet;
    }

}
