CC = g++
CFLAGS = -c -Wall -Iheaders -g -std=c++11
SOURCE_DIR = src/

all: typing-assistant

typing-assistant: main.o Trie.o InnerNode.o LeafNode.o Recommendations.o
	$(CC) -o typing-assistant main.o Trie.o InnerNode.o LeafNode.o Recommendations.o

main.o: $(SOURCE_DIR)main.cpp
	$(CC) $(CFLAGS) $(SOURCE_DIR)main.cpp

Trie.o: $(SOURCE_DIR)Trie.cpp
	$(CC) $(CFLAGS) $(SOURCE_DIR)Trie.cpp

InnerNode.o: $(SOURCE_DIR)InnerNode.cpp
	$(CC) $(CFLAGS) $(SOURCE_DIR)InnerNode.cpp

LeafNode.o: $(SOURCE_DIR)LeafNode.cpp
	$(CC) $(CFLAGS) $(SOURCE_DIR)LeafNode.cpp 

Recommendations.o: $(SOURCE_DIR)Recommendations.cpp
	$(CC) $(CFLAGS) $(SOURCE_DIR)Recommendations.cpp


clean:
	rm -rf typing-assistant *.o
