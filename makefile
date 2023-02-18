LIB_INCLUDE_DIR = ./include/sharc_ap/

CPP = g++
STD = c++14
CFLAGS = -Wall -Wextra -Wpedantic

BUILD_DIR = ./build

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

./build/tb: ./test/tb.cpp $(BUILD_DIR)
	$(CPP) -std=$(STD) $(CFLAGS) -I$(LIB_INCLUDE_DIR) $< -o $@

test: ./build/tb
	./build/tb

clean:
	rm -rf $(BUILD_DIR)

.PHONY: clean