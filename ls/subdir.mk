OBJECTS += ./ls.o 

CC_DEPS += ./deps/ls.d 

./%.o: ./%.c
	@echo 'Building file: $<'
	$(CC) -c -Wall $< -o $@
	@echo 'Build finished: $<'
	@echo

