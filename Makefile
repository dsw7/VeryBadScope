.PHONY = help check-env compile upload

LIGHT_PURPLE = "\033[1;1;35m"
NO_COLOR = "\033[0m"
FULLY_QUALIFIED_BOARD_NAME = arduino:avr:uno
PATH_INO_SRC = src/ino
BUILD_PATH = $(TMP)/inodaq2-build/
CORE_CACHE_PATH = $(TMP)/inodaq2-core-cache/

define MESSAGE
	@echo -e $(LIGHT_PURPLE)\> $(1)$(NO_COLOR)
endef

define HELP_LIST_TARGETS
To display all targets:
    $$ make help
Compile Arduino code:
    $$ make compile
Upload compiled Arduino code to board:
    $$ make upload
endef

export HELP_LIST_TARGETS

help:
	@echo "$$HELP_LIST_TARGETS"

check-env:
ifndef SERIAL_PORT
	$(error SERIAL_PORT is not set. Run "make <target> SERIAL_PORT=<serial-port-or-device-file>")
endif

compile: check-env
	$(call MESSAGE,Compiling Arduino code)
	@arduino-cli compile \
	--port $(SERIAL_PORT) \
	--fqbn $(FULLY_QUALIFIED_BOARD_NAME) \
	--verbose \
	--build-path=$(BUILD_PATH) \
	--build-cache-path=$(CORE_CACHE_PATH) \
	$(PATH_INO_SRC)/

upload: compile
	$(call MESSAGE,Uploading Arduino code)
	@arduino-cli upload \
	--port $(SERIAL_PORT) \
	--fqbn $(FULLY_QUALIFIED_BOARD_NAME) \
	--verbose \
	--input-dir=$(BUILD_PATH) \
	$(PATH_INO_SRC)/

test: upload
	$(call MESSAGE,Running unit tests)
	@python3 -m pytest --verbose --capture=no .
