OBJECTS += ./main.o 

CC_DEPS += ./deps/main.d 

./%.o: ../%.c
	@echo 'Building file: $<'
	$(CC) -c -Wall -g -O0 -I../../libs/include -I../luafunc -I../shellfunc $< -o $@
	@echo 'Build finished: $<'
	@echo

