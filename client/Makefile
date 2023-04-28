CC=clang # C compiler
FLAGS=-g -O0

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


.PHONY: test
test: create_test_build_dir create_build_dir build_lib build_test
	@echo
	@echo Running client test...
	@$(TEST_BUILD_DIR)/$(TEST_NAME)
	@echo
	@echo Test run successfully!


.PHONY: build
build: create_build_dir create_test_build_dir build_lib build_test build-client

.PHONY: build_test
build_test: client_test.c $(BUILD_DIR)/client.o $(BUILD_DIR)/parsers.o $(BUILD_DIR)/httpheader_list.o $(BUILD_DIR)/http_utils.o
	@echo Compiling and linking client test...
	@$(CC) $(FLAGS) $^ -o $(TEST_BUILD_DIR)/$(TEST_NAME)
	@echo Built client test successfully!


.PHONY: build-client
build-client: main.c $(BUILD_DIR)/client.o $(BUILD_DIR)/parsers.o $(BUILD_DIR)/httpheader_list.o $(BUILD_DIR)/http_utils.o $(BUILD_DIR)/base64.o $(BUILD_DIR)/blackJack.o
	@echo Compiling and linking client executable...
	@$(CC) $(FLAGS) $^ -o $(BUILD_DIR)/$(BUILD_TARGET)
	@echo Built client successfully!

.PHONY: build_lib
build_lib: create_build_dir $(BUILD_DIR)/client.o $(BUILD_DIR)/parsers.o $(BUILD_DIR)/httpheader_list.o $(BUILD_DIR)/http_utils.o $(BUILD_DIR)/base64.o $(BUILD_DIR)/blackJack.o
	@echo Library built successfully!
	 
$(BUILD_DIR)/client.o: client.c
	@echo Compiling $@...
	@$(CC) $(FLAGS) -c $< -o $@ 

$(BUILD_DIR)/httpheader_list.o: ../lib/httpheader_list.c
	@echo Compiling $@...
	@$(CC) $(FLAGS) -c $< -o $@ 

$(BUILD_DIR)/http_utils.o: ../lib/http_utils.c
	@echo Compiling $@...
	@$(CC) $(FLAGS) -c $< -o $@ 

$(BUILD_DIR)/parsers.o: ../lib/parsers.c 
	@echo Compiling $@...
	@$(CC) $(FLAGS) -c $< -o $@ 

$(BUILD_DIR)/base64.o: ../lib/base64.c 
	@echo Compiling $@...
	@$(CC) $(FLAGS) -c $< -o $@ 

$(BUILD_DIR)/blackJack.o: ../lib/blackJack.c 
	@echo Compiling $@...
	@$(CC) $(FLAGS) -c $< -o $@ 

create_build_dir:
	@mkdir -p $(BUILD_DIR)

create_test_build_dir:
	@mkdir -p $(TEST_BUILD_DIR)

.PHONY: clean
clean: 
	@echo Removing object files and executables...
	@rm -rf $(BUILD_DIR)
	@rm -rf $(TEST_BUILD_DIR)
