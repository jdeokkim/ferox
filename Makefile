#
# Copyright (c) 2021-2023 Jaedeok Kim <jdeokkim@protonmail.com>
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#

.PHONY: all clean

_COLOR_BEGIN != tput setaf 10
_COLOR_END != tput sgr0

PROJECT_NAME = ferox
PROJECT_FULL_NAME = c-krit/ferox

PROJECT_PREFIX = ${_COLOR_BEGIN}${PROJECT_FULL_NAME}:${_COLOR_END}

INCLUDE_PATH = include
LIBRARY_PATH = lib
SOURCE_PATH = src

OBJECTS = \
	${SOURCE_PATH}/broad-phase.o  \
	${SOURCE_PATH}/collision.o    \
	${SOURCE_PATH}/geometry.o     \
	${SOURCE_PATH}/rigid-body.o   \
	${SOURCE_PATH}/timer.o        \
	${SOURCE_PATH}/world.o

TARGETS = ${LIBRARY_PATH}/lib${PROJECT_NAME}.a

CC = gcc
AR = ar
CFLAGS = -D_DEFAULT_SOURCE -g -I${INCLUDE_PATH} -O2 -std=gnu99
LDLIBS = -lm

all: pre-build build post-build

pre-build:
	@echo "${PROJECT_PREFIX} Using: '${CC}' and '${AR}' to build this project."

build: ${TARGETS}

.c.o:
	@echo "${PROJECT_PREFIX} Compiling: $@ (from $<)"
	@${CC} -c $< -o $@ ${CFLAGS} ${LDFLAGS} ${LDLIBS}

${TARGETS}: ${OBJECTS}
	@mkdir -p ${LIBRARY_PATH}
	@echo "${PROJECT_PREFIX} Linking: ${TARGETS}"
	@${AR} rcs ${TARGETS} ${OBJECTS}

post-build:
	@echo "${PROJECT_PREFIX} Build complete."

clean:
	@echo "${PROJECT_PREFIX} Cleaning up."
	@rm -f ${LIBRARY_PATH}/*.a
	@rm -f ${SOURCE_PATH}/*.o
