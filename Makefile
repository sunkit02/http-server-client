.PHONY: all
all: build


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
