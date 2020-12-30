OBJECTS += ./cd.o \
./exec.o \
./main.o \
./shell.o \
./shutil.o 

CC_DEPS += ./deps/cd.d \
./deps/exec.d \
./deps/main.d \
./deps/shell.d \
./deps/shutil.d 

./%.o: ../%.c
	@echo 'Building file: $<'
	$(CC) -c -Wall -g -O0 -I../../libs/include $< -o $@
	@echo 'Build finished: $<'
	@echo

