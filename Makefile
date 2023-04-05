FLAGS = -Wall -Werror -Wextra -Iinclude

hw-01_bmp: obj obj/bmp.o obj/main.o obj/stego.o
	gcc -g -o hw-01_bmp obj/bmp.o obj/main.o obj/stego.o

obj/stego.o: src/stego.c include/stego.h
	gcc -o obj/stego.o -c $(FLAGS) src/stego.c

obj/bmp.o: src/bmp.c include/bmp.h
	gcc -o obj/bmp.o -c $(FLAGS) src/bmp.c

obj/main.o: src/main.c
	gcc -o obj/main.o -c $(FLAGS) src/main.c

obj:
	mkdir -p obj

clean:
	rm -rf obj hw-01_bmp

