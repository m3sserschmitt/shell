OBJECTS += ./touch.o 

CC_DEPS += ./deps/touch.d 

./%.o: ./%.c
	@echo 'Building file: $<'
	$(CC) -c -Wall $< -o $@
	@echo 'Build finished: $<'
	@echo

