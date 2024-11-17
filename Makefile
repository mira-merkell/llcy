# --------------------------------------------------------------------------- #
# Copyright 2024 Marek Miller                                                 #
#                                                                             #
# This program is free software: you can redistribute it and/or modify it     #
# under the terms of the GNU General Public License as published by the       #
# Free Software Foundation, either version 3 of the License, or (at your      #
# option) any later version.                                                  #
#                                                                             #
# This program is distributed in the hope that it will be useful, but         #
# WITHOUT ANY WARRANTY# without even the implied warranty of MERCHANTABILITY  #
# or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License    #
# for more details.                                                           #
#                                                                             #
# You should have received a copy of the GNU General Public License along     #
# with this program.  If not, see <https://www.gnu.org/licenses/>.            #
# --------------------------------------------------------------------------- #
AS		:= nasm
ASFLAGS		+= -felf64 -w+all -w-reloc-rel-dword -Ox
CC		?= gcc
CFLAGS		+= -std=c23 -Wall -Wextra -O2 -march=native
LDFLAGS		+=
LDLIBS		+=
INCLUDE		:= ./include

RM		:= rm -fv
MKDIR		:= mkdir -p

# Source code dependencies
BENCHES	:=


LIBDIR		:= lib
LIBOBJS		:= $(LIBDIR)/xoshiro256ss.o

$(LIBDIR)/xoshiro256ss.o: $(INCLUDE)/xoshiro256ss.h


LLCYDIR		:= llcy
LLCYLIBS	:= $(LLCYDIR)/llcy.a 					\
			$(LLCYDIR)/llcy.so
LLCYOBJS	:= $(LLCYDIR)/llcy.o					\
			$(LLCYDIR)/slot.o

$(LLCYDIR)/slot.o: $(INCLUDE)/$(LLCYDIR)/slot.h
$(LLCYOBJS):	$(INCLUDE)/llcy.h
$(LLCYLIBS):	$(LLCYOBJS)

$(LLCYDIR)/llcy.a: 
	ar rcs $@ $^

$(LLCYDIR)/llcy.so:
	$(CC) -shared -o $@ $^

TESTDIR		:= ./test
TESTS		:= $(TESTDIR)/t-llcy

$(TESTS):	$(LLCYDIR)/llcy.a					\
			$(LIBOBJS)

# Update flags
CFLAGS		+= -I$(INCLUDE)

.PHONY:	all
.DEFAULT_GOAL := all
all: build

.PHONY: build build-llcy build-bench build-test
build-llcy: $(LLCYLIBS)
build-bench: $(BENCHES)
build-test: $(TESTS)
build: build-llcy build-bench build-test 

.PHONY: debug
debug: $(BENCHES) $(LLCYLIBS) $(PROGS) $(TESTS)
debug: ASFLAGS	+= -DDEBUG -Og -Fdwarf
debug: CFLAGS	+= -DDEBUG -g -Og

.PHONY: check
check: build-test
	@for tt in $(TESTS); do						\
		./$$tt && echo "$$tt: OK" ||				\
			( echo "$$tt: FAIL" ; exit 1)			\
	done

.PHONY: clean
clean:
	$(RM) $(BENCHOBJS) $(BENCHES)
	$(RM) $(LLCYOBJS) $(LLCYLIBS)
	$(RM) $(TESTOBJS) $(TESTS)

.PHONY: format
format:
	@find ./ -name "*.c" 						\
		-or -name "*.h"						\
		-or -name "*.cpp"  |					\
		while read f ; do					\
			clang-format --style=file -i $$f ;		\
		done

