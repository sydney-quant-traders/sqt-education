.PHONY: all clean cleanall watch init

MAIN = main
BUILD_DIR = build

all: init
	latexmk $(MAIN).tex

watch: init
	latexmk -pvc $(MAIN).tex

clean:
	latexmk -c

cleanall:
	latexmk -C
	rm -rf $(BUILD_DIR)

init:
	@mkdir -p $(BUILD_DIR)