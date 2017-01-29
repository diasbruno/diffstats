CX=clang
CXX=$(CX)++

%.o: %.c
	$(CX) $(CFLAGS) $(INCLUDES) -c -o $@ $<
