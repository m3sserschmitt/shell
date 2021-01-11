OBJECTS += ./mv.o 

CC_DEPS += ./deps/mv.d 

./%.o: ./%.c
	@echo 'Building file: $<'
	$(CC) -c -Wall -g -O0 $< -o $@
	@echo 'Build finished: $<'
	@echo

