CC :=gcc
LD :=gcc
TARGET :=shell
OUT :=./shell
LIBS :=-llua -lm -ldl
CC_DEPS :=
OBJECTS :=
RM :=rm -v

all: $(OUT)

-include ./subdir.mk
-include $(CC_DEPS)

$(OUT): $(OBJECTS)
	@echo Building target: "$@".
	@echo Invoking $(LD) Linker ...
	$(LD) $(OBJECTS) $(LIBS) -g -O0 ../../libs/libshellutil.a -o $(OUT)
	@echo Target $(TARGET) build successfully.
	@echo Done.

clean:
	$(RM) $(OBJECTS) ./shell

