#*
#*  RingEXtract - RingEX C Interface for Data Extraction
#*  Copyright (C) 2024 Ian Wang
#*  
#*  This program is free software: you can redistribute it and/or modify
#*  it under the terms of the GNU General Public License as published by
#*  the Free Software Foundation, either version 3 of the License, or
#*  (at your option) any later version.
#*  
#*  This program is distributed in the hope that it will be useful,
#*  but WITHOUT ANY WARRANTY; without even the implied warranty of
#*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#*  GNU General Public License for more details.
#*  
#*  You should have received a copy of the GNU General Public License
#*  along with this program.  If not, see <https://www.gnu.org/licenses/>.
#*

src = $(wildcard *.c)
obj = $(src:.c=.o)
lib = libringextract.a

CC = gcc
CFLAGS = -std=c17 -O2 -Wall -Wextra -I$(vend)/include
LDFLAGS = -L$(vend)/lib -lcurl
ARFLAGS = rcs

$(lib): $(obj)
	$(AR) $(ARFLAGS) $@ $^

.PHONY: clean
clean:
	rm $(lib) $(obj)
