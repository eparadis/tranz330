sdcc -mz80 \
	--data-loc 0x8000 \
	--no-std-crt0 mycrt0/mycrt0.rel \
	$1

