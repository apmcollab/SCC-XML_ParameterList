#
# This makefile builds the XML_ParameterList support library. 
#
#
/*
#############################################################################
#
# Copyright  2024- Chris Anderson
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the Lesser GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# For a copy of the GNU General Public License see
# <http://www.gnu.org/licenses/>.
#
#############################################################################
*/
SHELL=/bin/sh

# Parameters for library construction script 

CXX     := g++
LIB_DIR = ./lib
SRC_DIR = .

# Create library directory if it doesn't exist 

create_lib_dir := \
$(shell if [ ! -e $(LIB_DIR) ]; then mkdir $(LIB_DIR); fi) 

# specify make parameters

ifndef VERBOSE
QUIET     :=@
MAKEFLAGS += --no-print-directory
MAKEFLAGS := $(sort $(MAKEFLAGS))
endif

ifeq ($(MAKECMDGOALS),release)

CFLAGS      :=-O2  -fno-gcse -fno-optimize-sibling-calls -Wno-write-strings
CXXFLAGS    :=-O2  -fno-gcse -fno-optimize-sibling-calls -Wno-write-strings
CDEFINES    += 
CXXDEFINES  += 
endif

ifeq ($(MAKECMDGOALS),debug)
CFLAGS      =-g -Wall -fno-inline
CDEFINES    =-D_DEBUG
CXXFLAGS    =-g -Wall -fno-inline
CXXDEFINES  +=-D_DEBUG 
endif

RELEASE_DIR  = ./_releaseXML_ParameterList
DEBUG_DIR    = ./_debugXML_ParameterList

DEBUG_LIBRARY    = $(LIB_DIR)/libXML_ParameterListd.a
RELEASE_LIBRARY  = $(LIB_DIR)/libXML_ParameterList.a 

include ./CommonLibMake.mk
