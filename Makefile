CC           = gcc
LD           = gcc
AR           = ar
ARFLAGS      = rcs
CFLAGS       = -Os -c -fopenmp
LDFLAGS      = -Os -fopenmp -Wl,-Map,test.map

OBJCOPYFLAGS = -j .text -O ihex
OBJCOPY      = objcopy

# include path to AVR library
INCLUDE_PATH = /usr/lib/avr/include
# splint static check
SPLINT       = splint test.c ecb.c cbc.c ctr.c cfb.c aes.c -I$(INCLUDE_PATH) +charindex -unrecog

default: test.elf ecb.elf cbc.elf ctr.elf cfb.elf

.SILENT:
.PHONY:  lint clean

test.hex : test.elf
	echo copy object-code to new image and format in hex
	$(OBJCOPY) ${OBJCOPYFLAGS} $< $@

test.o : test.c aes.h aes.o
	echo [CC] $@ $(CFLAGS)
	$(CC) $(CFLAGS) -o  $@ $<

ecb.hex : ecb.elf
	echo copy object-code to new image and format in hex
	$(OBJCOPY) ${OBJCOPYFLAGS} $< $@

ecb.o : ecb.c aes.h aes.o
	echo [CC] $@ $(CFLAGS)
	$(CC) $(CFLAGS) -o  $@ $<

cbc.hex : cbc.elf
	echo copy object-code to new image and format in hex
	$(OBJCOPY) ${OBJCOPYFLAGS} $< $@

cbc.o : cbc.c aes.h aes.o
	echo [CC] $@ $(CFLAGS)
	$(CC) $(CFLAGS) -o  $@ $<

ctr.hex : ctr.elf
	echo copy object-code to new image and format in hex
	$(OBJCOPY) ${OBJCOPYFLAGS} $< $@

ctr.o : ctr.c aes.h aes.o
	echo [CC] $@ $(CFLAGS)
	$(CC) $(CFLAGS) -o  $@ $<

cfb.hex : cfb.elf
	echo copy object-code to new image and format in hex
	$(OBJCOPY) ${OBJCOPYFLAGS} $< $@

cfb.o : cfb.c aes.h aes.o
	echo [CC] $@ $(CFLAGS)
	$(CC) $(CFLAGS) -o  $@ $<

aes.o : aes.c aes.h
	echo [CC] $@ $(CFLAGS)
	$(CC) $(CFLAGS) -o $@ $<

test.elf : aes.o test.o
	echo [LD] $@
	$(LD) $(LDFLAGS) -o $@ $^

ecb.elf : aes.o ecb.o
	echo [LD] $@
	$(LD) $(LDFLAGS) -o $@ $^

cbc.elf : aes.o cbc.o
	echo [LD] $@
	$(LD) $(LDFLAGS) -o $@ $^

ctr.elf : aes.o ctr.o
	echo [LD] $@
	$(LD) $(LDFLAGS) -o $@ $^

cfb.elf : aes.o cfb.o
	echo [LD] $@
	$(LD) $(LDFLAGS) -o $@ $^

aes.a : aes.o
	echo [AR] $@
	$(AR) $(ARFLAGS) $@ $^

lib : aes.a

clean:
	rm -f *.OBJ *.LST *.o *.gch *.out *.hex *.map *.elf *.a

test:
	make clean && make && ./test.elf

lint:
	$(call SPLINT)
