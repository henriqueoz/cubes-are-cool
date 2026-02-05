PROJECT_NAME = fps
BUILD_DIR = build
PLATFORM = windows

.PHONY: build configure run

configure:
	mkdir $(BUILD_DIR)
	cmake -B $(BUILD_DIR) -S . -G "Ninja" -DCMAKE_BUILD_TYPE=Debug

build:
	@cmake --build $(BUILD_DIR)

run:
ifeq ($(PLATFORM), windows)
	@./$(BUILD_DIR)/bin/$(PROJECT_NAME).exe
else
	@cd $(BUILD_DIR)/bin && ./$(PROJECT_NAME)
endif