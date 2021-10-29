.PHONY: all

CXX = clang++
CXXFLAGS = -std=c++17 -g
LIBS= -L/opt/homebrew/Cellar/sfml/2.5.1_1/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
MACFLAGS = -target arm64-apple-macos10.7 -framework Cocoa -framework Foundation
SRCS = src/app/app.cpp src/engine/engine.cpp main.cpp
OBJS = src/app/app.o src/engine/engine.o main.o

all: sfml-app

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ -I/opt/homebrew/Cellar/sfml/2.5.1_1/include/ -I src/

sfml-app: $(OBJS)
	$(CXX) $(OBJS) -g -o sfml-app -L/opt/homebrew/Cellar/sfml/2.5.1_1/lib -F/Library/Frameworks/ $(LIBS) $(MACFLAGS)

clean: 
	rm -f $(OBJS) sfml-app

