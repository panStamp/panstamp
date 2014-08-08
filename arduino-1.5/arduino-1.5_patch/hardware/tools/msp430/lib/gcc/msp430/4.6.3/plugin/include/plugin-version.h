#include "configargs.h"

static char basever[] = "4.6.3";
static char datestamp[] = "20120301";
static char devphase[] = "mspgcc LTS 20120406 unpatched";
static char revision[] = "";

/* FIXME plugins: We should make the version information more precise.
   One way to do is to add a checksum. */

static struct plugin_gcc_version gcc_version = {basever, datestamp,
						devphase, revision,
						configuration_arguments};
