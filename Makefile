# Common C compiler and linker flags
COMMON_C_FLAGS = -c -m64 -g -Wall  -pedantic -std=c99
COMMON_LINKER_FLAGS = -g -m64


#uselibrates target
use-libso: uselibrates.o librates.so
	gcc $(COOMON_LINKER_FLAGS) -o uselibrates uselibrates.c ./librates.so

# build the shared object (dynamic link library)
librates.so : http_pic.o rates_pic.o
	gcc -shared $(COMMON_LINKER_FLAGS) -o ./librates.so http_pic.o rates_pic.o -ljansson -lcurl

# compile rates.c with "position independent code" option in order to add to shared object
rates_pic.o: rates.c rates.h
	gcc $(COMMON_C_FLAGS) -fPIC -o rates_pic.o rates.c -ljansson -lcurl

# compile http.c with "position independent code" option in order to add to shared object
http_pic.o: http.c http.h
	gcc $(COMMON_C_FLAGS) -fPIC -o http_pic.o http.c -lcurl

clean:
	@rm	 -f rates_pic http_pic uselibrates *.o 2>/dev/null | true
	