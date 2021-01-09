OBJECTS += ./luafunc/cd.o \
./luafunc/exec.o 

CC_DEPS += ./luafunc/deps/cd.d \
./luafunc/deps/exec.d 

./luafunc/%.o: ../luafunc/%.c
	@echo 'Building file: $<'
	$(CC) -c -Wall -g -O0 -I../../libs/include -I../luafunc -I../shellfunc $< -o $@
	@echo 'Build finished: $<'
	@echo

