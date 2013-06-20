#
# OSOCCI Makefile.am
#
# Copyright 2012, Prologue, All rights reserved
#
# Authors:
#   Iain James Marshall <ijm667@hotmail.com>
#
bin_PROGRAMS = swiftprocci
EXTRA_DIST = procciswift.c #swiftprocci_contract.c swiftprocci_contract.h

swiftprocci_SOURCES = swiftprocci.c swiftprocci.h 

swiftprocci_CFLAGS = $(AM_CFLAGS) $(CFLAGS_COREST) $(CFLAGS_BROKER) $(CFLAGS_OCCI) $(CFLAGS_COXML) $(CFLAGS_CORDS) $(CFLAGS_COCCI) $(CFLAGS_COCARRIER) $(CFLAGS_PUBOCCI) $(CFLAGS_PROCCI) $(CFLAGS_COSACS) $(CFLAGS_COPABR) $(shell python-config --cflags)
swiftprocci_LDFLAGS = $(LD_COCARRIER) $(LD_COPABR) $(LD_PUBOCCI) $(LD_COCCI) $(LD_OCCI) $(LD_CORDS) $(LD_COREST) $(LD_COXML) $(LDFLAGS_UUID) $(LDFLAGS_SSL) $(LDFLAGS_THREADS) $(shell python-config --ldflags)

# Create auto-generated model files
model_h_files =  swift.h #swiftconfig.h #blob.h bucket.h
include $(top_srcdir)/tools/Makefile_generate_model.am
swiftprocci.c: $(model_c_files)
