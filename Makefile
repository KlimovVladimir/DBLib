TOPTARGETS := all clean install

TOPDIR = $(pwd)

SUBDIRS := lib tools

$(TOPTARGETS): $(SUBDIRS)
$(SUBDIRS):
	$(MAKE) -C $@ $(MAKECMDGOALS)

.PHONY: $(TOPTARGETS) $(SUBDIRS)