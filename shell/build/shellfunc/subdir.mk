OBJECTS += ./shellfunc/cd.o \
./shellfunc/exec.o \
./shellfunc/history.o \
./shellfunc/shell.o \
./shellfunc/shellfunc.o \
./shellfunc/shutil.o \
./shellfunc/shutil2.o 

CC_DEPS += ./shellfunc/deps/cd.d \
./shellfunc/deps/exec.d \
./shellfunc/deps/history.d \
./shellfunc/deps/shell.d \
./shellfunc/deps/shellfunc.d \
./shellfunc/deps/shutil.d \
./shellfunc/deps/shutil2.d 

./shellfunc/%.o: ../shellfunc/%.c
	@echo 'Building file: $<'
	$(CC) -c -Wall -g -O0 -I../../libs/include -I../luafunc -I../shellfunc $< -o $@
	@echo 'Build finished: $<'
	@echo

