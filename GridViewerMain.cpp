#include "GridViewer.h"
#include <chrono>
#include <thread>

const float MOVE_SPEED = .1;

int main(){
	GridViewer gridViewer;
	gridViewer.init();
  gridViewer.setup();

	SDL_Event event;
	SDL_StartTextInput();

	bool quit = false;
	while(!quit){
		static bool w_down = false;
		static bool a_down = false;
		static bool s_down = false;
		static bool d_down = false;


		glm::vec3 pos = gridViewer.getCameraPos();

		while( SDL_PollEvent( &event ) != 0 ) {
			switch(event.type){
				case SDL_KEYDOWN:
					if(SDLK_w == event.key.keysym.sym)
						w_down = true;
					if(SDLK_a == event.key.keysym.sym)
						a_down = true;
					if(SDLK_s == event.key.keysym.sym)
						s_down = true;
					if(SDLK_d == event.key.keysym.sym)
						d_down = true;
					break;
				case SDL_KEYUP: 
					if(SDLK_w == event.key.keysym.sym)
						w_down = false;
					if(SDLK_a == event.key.keysym.sym)
						a_down = false;
					if(SDLK_s == event.key.keysym.sym)
						s_down = false;
					if(SDLK_d == event.key.keysym.sym)
						d_down = false;
					break;
				case SDL_QUIT:
					quit = true;
					break;

				default:
					break;
			}
		}

		pos.z -= w_down*MOVE_SPEED;
		pos.z += s_down*MOVE_SPEED;
		pos.x -= a_down*MOVE_SPEED;
		pos.x += d_down*MOVE_SPEED;

		gridViewer.setCameraPos(pos);

    gridViewer.update();
		gridViewer.render();
		std::this_thread::sleep_for (std::chrono::milliseconds(10));
	}
}
