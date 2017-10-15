viewer:
	g++ -std=c++11 GridViewer.cpp GridViewerMain.cpp -o viewer -lGLEW -lGL -lSDL2 

lidar:
	g++ -std=c++11 main.cpp -o lidar

clean:
	rm lidar viewer
