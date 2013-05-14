# $Id: Makefile 478 2011-04-19 08:06:54Z bradatch $

###############################################################################
# Configuration: You may change the following variables
###############################################################################

BOARD	= de2-70
#BOARD	= db_4ce10

MAPFILE = map.txt

ADD_SIZE = ./addsize

###############################################################################
# DO NOT CHANGE ANYTHING BELOW (unless you know what you're doing ;) )
###############################################################################

###############################################################################
# Binaries
###############################################################################

AR   = or32-elf-ar
AS   = or32-elf-as
CC   = or32-elf-gcc
LD   = or32-elf-ld
OBJC = or32-elf-objcopy
OBJD = or32-elf-objdump


###############################################################################
# Variables for flags
###############################################################################

#MAP      = -Wl,-Map,$(MAPFILE) # as paramter to gcc
MAP	 = -Map $(MAPFILE)
LDSCRIPT = ld/altera-$(BOARD).ld
L_LIB	 = or32rte-$(BOARD)


###############################################################################
# Flags
###############################################################################

ASFLAGS  = -g
CPPFLAGS = -Iinclude -D__BOARD=$(BOARD)
CFLAGS   = -g -Wall -O2
CXXFALGS = $(CFLAGS)
LDFLAGS  = -Ttext 0x0 -Bstatic -T$(LDSCRIPT) $(MAP) -Llib -l$(L_LIB) 


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
CRT_SRC = crt/boot.S
CRT_OBJ = $(CRT_SRC:%.S=%.o)
CRT = lib/boot.o

# driver lib
LIB = lib/lib$(L_LIB).a

# test program
TARGET = main
TARGETSRC = $(TARGET).c
TARGETOBJ = $(TARGETSRC:%.c=%.o)


###############################################################################
# Rules
###############################################################################

#
# NOTE: We don't link against libc! Maybe someone should try to do this once,
#	but for teaching OSes it's not that important
#


.PHONY: all
all: $(CRT) $(LIB) $(TARGET).elf $(TARGET).bin


$(CRT): $(CRT_OBJ)
	cp $^ $@


$(LIB): $(OBJ)
	$(AR) -rc $@ $^


$(TARGET).elf: $(TARGETOBJ) $(CRT) $(LIB)
	$(LD) $(CRT) $(TARGETOBJ) $(LDFLAGS) -o $@


ifeq ($(BOARD),db_4ce10)
$(TARGET).bin: $(TARGET).elf
	$(OBJC) -O binary $< $(TARGET).bin
	@$(ADD_SIZE) $(TARGET).bin
else
$(TARGET).bin: $(TARGET).elf
	$(OBJC) -O binary $< $(TARGET).bin
endif
	

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
