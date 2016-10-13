//use fuse version 26
//started from hello.c, now we're here !
//tu beer hai.

#include "params.h"
//include<extractor.h> //newly added for classifying metadata
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <fuse.h>
#include <libgen.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#ifdef HAVE_SYS_XATTR_H
#include <sys/xattr.h>
#endif

#include "log.h"
//unsigned int FLAG_GLOBAL = 0;
//unsigned int counter=0;
char pathformeta[PATH_MAX]="python /";


// Report errors to logfile and give -errno to caller
static int scfs_error(char *str)
{
    int ret = -errno;
    
    log_msg("    ERROR %s: %s\n", str, strerror(errno));
    
    return ret;
}
char GLO[PATH_MAX];
//get the location of metaextractor
char metalocation[PATH_MAX];
void foo(){
 char path[PATH_MAX];
  char dest[PATH_MAX];
  struct stat info;
  pid_t pid = getpid();
  sprintf(path, "/proc/%d/exe", pid);
  if (readlink(path, dest, PATH_MAX) == -1)
    perror("readlink");
  strcpy(GLO,dest);
  }
static void classify (char fullpath[PATH_MAX], int boo){
char buf[PATH_MAX];
//char buffer[PATH_MAX] = "python home/poizen/MetaExtractor/newExtractor.py '";
char buffer[PATH_MAX];
strcpy(buffer,pathformeta);
log_msg("\n BUFFER HOLDS: %s",buffer);
if(boo == 0){
strcat(buffer,fullpath);
strcat(buffer,"'");
strcat(buffer," '");
strcat(buffer,SCFS_DATA->rootdir);
strcat(buffer,"'");
system(buffer);
}
else if(boo==1){
log_msg("\nPREclassified\n");
strcat(buffer,fullpath);
strcat(buffer,"'");
strcat(buffer," '");
strcat(buffer,SCFS_DATA->rootdir);
strcat(buffer,"'");
strcat(buffer," 'YES'");
system(buffer);
}

}
// Check whether the given user is permitted to perform the given operation on the given 

//  All the paths I see are relative to the root of the mounted
//  filesystem.  In order to get to the underlying filesystem, I need to
//  have the mountpoint.  I'll save it away early on in main(), and then
//  whenever I need a path for something I'll call this to construct
//  it.
/*

static void scfs_fullpath(char fpath[PATH_MAX], const char *path)
{ //there is some issue while the initial mounting. therefore for the first 34 times, we cannot execute this. 
char text[PATH_MAX];
char buffer[PATH_MAX] ="python /home/poizen/PycharmProjects/MetaExtractor/metaextractor.py ";
    
FILE *f;
FILE *ls;
strcpy(fpath, SCFS_DATA->rootdir);
   strcpy(text,fpath); 
 strncat(fpath, path, PATH_MAX);// very long paths will break here
  counter++; //34
 strcat(buffer, fpath);   
  log_msg("\n PYTHON_buffer: %s",buffer);
  	
   
 
    
  
    // we should be using a socket here, but just to make this work for
    // now 
  if (FLAG_GLOBAL == 0) {
  	FLAG_GLOBAL ++;
	log_msg("\nVADAPAV: GLOBAL! %d\n",FLAG_GLOBAL);
	f = fopen("/home/poizen/file.txt", "w");	// writing the
							// root dir here. 
	if (f == NULL) {
	    log_msg("\nVADAPAV:Error opening file!\n");
	  	error(1);
	}

	fprintf(f, "%s", text);
	log_msg("\nVADAPAV text: %s", text);
		// we just need to write the rootdir once. 
				// 

    }
 
    log_msg("\n COUNTER: %d",counter);
    //we just wrote the root dir . 
 
 if(counter>34)
   {
      ls=popen(buffer,"r"); 
    
     do{ 
     if(feof(ls)) 
     	{ 
    log_msg("\nEOF!!\n");
     break; 
     } 
     while(fgets(buffer,sizeof(buffer),ls)!=0){
     log_msg("\nPYTHON returned: %s",buffer);
     
     }
      }
     while(1) ; 
     pclose(ls);
     }
 
    
    pclose(f);
    
    log_msg
	("    scfs_fullpath:  rootdir = \"%s\", path = \"%s\", fpath = \"%s\"\n",
	 SCFS_DATA->rootdir, path, fpath);
}
*/
static void scfs_fullpath(char fpath[PATH_MAX], const char *path)
{ 

strcpy(fpath, SCFS_DATA->rootdir);
//strcpy(fpath,"/testdir/");
strncat(fpath, path, PATH_MAX); // very long paths will break here
log_msg("    scfs_fullpath:  rootdir = \"%s\", path = \"%s\", fpath = \"%s\"\n", SCFS_DATA->rootdir, path, fpath);
/*
//add conditions for classifying data that already has been
int c;
int b;
char getpath[200];
char boolean[1];
// checking if it has been previously classified

if (c = getxattr(fpath,"user.scfsC",NULL,0)!=-1){


log_msg("$$$$ inside");
c = getxattr(fpath,"user.scfsC",boolean,1);
if(boolean[0]=='Y'){
log_msg("$$$$ inside %c",boolean[0]);
classify(fpath,1);
}
}
else if(boolean[0]=='N'){
//DO NOTHING
}
else{
classify(fpath,0);
}
*/

}


///////////////////////////////////////////////////////////
//
// Prototypes for all these functions, and the C-style comments,
// come indirectly from /usr/include/fuse.h
//
/** Get file attributes.
 *
 * Similar to stat().  The 'st_dev' and 'st_blksize' fields are
 * ignored.  The 'st_ino' field is ignored except if the 'use_ino'
 * mount option is given.
 */
int scfs_getattr(const char *path, struct stat *statbuf)
{
    int retstat = 0;
    char fpath[PATH_MAX];
    
    log_msg("\nscfs_getattr(path=\"%s\", statbuf=0x%08x)\n",
	  path, statbuf);
    scfs_fullpath(fpath, path);
    //lstat in sys/stat.h, we use it from there. its like stat. just helps with the symb. links :D
    //lstat seems to be broken, need to fix this.
    retstat = lstat(fpath, statbuf); 
    if (retstat != 0)
	retstat = scfs_error("scfs_getattr stat");
    
    log_stat(statbuf);
    
    
    return retstat;
}

/** Read the target of a symbolic link
 *
 * The buffer should be filled with a null terminated string.  The
 * buffer size argument includes the space for the terminating
 * null character.  If the linkname is too long to fit in the
 * buffer, it should be truncated.  The return value should be 0
 * for success.
 */
// Note the system readlink() will truncate and lose the terminating
// null.  So, the size passed to to the system readlink() must be one
// less than the size passed to scfs_readlink()
// scfs_readlink() code by Bernardo F Costa (thanks!)
int scfs_readlink(const char *path, char *link, size_t size)
{
	//log_msg("This:: READLINK CALLED.  \n");
    int retstat = 0;
    char fpath[PATH_MAX];
    
    log_msg("scfs_readlink(path=\"%s\", link=\"%s\", size=%d)\n",
	  path, link, size);
    scfs_fullpath(fpath, path);
    
    retstat = readlink(fpath, link, size - 1); //returns count of bytes placed in buffer, otherwise, it returns -1
    if (retstat < 0)
	retstat = scfs_error("scfs_readlink readlink");
    else  {
	//terminate with null, since the function should understand where to stop.
    link[retstat] = '\0';
	retstat = 0;
    }
    
    return retstat;
}

/** Create a file node
 *
 * There is no create() operation, mknod() will be called for
 * creation of all non-directory, non-symlink nodes.
 */
// shouldn't that comment be "if" there is no.... ?
int scfs_mknod(const char *path, mode_t mode, dev_t dev)
{
    int retstat = 0;
    char fpath[PATH_MAX];
    log_msg("MKNOD is called \n");
    log_msg("\nscfs_mknod(path=\"%s\", mode=0%3o, dev=%lld)\n",
	  path, mode, dev);
    scfs_fullpath(fpath, path);
    
    // On Linux this could just be 'mknod(path, mode, rdev)' but this
    //  is more portable
    if (S_ISREG(mode)) {
        retstat = open(fpath, O_CREAT | O_EXCL | O_WRONLY, mode);
        		if (retstat < 0)
        			retstat = scfs_error("scfs_mknod open");
        		else {
        			retstat = close(retstat);
        			if (retstat < 0)
	    			retstat = scfs_error("scfs_mknod close");
        				}
    }
    else
	if (S_ISFIFO(mode)) {
	    retstat = mkfifo(fpath, mode);
	    if (retstat < 0)
		retstat = scfs_error("scfs_mknod mkfifo");
	} else {
	    retstat = mknod(fpath, mode, dev);
	    if (retstat < 0)
		retstat = scfs_error("scfs_mknod mknod");
	}
    
    return retstat;
}

/** Create a directory */
int scfs_mkdir(const char *path, mode_t mode)
{	//log_msg("This:: MKDIR is called \n");
    int retstat = 0;
    char fpath[PATH_MAX];
    
    log_msg("\nscfs_mkdir(path=\"%s\", mode=0%3o)\n",
	    path, mode);
    scfs_fullpath(fpath, path);
    
    retstat = mkdir(fpath, mode);
    if (retstat < 0)
	retstat = scfs_error("scfs_mkdir mkdir");
    
    return retstat;
}

/** Remove a file */
int scfs_unlink(const char *path)
{
    int retstat = 0;
    char fpath[PATH_MAX];
    //log_msg("This:: BB UNLINK CALLED \n");
    log_msg("scfs_unlink(path=\"%s\")\n",
	    path);
    scfs_fullpath(fpath, path);
    
    retstat = unlink(fpath);
    if (retstat < 0)
	retstat = scfs_error("scfs_unlink unlink");
    
    return retstat;
}

/** Remove a directory */
int scfs_rmdir(const char *path)
{ //can remove dir except for the parent and the parents baapu.
	//log_msg("This::RMDIR is called  \n");
    int retstat = 0;
    char fpath[PATH_MAX];
    
    log_msg("scfs_rmdir(path=\"%s\")\n",
	    path);
    scfs_fullpath(fpath, path);
    
    retstat = rmdir(fpath);
    if (retstat < 0)
	retstat = scfs_error("scfs_rmdir rmdir");
    
    return retstat;
}

/** Create a symbolic link */
// The parameters here are a little bit confusing, but do correspond
// to the symlink() system call.  The 'path' is where the link points,
// while the 'link' is the link itself.  So we need to leave the path
// unaltered, but insert the link into the mounted directory.
int scfs_symlink(const char *path, const char *link)
{ //passing "where to " and the link "name" given by "link"
    int retstat = 0;
    char flink[PATH_MAX];
    //log_msg("This:: YO YO SYMBI LINK\n");
    log_msg("\nscfs_symlink(path=\"%s\", link=\"%s\")\n",
	    path, link);
    scfs_fullpath(flink, link);
    
    retstat = symlink(path, flink);
    if (retstat < 0)
	retstat = scfs_error("scfs_symlink symlink");
    
    return retstat;
}

/** Rename a file */
// both path and newpath are fs-relative
int scfs_rename(const char *path, const char *newpath)
{	//log_msg("This:: hou de rename \n");
    int retstat = 0;
    char fpath[PATH_MAX];
    char fnewpath[PATH_MAX];
    
    log_msg("\nscfs_rename(fpath=\"%s\", newpath=\"%s\")\n",
	    path, newpath);
    scfs_fullpath(fpath, path);
    scfs_fullpath(fnewpath, newpath);
    
    retstat = rename(fpath, fnewpath);
    if (retstat < 0)
	retstat = scfs_error("scfs_rename rename");
    
    return retstat;
}

/** Create a hard link to a file */
int scfs_link(const char *path, const char *newpath)
{	//log_msg("This:: kahitari link hotay rao\n");
    int retstat = 0;
    char fpath[PATH_MAX], fnewpath[PATH_MAX];
    
    log_msg("\nscfs_link(path=\"%s\", newpath=\"%s\")\n",
	    path, newpath);
    scfs_fullpath(fpath, path);
    scfs_fullpath(fnewpath, newpath);
    
    retstat = link(fpath, fnewpath);
    if (retstat < 0)
	retstat = scfs_error("scfs_link link");
    
    return retstat;
}

/** Change the permission bits of a file */
int scfs_chmod(const char *path, mode_t mode)
{	//log_msg("This:: chmod karu  \n");
    int retstat = 0;
    char fpath[PATH_MAX];
    
    log_msg("\nscfs_chmod(fpath=\"%s\", mode=0%03o)\n",
	    path, mode);
    scfs_fullpath(fpath, path);
    
    retstat = chmod(fpath, mode);
    if (retstat < 0)
	retstat = scfs_error("scfs_chmod chmod");
    
    return retstat;
}

/** Change the owner and group of a file */
int scfs_chown(const char *path, uid_t uid, gid_t gid)
  
{	//log_msg("This:: ata chown karu \n");
    int retstat = 0;
    char fpath[PATH_MAX];
    
    log_msg("\nscfs_chown(path=\"%s\", uid=%d, gid=%d)\n",
	    path, uid, gid);
    scfs_fullpath(fpath, path);
    
    retstat = chown(fpath, uid, gid);
    if (retstat < 0)
	retstat = scfs_error("scfs_chown chown");
    
    return retstat;
}

/** Change the size of a file */
int scfs_truncate(const char *path, off_t newsize)
{ //basically, naavala CHOTA kivha MOTHA karta for a given path..
	/*truncate(const char* path, off_t size)
Truncate or extend the given file so that
it is precisely size bytes long. See truncate(2) for details.
This call is required for read/write filesystems,
because recreating a file will first truncate it.*/
    int retstat = 0;
    char fpath[PATH_MAX];
    //log_msg("This:: truncate cha meaning kai rao? \n");
    log_msg("\nscfs_truncate(path=\"%s\", newsize=%lld)\n",
	    path, newsize);
    scfs_fullpath(fpath, path);
    
    retstat = truncate(fpath, newsize);
    if (retstat < 0)
	scfs_error("scfs_truncate truncate");
    
    return retstat;
}

/** Change the access and/or modification times of a file */
/* note -- I'll want to change this as soon as 2.6 is in debian testing */
int scfs_utime(const char *path, struct utimbuf *ubuf)
{
    int retstat = 0;
    char fpath[PATH_MAX];
    //log_msg("This:: uname\n");
    log_msg("\nscfs_utime(path=\"%s\", ubuf=0x%08x)\n",
	    path, ubuf);
    scfs_fullpath(fpath, path);
    
    retstat = utime(fpath, ubuf);
    if (retstat < 0)
	retstat = scfs_error("scfs_utime utime");
    
    return retstat;
}

/** File open operation
 *
 * No creation, or truncation flags (O_CREAT, O_EXCL, O_TRUNC)
 * will be passed to open().  Open should check if the operation
 * is permitted for the given flags.  Optionally open may also
 * return an arbitrary filehandle in the fuse_file_info structure,
 * which will be passed to all file operations.
 *
 * Changed in version 2.2
 */
int scfs_open(const char *path, struct fuse_file_info *fi)
{	//log_msg("This:: open la bolavle ahe \n");
    int retstat = 0;
    int fd;
    char fpath[PATH_MAX];
    
    log_msg("\nscfs_open(path\"%s\", fi=0x%08x)\n",
	    path, fi);
    scfs_fullpath(fpath, path);
    
    fd = open(fpath, fi->flags);
    if (fd < 0)
	retstat = scfs_error("scfs_open open");
    
    fi->fh = fd;
    log_fi(fi);
    
    return retstat;
}

/** Read data from an open file
 *
 * Read should return exactly the number of bytes requested except
 * on EOF or error, otherwise the rest of the data will be
 * substituted with zeroes.  An exception to this is when the
 * 'direct_io' mount option is specified, in which case the return
 * value of the read system call will reflect the return value of
 * this operation.
 *
 * Changed in version 2.2
 */
// I don't fully understand the documentation above -- it doesn't
// match the documentation for the read() system call which says it
// can return with anything up to the amount of data requested. nor
// with the fusexmp code which returns the amount of data also
// returned by read.
int scfs_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
    int retstat = 0;
    //log_msg("This:: ATA ha read kartoy\n");
    log_msg("\nscfs_read(path=\"%s\", buf=0x%08x, size=%d, offset=%lld, fi=0x%08x)\n",
	    path, buf, size, offset, fi);
    // no need to get fpath on this one, since I work from fi->fh not the path
    log_fi(fi);
    
    retstat = pread(fi->fh, buf, size, offset);
    if (retstat < 0)
	retstat = scfs_error("scfs_read read");
    
    return retstat;
}

/** Write data to an open file
 *
 * Write should return exactly the number of bytes requested
 * except on error.  An exception to this is when the 'direct_io'
 * mount option is specified (see read operation).
 *
 * Changed in version 2.2
 */
// As  with read(), the documentation above is inconsistent with the
// documentation for the write() system call.
int scfs_write(const char *path, const char *buf, size_t size, off_t offset,
	     struct fuse_file_info *fi)
{
    int retstat = 0;
    //log_msg("This:: write hotay , wait kar \n");
    log_msg("\nscfs_write(path=\"%s\", buf=0x%08x, size=%d, offset=%lld, fi=0x%08x)\n",
	    path, buf, size, offset, fi
	    );
    // no need to get fpath on this one, since I work from fi->fh not the path
    log_fi(fi);
	
    retstat = pwrite(fi->fh, buf, size, offset);
    if (retstat < 0)
	retstat = scfs_error("scfs_write pwrite");
    
    return retstat;
}

/** Get file system statistics
 *
 * The 'f_frsize', 'f_favail', 'f_fsid' and 'f_flag' fields are ignored
 *
 * Replaced 'struct statfs' parameter with 'struct statvfs' in
 * version 2.5
 */
int scfs_statfs(const char *path, struct statvfs *statv)
{	//log_msg("This:: statfs kai ahe?  \n");
    int retstat = 0;
    char fpath[PATH_MAX];
    
    log_msg("\nscfs_statfs(path=\"%s\", statv=0x%08x)\n",
	    path, statv);
    scfs_fullpath(fpath, path);
    
    // get stats for underlying filesystem
    retstat = statvfs(fpath, statv);
    if (retstat < 0)
	retstat = scfs_error("scfs_statfs statvfs");
    
    log_statvfs(statv);
    
    return retstat;
}

int scfs_flush(const char *path, struct fuse_file_info *fi)
{
    int retstat = 0;
    //log_msg("This:: FLUSH KARTOY \n");
    log_msg("\nscfs_flush(path=\"%s\", fi=0x%08x)\n", path, fi);
    // no need to get fpath on this one, since I work from fi->fh not the path
    log_fi(fi);
	
    return retstat;
}

/** Release an open file
 *
 * Release is called when there are no more references to an open
 * file: all file descriptors are closed and all memory mappings
 * are unmapped.
 *
 * For every open() call there will be exactly one release() call
 * with the same flags and file descriptor.  It is possible to
 * have a file opened more than once, in which case only the last
 * release will mean, that no more reads/writes will happen on the
 * file.  The return value of release is ignored.
 *
 * Changed in version 2.2
 */
int scfs_release(const char *path, struct fuse_file_info *fi)
{
    char fpath[PATH_MAX];
    
    int retstat = 0;
    
    //log_msg("\nscfs_release(path=\"%s\", fi=0x%08x)\n",
	  //path, fi);
    log_fi(fi);

    scfs_fullpath(fpath,path);
    //classify(fpath);
    //add conditions for classifying data that already has been
    int c;
    char boolean[1];
// checking if it has been previously classified

    if (c = getxattr(fpath,"user.scfsC",NULL,0)!=-1){
    c = getxattr(fpath,"user.scfsC",boolean,1);
    if(boolean[0]=='Y'){
    log_msg("$$$$ inside %c",boolean[0]);
    classify(fpath,1);
}
}
    else if(boolean[0]=='N'){
//DO NOTHING
    }
    else
    {
    classify(fpath,0);
}
    
    // We need to close the file.  Had we allocated any resources
    // (buffers etc) we'd need to free them here as well.

    
    retstat = close(fi->fh);  
    return retstat;
}

/** Synchronize file contents
 *
 * If the datasync parameter is non-zero, then only the user data
 * should be flushed, not the meta data.
 *
 * Changed in version 2.2
 */
int scfs_fsync(const char *path, int datasync, struct fuse_file_info *fi)
{	//log_msg("This:: je zhala te (f)SYNC karu de \n");
    int retstat = 0;
    
    log_msg("\nscfs_fsync(path=\"%s\", datasync=%d, fi=0x%08x)\n",
	    path, datasync, fi);
    log_fi(fi);
    
    // some unix-like systems (notably freebsd) don't have a datasync call
#ifdef HAVE_FDATASYNC
    if (datasync)
	retstat = fdatasync(fi->fh);
    else
#endif	
	retstat = fsync(fi->fh);
    
    if (retstat < 0)
	scfs_error("scfs_fsync fsync");
    
    return retstat;
}

#ifdef HAVE_SYS_XATTR_H
/** Set extended attributes */
int scfs_setxattr(const char *path, const char *name, const char *value, size_t size, int flags)
{
	//log_msg("This:: metadata la XATTR use karu shakto o.O \n");
    int retstat = 0;
    char fpath[PATH_MAX];
    
    log_msg("\nscfs_setxattr(path=\"%s\", name=\"%s\", value=\"%s\", size=%d, flags=0x%08x)\n",
	    path, name, value, size, flags);
    scfs_fullpath(fpath, path);
    
    retstat = lsetxattr(fpath, name, value, size, flags);
    if (retstat < 0)
	retstat = scfs_error("scfs_setxattr lsetxattr");
    
    return retstat;
}

/** Get extended attributes */
int scfs_getxattr(const char *path, const char *name, char *value, size_t size)
{	//log_msg("This:: getting the xattr \n");
    int retstat = 0;
    char fpath[PATH_MAX];
    
    log_msg("\nscfs_getxattr(path = \"%s\", name = \"%s\", value = 0x%08x, size = %d)\n",
	    path, name, value, size);
    scfs_fullpath(fpath, path);
    
    retstat = lgetxattr(fpath, name, value, size);
    if (retstat < 0)
	retstat = scfs_error("scfs_getxattr lgetxattr");
    else
	log_msg("    value = \"%s\"\n", value);
    
    return retstat;
}

/** List extended attributes */
int scfs_listxattr(const char *path, char *list, size_t size)
{	//log_msg("This:: listing xattr \n");
    int retstat = 0;
    char fpath[PATH_MAX];
    char *ptr;
    
    log_msg("scfs_listxattr(path=\"%s\", list=0x%08x, size=%d)\n",
	    path, list, size
	    );
    scfs_fullpath(fpath, path);
    
    retstat = llistxattr(fpath, list, size);
    if (retstat < 0)
	retstat = scfs_error("scfs_listxattr llistxattr");
    
    log_msg("    returned attributes (length %d):\n", retstat);
    for (ptr = list; ptr < list + retstat; ptr += strlen(ptr)+1)
	log_msg("    \"%s\"\n", ptr);
    
    return retstat;
}

/** Remove extended attributes */
int scfs_removexattr(const char *path, const char *name)
{	//log_msg("This:: remove xattr \n");
    int retstat = 0;
    char fpath[PATH_MAX];
    
    log_msg("\nscfs_removexattr(path=\"%s\", name=\"%s\")\n",
	    path, name);
    scfs_fullpath(fpath, path);
    
    retstat = lremovexattr(fpath, name);
    if (retstat < 0)
	retstat = scfs_error("scfs_removexattr lrmovexattr");
    
    return retstat;
}
#endif

/** Open directory
 *
 * This method should check if the open operation is permitted for
 * this  directory
 *
 * Introduced in version 2.3
 */
int scfs_opendir(const char *path, struct fuse_file_info *fi)
{
    DIR *dp;
    int retstat = 0;
    char fpath[PATH_MAX];
    //log_msg("This:: open kartoy toh directory  \n");
    log_msg("\nscfs_opendir(path=\"%s\", fi=0x%08x)\n",
	  path, fi);
    scfs_fullpath(fpath, path);
    
    dp = opendir(fpath);
    if (dp == NULL)
	retstat = scfs_error("scfs_opendir opendir");
    
    fi->fh = (intptr_t) dp;
    
    log_fi(fi);
    
    return retstat;
}

/** Read directory
 *
 * This supersedes the old getdir() interface.  New applications
 * should use this.
 *
 * The filesystem may choose between two modes of operation:
 *
 * 1) The readdir implementation ignores the offset parameter, and
 * passes zero to the filler function's offset.  The filler
 * function will not return '1' (unless an error happens), so the
 * whole directory is read in a single readdir operation.  This
 * works just like the old getdir() method.
 *
 * 2) The readdir implementation keeps track of the offsets of the
 * directory entries.  It uses the offset parameter and always
 * passes non-zero offset to the filler function.  When the buffer
 * is full (or an error happens) the filler function will return
 * '1'.
 *
 * Introduced in version 2.3
 */
int scfs_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset,
	       struct fuse_file_info *fi)
{
    int retstat = 0;
    DIR *dp;
    struct dirent *de;
    //log_msg("This:: now its reading the dir o.O \n");
    log_msg("\nscfs_readdir(path=\"%s\", buf=0x%08x, filler=0x%08x, offset=%lld, fi=0x%08x)\n",
	    path, buf, filler, offset, fi);
    // once again, no need for fullpath -- but note that I need to cast fi->fh
    dp = (DIR *) (uintptr_t) fi->fh;

    // Every directory contains at least two entries: . and ..  If my
    // first call to the system readdir() returns NULL I've got an
    // error; near as I can tell, that's the only condition under
    // which I can get an error from readdir()
    de = readdir(dp);
    if (de == 0) {
	retstat = scfs_error("scfs_readdir readdir");
	return retstat;
    }

    // This will copy the entire directory into the buffer.  The loop exits
    // when either the system readdir() returns NULL, or filler()
    // returns something non-zero.  The first case just means I've
    // read the whole directory; the second means the buffer is full.
    do {
	log_msg("calling filler with name %s\n", de->d_name);
	if (filler(buf, de->d_name, NULL, 0) != 0) {
	    log_msg("    ERROR scfs_readdir filler:  buffer full");
	    return -ENOMEM;
	}
    } while ((de = readdir(dp)) != NULL);
    
    log_fi(fi);
    
    return retstat;
}

/** Release directory
 *
 * Introduced in version 2.3
 */
int scfs_releasedir(const char *path, struct fuse_file_info *fi)
{
    int retstat = 0;
    //log_msg("This:: one must not simple RELEASE DIR \n");
    log_msg("\nscfs_releasedir(path=\"%s\", fi=0x%08x)\n",
	    path, fi);
    log_fi(fi);
    
    closedir((DIR *) (uintptr_t) fi->fh);
    
    return retstat;
}

/** Synchronize directory contents
 *
 * If the datasync parameter is non-zero, then only the user data
 * should be flushed, not the meta data
 *
 * Introduced in version 2.3
 */
// when exactly is this called?  when a user calls fsync and it
// happens to be a directory? ???
int scfs_fsyncdir(const char *path, int datasync, struct fuse_file_info *fi)
{
    int retstat = 0;
    //log_msg("This:: fsyncDIR \n");
    log_msg("\nscfs_fsyncdir(path=\"%s\", datasync=%d, fi=0x%08x)\n",
	    path, datasync, fi);
    log_fi(fi);
    
    return retstat;
}

/**
 * Initialize filesystem
 *
 * The return value will passed in the private_data field of
 * fuse_context to all file operations and as a parameter to the
 * destroy() method.
 *
 * Introduced in version 2.3
 * Changed in version 2.6
 */
// Undocumented but extraordinarily useful fact:  the fuse_context is
// set up before this function is called, and
// fuse_get_context()->private_data returns the user_data passed to
// fuse_main().  Really seems like either it should be a third
// parameter coming in here, or else the fact should be documented
// (and this might as well return void, as it did in older versions of
// FUSE).
void *scfs_init(struct fuse_conn_info *conn)
{
    log_msg("\nscfs_init()\n");
    //log_msg("This:: INITIALIZE ZHALA!!  \n");
    log_conn(conn);
    log_fuse_context(fuse_get_context());
    
    return SCFS_DATA;
}

/**
 * Clean up filesystem
 *
 * Called on filesystem exit.
 *
 * Introduced in version 2.3
 */
void scfs_destroy(void *userdata)
{
	//log_msg("This:: wada kia toh DESTROY karna padenga \n");
    log_msg("\nscfs_destroy(userdata=0x%08x)\n", userdata);
}

/**
 * Check file access permissions
 *
 * This will be called for the access() system call.  If the
 * 'default_permissions' mount option is given, this method is not
 * called.
 *
 * This method is not called under Linux kernel versions 2.4.x
 *
 * Introduced in version 2.5
 */
int scfs_access(const char *path, int mask)
{	//log_msg("This::access dili  \n");
    int retstat = 0;
    char fpath[PATH_MAX];
   
    log_msg("\nscfs_access(path=\"%s\", mask=0%o)\n",
	    path, mask);
    scfs_fullpath(fpath, path);
    
    retstat = access(fpath, mask);
    
    if (retstat < 0)
	retstat = scfs_error("scfs_access access");
    
    return retstat;
}

/**
 * Create and open a file
 *
 * If the file does not exist, first create it with the specified
 * mode, and then open it.
 *
 * If this method is not implemented or under Linux kernel
 * versions earlier than 2.6.15, the mknod() and open() methods
 * will be called instead.
 *
 * Introduced in version 2.5
 */
int scfs_create(const char *path, mode_t mode, struct fuse_file_info *fi)
{	//log_msg("This:: create is called \n");
    int retstat = 0;
    char fpath[PATH_MAX];
    int fd;
    
    log_msg("\nscfs_create(path=\"%s\", mode=0%03o, fi=0x%08x)\n",
	    path, mode, fi);
    scfs_fullpath(fpath, path);
    
    fd = creat(fpath, mode);
    if (fd < 0)
	retstat = scfs_error("scfs_create creat");
    
    fi->fh = fd;
    
    log_fi(fi);
    
    return retstat;
}

/**
 * Change the size of an open file
 *
 * This method is called instead of the truncate() method if the
 * truncation was invoked from an ftruncate() system call.
 *
 * If this method is not implemented or under Linux kernel
 * versions earlier than 2.6.15, the truncate() method will be
 * called instead.
 *
 * Introduced in version 2.5
 */
int scfs_ftruncate(const char *path, off_t offset, struct fuse_file_info *fi)
{
    int retstat = 0;
    //log_msg("This:: ftruncate kai ahe ? \n");
    log_msg("\nscfs_ftruncate(path=\"%s\", offset=%lld, fi=0x%08x)\n",
	    path, offset, fi);
    log_fi(fi);
    
    retstat = ftruncate(fi->fh, offset);
    if (retstat < 0)
	retstat = scfs_error("scfs_ftruncate ftruncate");
    
    return retstat;
}

/**
 * Get attributes from an open file
 *
 * This method is called instead of the getattr() method if the
 * file information is available.
 *
 * Currently this is only called after the create() method if that
 * is implemented (see above).  Later it may be called for
 * invocations of fstat() too.
 *
 * Introduced in version 2.5
 */
int scfs_fgetattr(const char *path, struct stat *statbuf, struct fuse_file_info *fi)
{
    int retstat = 0;
    log_msg("\nscfs_fgetattr(path=\"%s\", statbuf=0x%08x, fi=0x%08x)\n",
	    path, statbuf, fi);
    log_fi(fi);

   
    if (!strcmp(path, "/"))
	return scfs_getattr(path, statbuf);
    
    retstat = fstat(fi->fh, statbuf);
    if (retstat < 0)
	retstat = scfs_error("scfs_fgetattr fstat");
    
    log_stat(statbuf);
    
    return retstat;
}

struct fuse_operations scfs_oper = {
  .getattr = scfs_getattr,
  .readlink = scfs_readlink,
  // no .getdir -- that's deprecated
  .getdir = NULL,
  .mknod = scfs_mknod,
  .mkdir = scfs_mkdir,
  .unlink = scfs_unlink,
  .rmdir = scfs_rmdir,
  .symlink = scfs_symlink,
  .rename = scfs_rename,
  .link = scfs_link,
  .chmod = scfs_chmod,
  .chown = scfs_chown,
  .truncate = scfs_truncate,
  .utime = scfs_utime,
  .open = scfs_open,
  .read = scfs_read,
  .write = scfs_write,
  /** Just a placeholder, don't set */ // huh???
  .statfs = scfs_statfs,
  .flush = scfs_flush,
  .release = scfs_release,
  .fsync = scfs_fsync,
  
#ifdef HAVE_SYS_XATTR_H
  .setxattr = scfs_setxattr,
  .getxattr = scfs_getxattr,
  .listxattr = scfs_listxattr,
  .removexattr = scfs_removexattr,
#endif
  
  .opendir = scfs_opendir,
  .readdir = scfs_readdir,
  .releasedir = scfs_releasedir,
  .fsyncdir = scfs_fsyncdir,
  .init = scfs_init,
  .destroy = scfs_destroy,
  .access = scfs_access,
  .create = scfs_create,
  .ftruncate = scfs_ftruncate,
  .fgetattr = scfs_fgetattr
};

void scfs_usage()
{
    fprintf(stderr, "usage:  scfs [FUSE and mount options] rootDir mountPoint\n");
    abort();
}

int main(int argc, char *argv[])
{
    int fuse_stat;
    struct scfs_state *scfs_data;
    //Get current exeucting directory to execute the newExtractor
         char *pch;
        foo();
        printf("\n GOOO: %s",GLO);
        pch = strtok(GLO,"/");
      while (pch != NULL)
        {
         if(strcmp(pch,"scfs")!=0){
                printf("\nif: %s \n ", pch);
                strcat(pathformeta,pch);
                strcat(pathformeta,"/");
        }
        pch= strtok(NULL,"/");
        
        }
        
        strcat(pathformeta,"newExtractor.py '");
 
  
    if ((getuid() == 0) || (geteuid() == 0)) {
	fprintf(stderr, "Running SCFS as root opens unnacceptable security holes\nThe reason we're giving you SCFS in a USER mode, is so that everyone can use it normally. \n why would you want to open it as ROOT? :/ ");
	return 1;
    }
    
    // Perform some sanity checking on the command line:  make sure
    // there are enough arguments, and that neither of the last two
    // start with a hyphen (this will break if you actually have a
    // rootpoint or mountpoint whose name starts with a hyphen, but so
    // will a zillion other programs)
    if ((argc < 3) || (argv[argc-2][0] == '-') || (argv[argc-1][0] == '-'))
	scfs_usage();

    scfs_data = malloc(sizeof(struct scfs_state));
    if (scfs_data == NULL) {
	perror("main calloc");
	abort();
    }

    // Pull the rootdir out of the argument list and save it in my
    // internal data
    scfs_data->rootdir = realpath(argv[argc-2], NULL);
    argv[argc-2] = argv[argc-1];
    argv[argc-1] = NULL;
    argc--;
    
    scfs_data->logfile = log_open();
    
    // turn over control to fuse
    fprintf(stderr, "about to call fuse_main\n");
    fuse_stat = fuse_main(argc, argv, &scfs_oper, scfs_data);
    fprintf(stderr, "fuse_main returned %d\n", fuse_stat);
    
    return fuse_stat;
}
