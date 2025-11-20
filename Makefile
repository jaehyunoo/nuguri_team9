# 프로젝트 이름 (생성될 실행 파일 이름)
TARGET = picture_maker

# 소스 파일
SRC = main.c

# 컴파일러
CC = gcc

# 컴파일 옵션
CFLAGS = -Wall -O2

# 실행 파일 생성 규칙
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

# 실행 (make run)
run: $(TARGET)
	./$(TARGET)

# 정리 (make clean)
clean:
	rm -f $(TARGET)
