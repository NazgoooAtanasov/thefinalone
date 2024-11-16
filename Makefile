INCLUDES=-Iinclude/
LIBS=$(INCLUDES)
CFLAGS=-Wall -Wextra -Werror -g -DCOMPILER_DEBUG
thefinalone: src/thefinalone.c
	mkdir -p build
	gcc $(CFLAGS) $(LIBS) -o build/thefinalone src/thefinalone.c src/core/cmd.c src/core/compiler.c src/core/toker.c src/core/syntax.c src/core/ast.c src/core/semantic.c src/core/target.c
