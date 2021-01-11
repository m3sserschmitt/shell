OBJECTS += ./kill.o 

CC_DEPS += ./deps/kill.d 

./%.o: ./%.c
	@echo 'Building file: $<'
	$(CC) -c -Wall $< -o $@
	@echo 'Build finished: $<'
	@echo

