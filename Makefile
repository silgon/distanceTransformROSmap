NAME = distancetransform.cpp
OUTPUT = distancetransform
comp:
	g++ $(NAME) -o $(OUTPUT) `pkg-config --cflags opencv` `pkg-config --libs opencv`
run: $(OUTPUT)
	./$(OUTPUT)
