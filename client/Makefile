CC=gcc # C compiler

BUILD_DIR=build
BUILD_TARGET=simple_client # Name of build executable

TEST_NAME=clienttest # Name of test executable
TEST_DIR=test
TEST_BUILD_DIR=test/build


.PHONY: all
all: build test run

.PHONY: build
build: main.c build-client
	@echo Compiling and linking client executable...
	@$(CC) $< -o $(BUILD_DIR)/$(BUILD_TARGET)

.PHONY: build-client
build-client: create_build_dir
	@echo Building client...

.PHONY: build-lib
build-lib:
	@echo Library built successfully!
	 
create_build_dir:
	@mkdir -p $(BUILD_DIR)

.PHONY: test
test:
	@echo Running client tests...

.PHONY: run
run:
	@$(BUILD_DIR)/$(BUILD_TARGET)
