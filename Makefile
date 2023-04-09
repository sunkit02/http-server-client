CC=gcc # C compiler
FLAGS=-g -ggdb

.PHONY: all
all: test build

.PHONY: test
test: create_test_dir test-httpheader-list test-parsers
	@echo All tests ran!

.PHONY: test-httpheader-list
test-httpheader-list: utils/test/httpheader_list_test.c utils/httpheader_list.c
	@$(CC) $(FLAGS) $^ -o utils/test/build/httpheader_list_test
	@echo "Compiled httpheader_list_test"
	@printf "Running httpheader_list_test...\n"
	@utils/test/build/httpheader_list_test

.PHONY: test-parsers
test-parsers: utils/test/parsers_test.c utils/parsers.c utils/httpheader_list.c
	@$(CC) $(FLAGS) $^ -o utils/test/build/parsers_test
	@echo "Compiled parsers_test"
	@echo "Running parsers_test..."
	@utils/test/build/parsers_test

create_test_dir:
	@mkdir -p utils/test/build

.PHONY: build
build: build-server build-client

.PHONY: build-server
build-server:
	@echo Building server...
	make -C server build

.PHONY: build-client
build-client:
	@echo Building client...
	make -C client build

.PHONY: clean
clean: clean-utils

.PHONY: clean-utils
clean:
	@echo Removing utils tests...
	@rm utils/test/build/httpheader_list_test utils/test/build/parsers_test
