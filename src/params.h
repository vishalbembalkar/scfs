#ifndef _PARAMS_H_
#define _PARAMS_H_

// The FUSE API has been changed a number of times.  So, our code
// needs to define the version of the API that we assume.  As of this
// writing, the most current API version is 26
#define FUSE_USE_VERSION 26

// need this to get pwrite().  I have to use setvbuf() instead of
// setlinebuf() later in consequence.
#define _XOPEN_SOURCE 500

#include <limits.h>
#include <stdio.h>
struct scfs_state {
    FILE *logfile;
    char *rootdir;
};
#define SCFS_DATA ((struct scfs_state *) fuse_get_context()->private_data)

#endif
