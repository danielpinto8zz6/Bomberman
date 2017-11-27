SUBDIRS = src/server src/client

all: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

.PHONY: $(SUBDIRS)

clean: 
	rm build/bombermanClient build/bombermanServer src/server/*.o src/client/*.o
