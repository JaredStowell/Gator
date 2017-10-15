#include <iostream>
#include "GridViewer.h"

GLuint GridViewer::loadShader(std::string path, int shaderType){
	std::ifstream shaderSource(path);

	shaderSource.seekg(0, shaderSource.end);
	int length = shaderSource.tellg();
	shaderSource.seekg(0, shaderSource.beg);

	char* source = new char[length+1];
	shaderSource.read(source, length);
	source[length] = '\0';
	shaderSource.close();

	GLuint shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &source, NULL);

	delete[] source;

	glCompileShader(shader);

	GLint vShaderCompiled = GL_FALSE;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &vShaderCompiled);

	if(vShaderCompiled != GL_TRUE){
		std::cout << "There was an error and the vertex shader couldn't be compiled... sorry.\n";
		printShaderLog(shader);
		return false;
	}
	glAttachShader(gProgramID, shader);

	return shader;
}

bool GridViewer::initGL(){
	bool success = true;

	gProgramID = glCreateProgram();


	GLuint vShader = loadShader("vertex.shader", GL_VERTEX_SHADER);
	GLuint fShader = loadShader("fragment.shader", GL_FRAGMENT_SHADER);
	GLuint gShader = loadShader("geometry.shader", GL_GEOMETRY_SHADER);

	glLinkProgram( gProgramID );

	GLint programSuccess = GL_TRUE;
	glGetProgramiv(gProgramID, GL_LINK_STATUS, &programSuccess);
	if(programSuccess != GL_TRUE){
		std::cout << "Unable to link shader program :( \n";
		printProgramLog(gProgramID);
		return false;
	}

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_MULTISAMPLE);
  // glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
  // glHint( GL_POLYGON_SMOOTH_HINT, GL_NICEST );


	return true;
}


void GridViewer::printProgramLog(GLuint program) {
    if(glIsProgram(program)) {
        int infoLogLength = 0;
        int maxLength = infoLogLength;
        glGetProgramiv( program, GL_INFO_LOG_LENGTH, &maxLength );
        char* infoLog = new char[ maxLength ];
        glGetProgramInfoLog( program, maxLength, &infoLogLength, infoLog );
        if(infoLogLength > 0) {
            printf("%s\n", infoLog);
        }
        delete[] infoLog;
    }
    else {
        printf("Name %d is not a program\n", program );
    }
}

void GridViewer::printShaderLog(GLuint shader) {
    if(glIsShader(shader)) {
        int infoLogLength = 0;
        int maxLength = infoLogLength;
        glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &maxLength );
        char* infoLog = new char[ maxLength ];
        glGetShaderInfoLog( shader, maxLength, &infoLogLength, infoLog );
        if(infoLogLength > 0) {
            printf( "%s\n", infoLog );
        }
        delete[] infoLog;
    } else {
        printf( "Name %d is not a shader\n", shader );
    }
}




void GridViewer::init(){

	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );

	SDL_Init(SDL_INIT_VIDEO);


  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
  //SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	gWindow = SDL_CreateWindow(
    "SDL2/OpenGL Demo", 0, 0, 640, 480,
    SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

	SDL_GLContext gContext = SDL_GL_CreateContext( gWindow );
	if( gContext == NULL ) {
		printf( "OpenGL context could not be created! SDL Error: %s\n", SDL_GetError() );
		return;
	}
	else {
		//Initialize GLEW
		glewExperimental = GL_TRUE;
		GLenum glewError = glewInit();

		if( glewError != GLEW_OK ) {
			printf( "Error initializing GLEW! %s\n", glewGetErrorString( glewError ) );
		}

		//Use Vsync
		if( SDL_GL_SetSwapInterval( 1 ) < 0 ) {
			printf( "Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError() );
		}

		//Initialize OpenGL
		if( !initGL() )	{
			printf( "Unable to initialize OpenGL!\n" );
			return;
		}

	}
}


void GridViewer::render()
{
    //Clear color buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Bind program
    glUseProgram( gProgramID );

		glm::mat4 view;

		glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

		// Projection
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(45.0f), (GLfloat)640 / (GLfloat)480, 0.01f, 100000.0f);
		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(gProgramID, "model");
		GLint viewLoc = glGetUniformLocation(gProgramID, "view");
		GLint projLoc = glGetUniformLocation(gProgramID, "projection");
		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


		static glm::vec3 cubePositions[] = {
					glm::vec3( 0.0f,  0.0f,  0.0f),
					glm::vec3( 2.0f,  5.0f, -15.0f),
					glm::vec3(-1.5f, -2.2f, -2.5f),
					glm::vec3(-3.8f, -2.0f, -12.3f),
					glm::vec3( 2.4f, -0.4f, -3.5f),
					glm::vec3(-1.7f,  3.0f, -7.5f),
					glm::vec3( 1.3f, -2.0f, -2.5f),
					glm::vec3( 1.5f,  2.0f, -2.5f),
					glm::vec3( 1.5f,  0.2f, -1.5f),
					glm::vec3(-1.3f,  1.0f, -1.5f)
			};


		glBindVertexArray(VAO);
		for (GLuint i = 0; i < 10; i++)
		{
				// Calculate the model matrix for each object and pass it to shader before drawing
				glm::mat4 model;
				model = glm::translate(model, cubePositions[i]);
				GLfloat angle = 20.0f * i;
				model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

				glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);


    //Unbind program
    glUseProgram( NULL );

		SDL_GL_SwapWindow( gWindow );
}

void GridViewer::setup(){

	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat vertices[] = {
		-1.0f,-1.0f,-1.0f, // triangle 1 : begin
    -1.0f,-1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f, // triangle 1 : end
    1.0f, 1.0f,-1.0f, // triangle 2 : begin
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f, // triangle 2 : end
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f
	};


	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0); // Unbind VAO
}

void GridViewer::update(){

}
