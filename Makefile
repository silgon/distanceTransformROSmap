CXXFLAGS = -std=c++11
CXX = g++

distancetransform: distancetransform.cpp
	$(CXX) distancetransform.cpp -o distancetransform $(CXXFLAGS) `pkg-config --cflags opencv` `pkg-config --libs opencv`

distancetransforminmeters: distancetransforminmeters.cpp
	$(CXX) distancetransforminmeters.cpp -o distancetransforminmeters $(CXXFLAGS) `pkg-config --cflags opencv` `pkg-config --libs opencv` `pkg-config --cflags yaml-cpp` `pkg-config --libs yaml-cpp`

run_simple: distancetransform
	./distancetransform map.pgm

run_data: distancetransforminmeters
	./distancetransforminmeters map.yaml
