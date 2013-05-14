# $Id: Makefile 676 2013-04-12 13:57:21Z klugeflo $

###############################################################################
# Configuration: You may change the following variables
###############################################################################

#BOARD	= de2-70
BOARD	= db_4ce10

#ADD_SIZE = ./addsize

###############################################################################
# DO NOT CHANGE ANYTHING BELOW (unless you know what you're doing ;) )
###############################################################################

###############################################################################
# Binaries
###############################################################################

AR   = nios2-elf-ar
AS   = nios2-elf-as
CC   = nios2-elf-gcc
LD   = nios2-elf-g++
OBJC = nios2-elf-objcopy
OBJD = nios2-elf-objdump

N2DOWNLOAD = bin/n2download.sh

###############################################################################
# Files
###############################################################################

# drivers
SRC = $(wildcard src/*.c)
PPASM = $(wildcard src/*.S)
ASM = $(PPASM:%.S=%.s) $(wildcard src/*.s)
OBJ = $(ASM:%.s=%.o) $(SRC:%.c=%.o)
DEP = $(ASM:%.s=%.d) $(SRC:%.c=%.d)

# bootcode
CRT_SRC = crt/crt0.S
CRT_OBJ = $(CRT_SRC:%.S=%.o)
CRT = lib/crt0.o

# driver lib
LIB = lib/lib$(L_LIB).a

# test program
TARGET = main
TARGETSRC = $(TARGET).c
TARGETOBJ = $(TARGETSRC:%.c=%.o)
MAPFILE = $(TARGET).map


###############################################################################
# Variables for flags
###############################################################################

#MAP      = -Wl,-Map,$(MAPFILE) # as paramter to gcc
#MAP	 = -Map $(MAPFILE)
LDSCRIPT = ld/altera-nios2-$(BOARD).ld
L_LIB	 = nios2rte-$(BOARD)


# general flags
DEBUG_SYMBOLS = -g

# asflags

# cppflags
CPP_INCLUDE = -Iinclude
CPP_BOARD   = -D__BOARD=$(BOARD)
CPP_DEF =-DSYSTEM_BUS_WIDTH=32 -D__hal__ -DALT_NO_INSTRUCTION_EMULATION -DALT_SINGLE_THREADED

# cflags
C_OPTIMIZE = #-O2

# ldflags
LD_CRT = $(CRT)

###############################################################################
# Flags built from the above variables
###############################################################################

# now build the flags
ASFLAGS  = $(DEBUG_SYMBOLS)
CPPFLAGS = $(CPP_INCLUDE) $(CPP_DEF) $(CPP_BOARD)

CFLAGS   = -Wall $(DEBUG_SYMBOLS) $(OPTIMIZE) -pipe -EL -mno-hw-div -mno-hw-mul -mno-hw-mulx
CXXFALGS = $(CFLAGS)
LDFLAGS  = -T'$(LDSCRIPT)' -msys-crt0='$(LD_CRT)' -Wl,-Map,$(MAPFILE) -Llib -l$(L_LIB)
#LDFLAGS  = -Ttext 0x0 -Bstatic -T$(LDSCRIPT) $(MAP) -Llib -l$(L_LIB)


###############################################################################
# Rules
###############################################################################

#
# NOTE: We don't link against libc! Maybe someone should try to do this once,
#	but for teaching OSes it's not that important
#
# FAK/13-04-12: don't know whether that's still true for the NIOS2 platform


###############################################################################
# Dummy rules for convenience

.PHONY: all
all: crt lib $(TARGET)
#all: $(CRT) $(LIB) $(TARGET).elf $(TARGET).bin


.PHONY: crt
crt: $(CRT)

.PHONY: lib
lib: $(LIB)

.PHONY: $(TARGET)
$(TARGET): $(TARGET).elf

###############################################################################
# here come the actual rules

$(CRT): $(CRT_OBJ)
	cp $^ $@


$(LIB): $(OBJ)
	$(AR) -rc $@ $^


$(TARGET).elf: $(TARGETOBJ) $(CRT) $(LIB)
#	$(LD) $(CRT) $(TARGETOBJ) $(LDFLAGS) -o $@
	$(LD) $(TARGETOBJ) $(LDFLAGS) -o $@


$(TARGET).bin: $(TARGET).elf
	$(OBJC) -O binary $< $(TARGET).bin
#	@$(ADD_SIZE) $(TARGET).bin


# auto generate dependencies (see make manual)
%.d: %.c
	@set -e; rm -f $@; \
	$(CC) -MM $(CPPFLAGS) $(CFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$


-include $(DEP)


.PHONY: clean
clean:
	$(RM) $(OBJ) $(DEP)
	$(RM) $(CRT_OBJ)
	$(RM) $(CRT) $(LIB)
	$(RM) $(TARGET).elf $(TARGET).bin $(TARGETOBJ)
	$(RM) $(MAPFILE)
	$(RM) transfer.log
	$(RM) src/*.d.*


.PHONY: new
new: clean all



# Running the file

.PHONY: upload
upload: $(TARGET).elf
	$(N2DOWNLOAD) $(TARGET).elf

.PHONY: run
run: upload
	$(N2DOWNLOAD) -g

.PHONY: cont
cont:
	$(N2DOWNLOAD) -g

.PHONY: stop
stop:
	$(N2DOWNLOAD) --stop
