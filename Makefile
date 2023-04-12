FILES = MyMalloc.c main.c
DEBUG_CFLAG = -g -Wall -Werror -include
NON_DEBUG_CFLAG = -Wall -include
TARGET = prog

$(TARGET): $(FILES)
	gcc $(DEBUG_CFLAG) $(FILES) -o $(TARGET)

libcreate:
	gcc $(NON_DEBUG_CFLAG) $(FILES) -o $(TARGET)

clean:
	rm $(TARGET)
