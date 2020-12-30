CC :=gcc
LD :=ld
TARGET :=libshellutil.so
OUT :=../../libshellutil.so
LIBS :=
CC_DEPS :=
OBJECTS :=
RM :=rm -v

all: $(OUT)

-include ./subdir.mk
-include $(CC_DEPS)

$(OUT): $(OBJECTS)
	@echo Building target: "$@".
	@echo Invoking $(LD) Linker ...
	$(LD) $(OBJECTS) $(LIBS) -shared -o $(OUT)
	@echo Target $(TARGET) build successfully.
	@echo Done.

clean:
	$(RM) $(OBJECTS) ../../libshellutil.so

static:
	ar -rvs ../../libshellutil.a *.o

