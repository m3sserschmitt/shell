OBJECTS += ./echo.o 

CC_DEPS += ./deps/echo.d 

./%.o: ./%.c
	@echo 'Building file: $<'
	$(CC) -c -Wall $< -o $@
	@echo 'Build finished: $<'
	@echo

