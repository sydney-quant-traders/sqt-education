.PHONY: all clean cleanall watch init force

MAIN = main
BUILD_DIR = build

all: init
	latexmk -g $(MAIN).tex

watch: init
	latexmk -pvc $(MAIN).tex

# Force full rebuild from scratch
force: init
	latexmk -gg $(MAIN).tex

clean:
	latexmk -c

cleanall:
	latexmk -C
	rm -rf $(BUILD_DIR)

init:
	@mkdir -p $(BUILD_DIR)