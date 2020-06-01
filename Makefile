  prog = Trees
   obj = $(prog).o
   gdb = -g
 flags = $(gdb) -std=c++1z
    cc = g++
   ext = cpp

$(prog): $(obj)
	$(cc) $(gdb) -o $@ $^

 $(obj): $(prog).$(ext)
	$(cc) $(flags) -o $@ -c $<

 .PHONY: clean debug

  clean:
	$(RM) $(prog) $(obj)

  debug: flags += -DDEBUG
  debug: $(prog)
