CC      = g++
CFLAGS  = -O3 -std=c++17
TARGET  = a.out
SRCS    = main.cpp \
		  Labeling.cpp \
		  IDManager.cpp \
		  FaceManager.cpp \
		  EdgeManager.cpp \
		  VertexManager.cpp \
		  UpdateID.cpp
OBJS    = $(SRCS:.cpp=.o)
DEPS	= UnfoldingDimensions.hpp \
		  Correspondence.hpp \
		  AdjacentList.hpp \
		  Labeling.hpp \
		  IDManager.hpp \
		  FaceManager.hpp \
		  EdgeManager.hpp \
		  VertexManager.hpp \
		  UpdateID.hpp \
		  ArraySize.hpp \
		  FillingBorderFaces.hpp \
		  Direction.hpp \
		  Coordinates.hpp

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f a.out $(OBJS) $(TARGET)
