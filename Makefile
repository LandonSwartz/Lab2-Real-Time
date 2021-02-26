CC = gcc
CFLAGS = #-Wall
LDFLAGS=-lpthread -lrt
OBJFILES = Lab2_Part2.o
TARGET = Lab2_Part2
	
$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)
clean:
	rm -f $(OBJFILES) $(TARGET) *~
