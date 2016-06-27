# scfs
Smart classifier filesystem
it is a filesystem in user space, you can use two methods to classify either
a machine learning approach or a simple decision tree. 


to install this, just run 
$./configure OR $./directinstall 
$make (only if you configure it) 


in the src directory
Run make to compile code
an object file scfs will be created 
use this to mount the directories,

eg: 

$./scfs dir1/ dir2/ 

^this will mount dir1 on dir2 with the fileoperations implemented in our Filesystem
you can check if its "actually" mounted by 

$ mount | grep "scfS" 

to check the filesystem type you can run the command 
$df -h -T

NOTETOSELF: 
	
>	changed scfs.c to classify at utime

>	changed newextractor.py to accept series and also music files which don't have enough metadata but they have information
	in their names. 