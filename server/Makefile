CC=gcc # C compiler
FLAGS=-g -ggdb

BUILD_DIR=build
BUILD_TARGET=simple_server # Name of build executable

TEST_NAME=servertest # Name of test executable
TEST_DIR=test
TEST_BUILD_DIR=test/build

.PHONY: all
all: build test run

.PHONY: test
test: build-lib create_test_dir $(TEST_BUILD_DIR)/$(TEST_NAME)
	@echo Running server tests...
	@printf "\n\n"
	@$(TEST_BUILD_DIR)/$(TEST_NAME)
	@printf "\n\n"
	@echo All tests passed!

# TODO: Figure out why servertest.c cannot be replaced with $(TEST_NAME)
$(TEST_BUILD_DIR)/$(TEST_NAME): servertest.c $(BUILD_DIR)/server.o $(BUILD_DIR)/parsers.o $(BUILD_DIR)/endpointlist.o
	@echo Compiling tests...
	@$(CC) $(FLAGS) $^ -o $@
	@echo Compiled tests successfully!

# $(TEST_NAME): $(TEST_NAME).c $(BUILD_DIR)/server.o $(BUILD_DIR)/parsers.o $(BUILD_DIR)/endpointlist.o
# 	@echo Compiling tests...
# 	@$(CC) $(FLAGS) $^ -o $@
# 	@echo Compiled tests successfully!

create_test_dir:
	@echo Creating test directories...
	@mkdir -p $(TEST_DIR)
	@mkdir -p $(TEST_BUILD_DIR)


.PHONY: run
run: build
	@$(BUILD_DIR)/$(BUILD_TARGET)


.PHONY: build
build: build-lib build-server


.PHONY: build-server
build-server: main.c $(BUILD_DIR)/server.o $(BUILD_DIR)/parsers.o $(BUILD_DIR)/endpointlist.o $(BUILD_DIR)/httpheader_list.o
	@echo Compiling and linking server executable...
	@$(CC) $(FLAGS) $^ -o $(BUILD_DIR)/$(BUILD_TARGET)
	@echo Built server successfully!


.PHONY: build-lib
build-lib: create_build_dir $(BUILD_DIR)/server.o $(BUILD_DIR)/parsers.o $(BUILD_DIR)/endpointlist.o $(BUILD_DIR)/httpheader_list.o
	@echo Library built successfully!

$(BUILD_DIR)/server.o: server.c 
	@echo Compiling $@...
	@$(CC) $(FLAGS) -c $< -o $@ 

$(BUILD_DIR)/httpheader_list.o: ../utils/httpheader_list.c 
	@echo Compiling $@...
	@$(CC) $(FLAGS) -c $< -o $@ 

$(BUILD_DIR)/parsers.o: ../utils/parsers.c 
	@echo Compiling $@...
	@$(CC) $(FLAGS) -c $< -o $@ 

$(BUILD_DIR)/endpointlist.o: datastructures/endpointlist.c 
	@echo Compiling $@...
	@$(CC) $(FLAGS) -c $< -o $@ 


create_build_dir: 
	@mkdir -p $(BUILD_DIR)


.PHONY: clean
clean:
	@rm -rf $(BUILD_DIR) $(TEST_BUILD_DIR)


	
