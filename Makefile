forever-splice: forever-splice.c
	$(CC) -o $@ $<

.PHONY: clean

clean:
	rm -f forever-splice
