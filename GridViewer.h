#include <inttypes.h>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <fstream>
#include <string>



class GridViewer{
private:
  static const int xSize = 1000;
  static const int ySize = 1000;
  static const int zSize = 400;

  //uint8_t grid[xSize][ySize][zSize];


  GLuint gProgramID = 0;
  GLuint gVBO = 0;
  GLuint gIBO = 0;
  SDL_Window *gWindow ;

  glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f,  3.0f);

  bool initGL();
  void printProgramLog(GLuint program);
  void printShaderLog(GLuint shader);
  GLuint loadShader(std::string path, int shaderType);


  GLuint VBO, VAO;



public:

  void init();
  void render();
  void setup();
  void update();

  glm::vec3 getCameraPos(){
    return cameraPos;
  }

  void setCameraPos(glm::vec3 pos){
    cameraPos = pos;
  }

};
