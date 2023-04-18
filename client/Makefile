CC=gcc # C compiler
FLAGS=-g -ggdb

BUILD_DIR=build
BUILD_TARGET=simple_client # Name of build executable

TEST_NAME=clienttest # Name of test executable
TEST_DIR=test
TEST_BUILD_DIR=test/build


.PHONY: all
all: build test run


.PHONY: run
run: build
	@$(BUILD_DIR)/$(BUILD_TARGET)


.PHONY: build
build: build-lib build-client


.PHONY: build-client
build-client: main.c $(BUILD_DIR)/client.o $(BUILD_DIR)/parsers.o $(BUILD_DIR)/httpheader_list.o
	@echo Compiling and linking client executable...
	@$(CC) $(FLAGS) $^ -o $(BUILD_DIR)/$(BUILD_TARGET)
	@echo Built client successfully!


.PHONY: build-lib
build-lib: create_build_dir $(BUILD_DIR)/client.o $(BUILD_DIR)/parsers.o $(BUILD_DIR)/httpheader_list.o
	@echo Library built successfully!
	 
$(BUILD_DIR)/client.o: client.c
	@echo Compiling $@...
	@$(CC) $(FLAGS) -c $< -o $@ 

$(BUILD_DIR)/httpheader_list.o: ../lib/httpheader_list.c
	@echo Compiling $@...
	@$(CC) $(FLAGS) -c $< -o $@ 

$(BUILD_DIR)/parsers.o: ../lib/parsers.c 
	@echo Compiling $@...
	@$(CC) $(FLAGS) -c $< -o $@ 

create_build_dir:
	@mkdir -p $(BUILD_DIR)


.PHONY: test
test:
	@echo Running client tests...

.PHONY: clean
clean: 
	@echo Removing object files and executables...
	@rm -rf build/
