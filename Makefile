#!/bin/make
#
# makefile for latex projects based on latexmk.
#
# Author: Luigi Pertoldi
# Created: 19/04/2017
#
# Notes: \input does not work with latexmk
#
PROJ = main
LC   = latexmk
COPT = -silent -time -pdflua -outdir=log -M -MP -MF log/.deps
DIRS = log log/src

all : $(PROJ).pdf abstract

$(PROJ).pdf : $(PROJ).tex $(DIRS) FORCE_MAKE
	cd img && make all && cd ..
	$(LC) $(COPT) $<
	cp log/main.pdf ./main.pdf
#if [ ! -h $@ ]; then ln -s log/$@ ./$@; fi

abstract : abstract/abstract.tex FORCE_MAKE
	cd abstract && make all && cd ..

preview : $(DIRS)
	$(LC) $(COPT) -pvc $(PROJ).tex

$(DIRS) :
	mkdir -p $(DIRS)

clean :
	$(LC) $(COPT) -C

.PHONY : FORCE_MAKE preview clean
-include log/.deps
