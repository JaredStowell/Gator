#include <string>
#include <fstream>

namespace Lidar{

  class PCDFile{
  private:
    std::ofstream outputFile;
    int numPoints = 0;
  public:
    PCDFile(std::string filename){
      outputFile.open(filename);
      writeHeader();
    }

    void writeHeader(){
      outputFile << "VERSION .7\n";
      outputFile << "FIELDS x y z\n";
      outputFile << "SIZE 4 4 4\n";
      outputFile << "TYPE F F F\n";
      outputFile << "COUNT 1 1 1\n";
      outputFile << "WIDTH x\n";
      outputFile << "HEIGHT 1\n";
      outputFile << "VIEWPOINT 0 0 0 1 0 0 0\n";
      outputFile << "POINTS x\n";
      outputFile << "DATA ascii\n";
    }

    void writeLine(float x, float y, float z){
      outputFile << x << " " << y << " " << z << std::endl;
      numPoints++;
    }

    void finalize(){
      outputFile << numPoints;
      outputFile.close();
    }

  };

}
