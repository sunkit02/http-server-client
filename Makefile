CC=gcc # C compiler
FLAGS=-g -ggdb

UTILS_TEST_BUILD_DIR=utils/test/build
TEST_LINK_DIR=tests

.PHONY: all
all: build test

.PHONY: build
build: build_tests build_server build_client

.PHONY: test
test: build_tests
	@echo
	@echo "Running parsers_test..."
	@$(UTILS_TEST_BUILD_DIR)/parsers_test
	@echo
	@echo "Running httpheader_list_test..."
	@$(UTILS_TEST_BUILD_DIR)/httpheader_list_test
	@echo
	@echo All tests ran!

.PHONY: build_tests
build_tests: create_test_dir  parsers_test httpheader_list_test

.PHONY: parsers_test
parsers_test: utils/test/parsers_test.c utils/parsers.c utils/httpheader_list.c utils/test/test_utils.c
	@echo "Compiling parsers_test..."
	@$(CC) $(FLAGS) $^ -o $(UTILS_TEST_BUILD_DIR)/$@
	@echo Adding link to tests dir in root...
	@if [ ! -e $(TEST_LINK_DIR)/$@ ]; then ln $(UTILS_TEST_BUILD_DIR)/$@ $(TEST_LINK_DIR); fi

.PHONY: httpheader_list_test
httpheader_list_test: utils/test/httpheader_list_test.c utils/httpheader_list.c utils/test/test_utils.c
	@echo "Compiling httpheader_list_test..."
	@$(CC) $(FLAGS) $^ -o $(UTILS_TEST_BUILD_DIR)/$@
	@echo Adding link to tests dir in root...
	@if [ ! -e $(TEST_LINK_DIR)/$@ ]; then ln $(UTILS_TEST_BUILD_DIR)/$@ $(TEST_LINK_DIR); fi

.PHONY: create_test_dir
create_test_dir:
	@mkdir -p $(UTILS_TEST_BUILD_DIR)
	@mkdir -p $(TEST_LINK_DIR)


.PHONY: build_server
build_server:
	@echo Building server...
	make -C server build

.PHONY: build_client
build_client:
	@echo Building client...
	make -C client build

.PHONY: clean
clean: clean_utils clean_server clean_client

.PHONY: clean_utils
clean:
	@echo Removing utils tests and links...
	@rm -rf $(UTILS_TEST_BUILD_DIR)
	@if [ -e $(TEST_LINK_DIR)/httpheader_list_test ]; then rm $(TEST_LINK_DIR)/httpheader_list_test; fi
	@if [ -e $(TEST_LINK_DIR)/parsers_test ]; then rm $(TEST_LINK_DIR)/parsers_test; fi

.PHONY: clean_server
clean_server:
	@echo Cleaning server...
	make -C server clean

.PHONY: clean_client
clean_client:
	@echo Cleaning client...
	make -C client clean
