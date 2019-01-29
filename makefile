
all:
	make release

release:
	mkdir -p release
	cd src && make illustrationlogic
	cp src/illustrationlogic release

.PHONY: release

