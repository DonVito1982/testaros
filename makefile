CFLAGS = -Wextra -Wall -Wpedantic -g -std=c11

.PHONY: test
test: bin/test
	@bin/test

build/testaros.o: src/testaros.c src/testaros.h
	clang -o $@ -c src/testaros.c $(CFLAGS)

build/selftest.o: test/main.c
	clang -o $@ -c test/main.c $(CFLAGS)

bin/test: build/testaros.o build/selftest.o
	clang -o $@ $^

.PHONY: clean
clean:
	rm bin/*
	rm build/*
