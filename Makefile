.PHONY: all
all: test build

.PHONY: test
test: utils/test/httpheader_list_test.c utils/httpheader_list.c
	@gcc $^ -o utils/test/httpheader_list_test
	@echo "Compiled httpheader_list_test"
	@echo
	@utils/test/httpheader_list_test


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
	@rm utils/test/httpheader_list_test
