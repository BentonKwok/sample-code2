APPS = assignment3

#Change to your own student number
STUDENT_NUMBER = 301171198

OBJECTS = lexemesTypes.o parserClasses.o assignment3.o
HEADERS = lexemesTypes.h parserClasses.h

#Debug flag
CXXFLAGS += -g

all: $(APPS)

$(OBJECTS): $(HEADERS)
$(APPS): $(OBJECTS)
	$(CXX) $(LDFLAGS) -o $@ $^

clean:
	$(RM) *.o $(APPS)

tar:
	tar -czf $(STUDENT_NUMBER).tar.gz Makefile *.cpp *.h


