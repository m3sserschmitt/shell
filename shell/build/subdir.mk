OBJECTS += ./exec.o \
./shell.o \
./util.o 

CC_DEPS += ./deps/exec.d \
./deps/shell.d \
./deps/util.d 

./%.o: ../%.c
	@echo 'Building file: $<'
	$(CC) -c -Wall $< -o $@
	@echo 'Build finished: $<'
	@echo

