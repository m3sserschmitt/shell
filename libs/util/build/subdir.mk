OBJECTS += ./data.o \
./mem.o \
./path.o \
./str.o \
./user.o 

CC_DEPS += ./deps/data.d \
./deps/mem.d \
./deps/path.d \
./deps/str.d \
./deps/user.d 

./%.o: ../%.c
	@echo 'Building file: $<'
	$(CC) -c -Wall -fPIC -g -O0 $< -o $@
	@echo 'Build finished: $<'
	@echo

