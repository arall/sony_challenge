mod_ping.la: mod_ping.slo
	$(SH_LINK) -rpath $(libexecdir) -module -avoid-version  mod_ping.lo
DISTCLEAN_TARGETS = modules.mk
shared =  mod_ping.la
