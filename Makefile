# Macros
CC = gcc
CFLAGS = -std=c99 --pedantic -Wall -Wextra -Wmissing-prototypes
#WARNINGS = -Wall -Wextra -pedantic -Wshadow -Wpointer-arith -Wcast-align \
            #-Wwrite-strings -Wmissing-prototypes -Wmissing-declarations \
            #-Wredundant-decls -Wnested-externs -Winline \
            #-Wuninitialized -Wconversion -Wstrict-prototypes
LDFLAGS = -lm
SRCDIR   = ./src/
BUILDDIR   = ./build/
BINDIR   = ./bin/
DATADIR = ./data/
TARGET = NNSketch
TTARGET = test
ALL = $(TARGET) $(TTARGET) 
# Object files

$(BUILDDIR)%.o: $(SRCDIR)%.c
	@mkdir -p $(BUILDDIR) $(BINDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

# Executables
all: $(ALL)

$(TARGET): $(BUILDDIR)main.o $(BUILDDIR)PointLine.o $(BUILDDIR)Sketch.o $(BUILDDIR)Dataset.o $(BUILDDIR)Recognizer.o $(BUILDDIR)easyppm.o
$(TTARGET): $(BUILDDIR)PointLine.o $(BUILDDIR)Sketch.o $(BUILDDIR)Dataset.o $(BUILDDIR)Recognizer.o $(BUILDDIR)easyppm.o $(BUILDDIR)test.o

$(ALL):
	$(CC) -o $(BINDIR)$@ $^ $(LDFLAGS) 

# Phony
.PHONY: all clean run $(TARGET) $(TTARGET)

clean:
	rm -f $(BUILDDIR)*.o $(BINDIR)$(TTARGET) $(BINDIR)$(TARGET)

run: $(TARGET)
	$(BINDIR)$(TARGET) $(DATADIR)trainingset.txt $(DATADIR)testset.txt

runtest: $(TTARGET)
	$(BINDIR)$(TTARGET) $(DATADIR)trainingset.txt $(DATADIR)testset.txt

$(BUILDDIR)main.o: $(SRCDIR)PointLine.h $(SRCDIR)Sketch.h $(SRCDIR)Dataset.h $(SRCDIR)Recognizer.h $(SRCDIR)easyppm.h
$(BUILDDIR)Dataset.o: $(SRCDIR)Sketch.h $(SRCDIR)PointLine.h
$(BUILDDIR)PointLine.o: $(SRCDIR)PointLine.h
$(BUILDDIR)Recognizer.o: $(SRCDIR)Dataset.h $(SRCDIR)Sketch.h $(SRCDIR)PointLine.h $(SRCDIR)Recognizer.h
$(BUILDDIR)Sketch.o: $(SRCDIR)PointLine.h $(SRCDIR)Sketch.h
$(BUILDDIR)easyppm.o: $(SRCDIR)easyppm.h

