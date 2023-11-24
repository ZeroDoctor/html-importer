
.PHONY: build
build:
	rm ./build/zdhtml || true
	rm ./build/compile_commands.json || true
	rm ./compile_commands.json || true
	cmake . -D CMAKE_BUILD_TYPE=Debug -D CMAKE_EXPORT_COMPILE_COMMANDS=ON -S . -B build
	cmake --build build
	ln -s ./build/compile_commands.json .

.PHONY: run
run:
	./build/zdhtml ./example
