CC = gcc 
CFLAGS = -Wall `pkg-config --cflags gtk+-3.0`
LIBS = `pkg-config --libs gtk+-3.0` -lm
HOME_DIR = ~
SRC_DIR = $(HOME_DIR)/Proiecte/Project_2/src
SOURCES = $(SRC_DIR)/gui.c $(SRC_DIR)/functions.c $(SRC_DIR)/main.c
EXECUTABLE = program

all: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(EXECUTABLE) $(LIBS)

clean:
	rm -f $(EXECUTABLE)
