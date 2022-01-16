CFLAGS=-Wall -O2
TOOLS=mec_csum_flasher mec_csum_outer

all: $(TOOLS)

%: %.c mmapfile.h
	$(CC) $(CFLAGS) $< -o $@ $(LD_LIBS_$@)

clean:
	rm -f $(TOOLS)

