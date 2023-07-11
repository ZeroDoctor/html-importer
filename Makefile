
build:
	cmake . -D CMAKE_BUILD_TYPE=Debug -D CMAKE_EXPORT_COMPILE_COMMANDS=ON -S . -B build
	ln -s ./build/compile_commands.json .

