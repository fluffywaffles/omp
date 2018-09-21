# NOTE(jordan): shell variables must be escaped
.PHONY: clean bin-exists

bin-exists = $(test !(-d bin); echo $$?)

.bin-dir:
	@mkdir -p bin

hello-world: hello.c .bin-dir
	clang -fopenmp -o bin/$@ hello.c

clean:
	rm -rf bin
