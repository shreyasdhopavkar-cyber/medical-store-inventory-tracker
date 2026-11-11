# Compilers
CC = gcc
CXX = g++

# Compiler flags
CFLAGS = -Wall -Wextra -Wpedantic -std=c11
CXXFLAGS = -Wall -Wextra -std=c++14

# Directories
SRC_DIR = src
INC_DIR = include
TEST_DIR = tests

# Include path
INCLUDES = -I$(INC_DIR)

# Source files
APP_SRCS = $(SRC_DIR)/main.c $(SRC_DIR)/medicine.c $(SRC_DIR)/validation.c \
           $(SRC_DIR)/file_handler.c $(SRC_DIR)/report.c

# Library sources (everything except main.c, used by tests)
LIB_SRCS = $(SRC_DIR)/medicine.c $(SRC_DIR)/validation.c \
           $(SRC_DIR)/file_handler.c $(SRC_DIR)/report.c

# Object files
APP_OBJS = $(APP_SRCS:.c=.o)
LIB_OBJS = $(LIB_SRCS:.c=.o)

# Test sources
TEST_SRCS = $(TEST_DIR)/test_validation.cpp $(TEST_DIR)/test_medicine.cpp \
            $(TEST_DIR)/test_report.cpp

# Output executables
APP_NAME = medstore
TEST_NAME = test_medstore

# Google Test linking flags
GTEST_LIBS = -lgtest -lgtest_main -lpthread

# ---- Targets ----

all: $(APP_NAME)

$(APP_NAME): $(APP_OBJS)
	$(CC) $(CFLAGS) $(APP_OBJS) -o $(APP_NAME)

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

test: $(LIB_OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(TEST_SRCS) $(LIB_OBJS) $(GTEST_LIBS) -o $(TEST_NAME)
	./$(TEST_NAME)

run: $(APP_NAME)
	./$(APP_NAME)

clean:
ifeq ($(OS),Windows_NT)
	-del /Q src\*.o $(APP_NAME).exe $(TEST_NAME).exe 2>nul
else
	rm -f $(SRC_DIR)/*.o $(APP_NAME) $(TEST_NAME)
endif

.PHONY: all test run clean
