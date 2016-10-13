#include "params.h"
#include <fuse.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>

#include "log.h"

FILE *log_open()
{
    FILE *logfile;
    
    // We cant directly print what ever goes on underneath the hood
    //so we must create something to show what goes on. THIS log, basically, 
    //shows all the calls made and the necessary stuff
    logfile = fopen("scfs.log", "w");
    if (logfile == NULL) {
	perror("logfile");
	exit(EXIT_FAILURE);
    }
    
    // set logfile to line buffering
    setvbuf(logfile, NULL, _IOLBF, 0);

    return logfile;
}
// dotdotdot will create a default log if one doesn't exist. 
void log_msg(const char *format, ...)
{
    va_list ap;
    va_start(ap, format);

    vfprintf(SCFS_DATA->logfile, format, ap);
}

// fuse context, idhar sab kuch save hota. 
void log_fuse_context(struct fuse_context *context)
{
	log_msg("    context:\n");
    	log_struct(context, fuse, %08x, );
	log_struct(context, uid, %d, );


    log_struct(context, gid, %d, );

 
    log_struct(context, pid, %d, );


    log_struct(context, private_data, %08x, );
    log_struct(((struct scfs_state *)context->private_data), logfile, %08x, );
    log_struct(((struct scfs_state *)context->private_data), rootdir, %s, );
	
   
    log_struct(context, umask, %05o, );
}

//This is for future use, we are not using any of these in scfs for NOW! 
//it has all the information about the socket , connection,etc
//We can even comment this entire thing. 
void log_conn(struct fuse_conn_info *conn)
{
    log_msg("    conn:\n");
    
    /** Major version of the protocol (read-only) */
   
    log_struct(conn, proto_major, %d, );

    /** Minor version of the protocol (read-only) */
  
    log_struct(conn, proto_minor, %d, );

    /** Is asynchronous read supported (read-write) */
  
    log_struct(conn, async_read, %d, );

    /** Maximum size of the write buffer */
  
    log_struct(conn, max_write, %d, );
    
    /** Maximum readahead */
 
    log_struct(conn, max_readahead, %d, );
    
    /** Capability flags, that the kernel supports */
    
    log_struct(conn, capable, %08x, );
    
    /** Capability flags, that the filesystem wants to enable */

    log_struct(conn, want, %08x, );
    
    /** Maximum number of backgrounded requests */
    log_struct(conn, max_background, %d, );
    
    /** Kernel congestion threshold parameter */
    
    log_struct(conn, congestion_threshold, %d, );
    
    /** For future use. */
    // unsigned reserved[23];
}
    
// struct fuse_file_info keeps information about files(SHERLOCKED? ) 
// This dumps all the information in a struct fuse_file_info.  The struct
// definition, and comments, come from /usr/include/fuse/fuse_common.h
//and some website, i've mailed it on our project group. 
//The same have been reflected here so we all can understand it 

void log_fi (struct fuse_file_info *fi)
{
    log_msg("    fi:\n");
    
    /** Open flags.  Available in open() and release() */
   
	log_struct(fi, flags, 0x%08x, );
	
    /** Old file handle, don't use */
	log_struct(fi, fh_old, 0x%08lx,  );

    /** In case of a write operation indicates if this was caused by a
        writepage */
	log_struct(fi, writepage, %d, );

    /** Can be filled in by open, to use direct I/O on this file.
        Introduced in version 2.4 */
	log_struct(fi, direct_io, %d, );

    /** Can be filled in by open, to indicate, that cached file data
        need not be invalidated.  Introduced in version 2.4 */
	log_struct(fi, keep_cache, %d, );

     /** File handle.  May be filled in by filesystem in open().
        Available in all other file operations */
	log_struct(fi, fh, 0x%016llx,  );
	
    /** Lock owner id.  Available in locking operations and flush */
  
	log_struct(fi, lock_owner, 0x%016llx, );
};

// This dumps the info from a struct stat.  The struct is defined in
// <bits/stat.h>; this is indirectly included from <fcntl.h>
void log_stat(struct stat *si)
{
    log_msg("    si:\n");
    
/* ID of device containing file */
	log_struct(si, st_dev, %lld, );
	
/* inode number */
	log_struct(si, st_ino, %lld, );
	
/* protection */
	log_struct(si, st_mode, 0%o, );
	
/* number of hard links */
	log_struct(si, st_nlink, %d, );
	
/* user ID of owner */
	log_struct(si, st_uid, %d, );
	
  /* group ID of owner */
	log_struct(si, st_gid, %d, );
	
 /* device ID (if special file) */
	log_struct(si, st_rdev, %lld,  );
	
 /* total size, in bytes */
	log_struct(si, st_size, %lld,  );
	
/* blocksize for filesystem I/O */
	log_struct(si, st_blksize, %ld,  );

/* number of blocks allocated */
	log_struct(si, st_blocks, %lld,  );

 /* time of last access */
	log_struct(si, st_atime, 0x%08lx, );

/* time of last modification */
	log_struct(si, st_mtime, 0x%08lx, );

 /* time of last status change */
	log_struct(si, st_ctime, 0x%08lx, );
	
}

void log_statvfs(struct statvfs *sv)
{
    log_msg("    sv:\n");
    
/* file system block size */
	log_struct(sv, f_bsize, %ld, );
	
  /* fragment size */
	log_struct(sv, f_frsize, %ld, );
	
     /* size of fs in f_frsize units */
	log_struct(sv, f_blocks, %lld, );
	   
    /* # free blocks */
	log_struct(sv, f_bfree, %lld, );
 /* # free blocks for non-root */
	log_struct(sv, f_bavail, %lld, );
	
    /* # inodes */
	log_struct(sv, f_files, %lld, );
	
     /* # free inodes */
	log_struct(sv, f_ffree, %lld, );
	
     /* # free inodes for non-root */
	log_struct(sv, f_favail, %lld, );
	
     /* file system ID */
	log_struct(sv, f_fsid, %ld, );
	
      /* mount flags */
	log_struct(sv, f_flag, 0x%08lx, );
	
 /* maximum filename length */
	log_struct(sv, f_namemax, %ld, );
	
}

void log_utime(struct utimbuf *buf)
{
    log_msg("    buf:\n");
    log_struct(buf, actime, 0x%08lx, );
	
    log_struct(buf, modtime, 0x%08lx, );
}
