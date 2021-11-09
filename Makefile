.PHONY: all

CXX = clang++
#CXXFLAGS = -std=c++17 -g -I/opt/homebrew/Cellar/sfml/2.5.1_1/include/
CXXFLAGS = -std=c++17 -g -I/Users/dev/Documents/SFML/SFML-master/include/
LIBS= -L/Users/dev/Documents/SFML/SFML-master/cmake/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
MACFLAGS = -target arm64-apple-macos10.7 -framework Cocoa -framework Foundation
SRCS = TeddyRL/TeddyRL/app/app.cpp TeddyRL/TeddyRL/engine/engine.cpp TeddyRL/TeddyRL/src/main.cpp TeddyRL/TeddyRL/ResourcePath.mm
OBJS = TeddyRL/TeddyRL/app/app.o TeddyRL/TeddyRL/engine/engine.o TeddyRL/TeddyRL/main.o
LFLAGS = -L/Users/dev/Documents/SFML/SFML-master/cmake/lib 
#LFLAGS = -L/opt/homebrew/Cellar/sfml/2.5.1_1/lib 

all: sfml-app

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ -I TeddyRL/TeddyRL/

sfml-app: $(OBJS)
	$(CXX) $(OBJS) -g -o sfml-app $(LFLAGS) -F/Library/Frameworks/ $(LIBS) $(MACFLAGS)

# $(CXX) $(OBJS) -g -o sfml-app -L/Users/dev/Documents/SFML/SFML-master/cmake/lib $(LIBS) $(MACFLAGS)
# $(CXX) $(OBJS) -c src/app/app.cpp -I/Users/dev/Documents/SFML/SFML-master/include/
# $(CXX) $(OBJS) -c src/engine/engine.cpp -I/Users/dev/Documents/SFML/SFML-master/include/
# $(CXX) $(OBJS) -c main.cpp -I/Users/dev/Documents/SFML/SFML-master/include/

clean: 
	rm -f $(OBJS) sfml-app
