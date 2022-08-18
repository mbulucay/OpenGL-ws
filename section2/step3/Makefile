dep=-Wall -lglfw -lGL -lGLU -lglut -lGLEW -lX11 -lXxf86vm -lXrandr -lpthread -lXi -lrt -ldl -lXinerama -lXcursor

translate:
	g++ translate.cpp -o translate ${dep}

rotate:
	g++ rotate.cpp -o rotate ${dep}

scale:
	g++ scale.cpp -o scale ${dep}

clean:
	rm -f translate rotate scale