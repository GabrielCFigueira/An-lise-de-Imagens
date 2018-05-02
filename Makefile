# Main Makefile
# ASA 2018
# Rafael Andrade, Gabriel Figueira
# P2

SHELL         := /bin/sh
RELEASE_NAME  := asa.out


OBJ_FILES   := build
SRC_FILES   := src
TST_FILES   := tests
TST_COMM    := ./run_tests.sh
ANA_COMM    := ./tests/analysis.sh

ZIP         := zip
ZIP_FLAGS   := -ur --quiet

CLEAN_COM   := cleaner

REPORT_NAME  := docs/relatorio-al040
BIBLIOGRAPHY := docs/references.bibtex
.DEFAULT_GOAL := .DEFAULT

.DEFAULT:
	$(MAKE) $(MAKECMDGOALS) -C $(OBJ_FILES)

.PHONY : zip test do cleantest report analysis
zip :
	$(MAKE) $(CLEAN_COM)
	$(ZIP) $(ZIP_FLAGS) $(RELEASE_NAME).zip $(MAKEFILE_LIST) ||:
	$(ZIP) $(ZIP_FLAGS) $(RELEASE_NAME).zip $(SRC_FILES) ||:


test :
	cd $(TST_FILES) && $(TST_COMM)

cleantest :
	cd $(TST_FILES) && `find . -regex '.*\.gen' -delete`

do :
	$(MAKE) -C $(OBJ_FILES)

report :
	pandoc --bibliography=$(BIBLIOGRAPHY) -f markdown -t latex $(REPORT_NAME).md -o $(REPORT_NAME).pdf

analysis :
	$(ANA_COMM)
