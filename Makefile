PROJECT_NAME = fps
BUILD_DIR = build

.PHONY: build configure run

configure:
	mkdir $(BUILD_DIR)
	cmake -B $(BUILD_DIR) -S . -G "Ninja" -DCMAKE_BUILD_TYPE=Debug

build:
	@cmake --build $(BUILD_DIR) --config Release

run:
	./$(BUILD_DIR)/$(PROJECT_NAME)/$(PROJECT_NAME).exe