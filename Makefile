#소스파일 이름
    SRC = main.c

    #실행파일 이름
    TARGET = test

    #OS 구분
    ifeq ($(OS),Windows_NT)
	    #----- Windows -----
        CC = gcc
        RM = del /Q
        TARGET := $(TARGET).exe
        CFLAGS = -Wall -O2 -D_WIN32
        LIBS = -lws2_32
    else
	    #----- Linux / macOS -----
        CC = gcc
        RM = rm -f
        CFLAGS = -Wall -O2
        LIBS =
    endif

    all: $(TARGET)

    $(TARGET): $(SRC)
	    $(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LIBS)

    run: $(TARGET)
	    ./$(TARGET)

    clean:
	    $(RM) $(TARGET)
