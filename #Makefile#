# main is a phony target
.PHONY: main

# Define default compiler flags
CFLAGS = -g -Wall -std=c99 -D_DEFAULT_SOURCE -Wno-missing-braces -Werror=pointer-arith -fno-strict-aliasing

# Define library flags
LDLIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11


main:
# Delete the bin folder if it exists
	rm -rf bin || true
# Create the bin folder
	mkdir bin
# Create executable using all .c files in src
	gcc -o bin/arkavoid src/*.c $(CFLAGS) $(LDLIBS)
# Copy levels folder into bin
	cp -r levels bin/levels
