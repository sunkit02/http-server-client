CC=gcc # C compiler
FLAGS=-g -ggdb

BUILD_DIR=build
BUILD_TARGET=simple_server # Name of build executable

TEST_NAME=servertest # Name of test executable
TEST_DIR=test
TEST_BUILD_DIR=test/build
TEST_LINK_DIR=../tests

.PHONY: all
all: build test run

.PHONY: test
test: build_lib create_test_dir $(TEST_BUILD_DIR)/$(TEST_NAME)
	@echo Running server tests...
	@printf "\n\n"
	@$(TEST_BUILD_DIR)/$(TEST_NAME)
	@printf "\n\n"
	@echo All tests passed!

.PHONY: build
build: build_lib build_server create_test_dir $(TEST_BUILD_DIR)/$(TEST_NAME)

# TODO: Figure out why servertest.c cannot be replaced with $(TEST_NAME)
$(TEST_BUILD_DIR)/$(TEST_NAME): servertest.c $(BUILD_DIR)/server.o $(BUILD_DIR)/parsers.o $(BUILD_DIR)/endpointlist.o $(BUILD_DIR)/httpheader_list.o
	@echo Compiling tests...
	@$(CC) $(FLAGS) $^ -o $@
	@echo Compiled tests successfully!
	@echo Adding link to tests dir in root...
	@if [ -e $(TEST_BUILD_DIR)/$(TEST_NAME) ]; then ln $@ $(TEST_LINK_DIR); fi

.PHONY: create_test_dir
create_test_dir:
	@echo Creating test directories...
	@mkdir -p $(TEST_DIR)
	@mkdir -p $(TEST_BUILD_DIR)
	@echo


.PHONY: run
run: build
	@echo
	@$(BUILD_DIR)/$(BUILD_TARGET)


.PHONY: build_server
build_server: main.c $(BUILD_DIR)/server.o $(BUILD_DIR)/parsers.o $(BUILD_DIR)/endpointlist.o $(BUILD_DIR)/httpheader_list.o $(BUILD_DIR)/http_utils.o
	@echo Compiling and linking server executable...
	@$(CC) $(FLAGS) $^ -o $(BUILD_DIR)/$(BUILD_TARGET)
	@echo Built server successfully!
	@echo


.PHONY: build_lib
build_lib: create_build_dir $(BUILD_DIR)/server.o $(BUILD_DIR)/parsers.o $(BUILD_DIR)/endpointlist.o $(BUILD_DIR)/httpheader_list.o $(BUILD_DIR)/http_utils.o
	@echo Library built successfully!
	@echo

$(BUILD_DIR)/server.o: server.c 
	@echo Compiling $@...
	@$(CC) $(FLAGS) -c $< -o $@ 

$(BUILD_DIR)/httpheader_list.o: ../lib/httpheader_list.c 
	@echo Compiling $@...
	@$(CC) $(FLAGS) -c $< -o $@ 

$(BUILD_DIR)/parsers.o: ../lib/parsers.c 
	@echo Compiling $@...
	@$(CC) $(FLAGS) -c $< -o $@ 

$(BUILD_DIR)/endpointlist.o: datastructures/endpointlist.c 
	@echo Compiling $@...
	@$(CC) $(FLAGS) -c $< -o $@ 

$(BUILD_DIR)/http_utils.o: ../lib/http_utils.c
	@echo Compiling $@...
	@$(CC) $(FLAGS) -c $< -o $@ 

create_build_dir: 
	@mkdir -p $(BUILD_DIR)


.PHONY: clean
clean:
	@echo Removing object files and executables...
	@rm -rf $(BUILD_DIR) $(TEST_BUILD_DIR)
	@echo Removing server test and links...
	@if [ -f $(TEST_LINK_DIR)/$(TEST_NAME) ]; then rm $(TEST_LINK_DIR)/$(TEST_NAME); fi
