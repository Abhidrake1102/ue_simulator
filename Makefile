CXX = g++

CXXFLAGS = -std=c++11 -Wall -Wextra

TARGET = ue_simulator

OBJECTS = ue_client.o MessageSequencer.o

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET)

ue_client.o: ue_client.cpp message.h MessageSequencer.h
	$(CXX) $(CXXFLAGS) -c ue_client.cpp

MessageSequencer.o: MessageSequencer.cpp MessageSequencer.h message.h
	$(CXX) $(CXXFLAGS) -c MessageSequencer.cpp

clean:
	rm -f $(OBJECTS) $(TARGET)
