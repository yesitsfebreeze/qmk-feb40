MAKEFLAGS += --no-print-directory
.PHONY: all push copy build flash

kb=feb40

push: #m="my message"
	$(eval m ?= $(if $(m),$(m),"update"))
	@git add --all
	@git commit -m "$(m)"
	@git push

copy:
	@echo Copying firmware to ./build
	@if [ -e "../../${kb}_default.bin" ]; then cp "../../${kb}_default.bin" "./build/${kb}_default.bin"; fi
	@if [ -e "../../${kb}_default.hex" ]; then cp "../../${kb}_default.hex" "./build/${kb}_default.hex"; fi

build:
	@qmk compile -kb $(kb) -km default
	@make copy -B

flash: # console=true/false
	@qmk flash -kb $(kb) -km default
	@make copy -B
ifeq ($(console),true)
	@qmk console
endif
