.PHONY: flash push

all: push

flash:
	@qmk flash -kb feb40 -km default

push:
	@git add -A
	@git commit -m adjustments
	@git push