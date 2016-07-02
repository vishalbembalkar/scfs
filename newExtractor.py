from hachoir_core.error import HachoirError
from hachoir_core.cmd_line import unicodeFilename
from hachoir_parser import createParser
from hachoir_metadata import extractMetadata
from guessit import guessit  # need to do this or object wont be callable
from sys import stderr, exit
from mimetypes import MimeTypes
#from hachoir_core.tools import makePrintable
#from hachoir_core.i18n import getTerminalCharset

import xattr
import urllib
import os
import calendar
import errno
import shutil
import glob
import sys
import csv
import omdb
import geocoder
import xml.etree.ElementTree as ET

def classify(path,rootdir): # add an extra argument here to take the root dir  :)

    print 'path given: ', path,' RootDir: ',rootdir
    foo = path.rsplit('/', 1)
    fname = foo[1]

    # defaults audio, video:
    artist = album = genre = 'unknown'

    # defaults image:
    latitude = longitude = 0
    city = state = country = 'unknown'
    year = '1960'
    month = 'January'
    # here we go :
    filename = path
    filename, realname = unicodeFilename(filename), filename
    parser = createParser(filename, realname)
    if not parser:
        print >>stderr, "Unable to parse file"
        exit(1)
    try:
        metadata = extractMetadata(parser)
    except HachoirError, err:
        print "Metadata extraction error: %s" % unicode(err)
        metadata = None
    if not metadata:
        print "Unable to extract metadata"
        mime = MimeTypes()
        url = urllib.pathname2url(path)
        mimetype = mime.guess_type(url)
        mimetype = mimetype[0]
        print mimetype
        #exit(1)
    try:
        metalist = metadata.exportPlaintext()
        for i in range(0, len(metalist)):
            checktype = metalist[i].split(':')
            if '- MIME type' ==  checktype[0]:
                checktype = checktype[1]
                checktype = checktype.split('/')
                mimetype  = checktype[0].strip()
    except:
        pass

# classification starts here

    if mimetype == 'audio':
        for i in range(0, len(metalist)):
            information = metalist[i].split(":")
            if (information[0] == '- Author'):
                artist = information[1].strip()

            elif (information[0] == '- Album'):
                album = information[1].strip()
            elif (information[0] == '- Music genre'):
                genre = information[1].strip()
                # have to add this to setfattr.
        valueofsetx = '/music/' + artist + '/' + album + '/'
        default_path_for_music = rootdir+valueofsetx

        destination = default_path_for_music+fname
        if path != destination:
            cmd = default_path_for_music
            print cmd
            try:
                os.makedirs(cmd, 0755)
            except OSError as e:
                if e.errno == 17:
                    os.chmod(cmd, 0755)
            shutil.move(path, destination)
            #adding the attribute
           # xattr.setxattr(destination, 'user.fpath', valueofsetx)
            print 'file moved to dir: ', destination

    elif mimetype == 'video':
        f = guessit(fname)
        extension = fname.rsplit('.', 1)
        getdata = omdb.title(f['title'])
        if getdata.type == 'movie':
            newfilename = str(getdata.title)+'.'+extension[1]
            valueofsetx = '/movies/' + getdata.genre+'/'+str(getdata.title)+'.'+extension[1]
            valueofsetx_withoutfilename = '/movies/' + getdata.genre+'/'
            default_path_for_movies = rootdir + valueofsetx_withoutfilename
            destination = default_path_for_movies
            if path != destination:
                cmd = default_path_for_movies
                try:
                    os.makedirs(cmd, 0755)
                except OSError as e:
                    print e
                    if e.errno == 17:
                        os.chmod(cmd, 0755)
                try:
                    # change this like for srt files, this is too arbitrary.
                    subs_at_root = foo[0] + '/Subs/'
                    destination_subs = default_path_for_movies
                    shutil.move(subs_at_root, destination_subs)
                except:
                    pass
                try:
                    files = glob.iglob(os.path.join(foo[0], "*.srt"))
                    for file in files:
                        if os.path.isfile(file):
                            shutil.move(file, default_path_for_movies)
                except:
                    pass
                destination = rootdir + valueofsetx
                shutil.move(path, destination)
                print 'file moved to dir ', destination
        if getdata.type == 'series':
            seasons = guessit(fname)

            # TODO: just the way I changed the how episode 1 will become episode 01, change the same thing
            # TODO: for seasons.
            try:
                season = str(seasons['season'])
                if season < 9:
                    season = '0' + str(season)
            except:
                season = '1'
            episode = seasons['episode']
            if episode < 9:
                episode = '0' + str(episode)
            valueofsetx = '/series/' + str(getdata.genre) + '/' + str(getdata.title)+'/Season '+ season+'/'\
                          + str(getdata.title) +' S0'+season+'e'+episode+'.'+extension[1]
            valueofsetx_withoutfilename = '/series/' + str(getdata.genre) + '/' + str(getdata.title)+'/Season '+ season+'/'
            default_path_for_series = rootdir + valueofsetx_withoutfilename
            destination = default_path_for_series
            print 'doo',destination
            if path != destination:
                cmd = default_path_for_series
                try:
                    print 'cmd',cmd
                    os.makedirs(cmd, 0755)
                except OSError as e:
                    if e.errno == 17:
                        os.chmod(cmd, 0755)
                try:
                    # change this like for srt files, this is too arbitrary.
                    subs_at_root = foo[0] + '/Subs/'
                    destination_subs = default_path_for_series
                    shutil.move(subs_at_root, destination_subs)
                except:
                    pass
                try:
                    files = glob.iglob(os.path.join(foo[0], "*.srt"))
                    for file in files:
                        if os.path.isfile(file):
                            shutil.move(file, default_path_for_series)
                except:
                    pass
                destination = rootdir + valueofsetx
                #shutil.move(path, destination)
                print 'file moved to dir ', destination
        '''getdata = omdb.request(t=f['title'], y=f['year'], r='xml')
        root = ET.fromstring(getdata.content)
        for genre in root.findall('movie'):
            genre = genre.get('genre')


        valueofsetx =  "/movies/" + genre+"/"
        if path != destination:
            cmd = default_path_for_movies
            print cmd
            try:
                os.makedirs(cmd, 0755)
            except OSError as e:
                if e.errno == 17:
                    os.chmod(cmd, 0755)
            try:
                # change this like for srt files, this is too arbitrary.
                subs_at_root = foo[0] + '/Subs/'
                destination_subs = default_path_for_movies
                shutil.move(subs_at_root, destination_subs)
            except :
                pass
            try:
                files = glob.iglob(os.path.join(foo[0],"*.srt"))
                for file in files:
                    if os.path.isfile(file):
                        shutil.move(file, default_path_for_movies)
            except :
                pass
            shutil.move(path, destination)
            print 'file moved to dir ', destination
        '''

    elif mimetype == 'image':
        for i in range(0, len(metalist)):
            information = metalist[i].split(":")
            if (information[0] == '- Longitude'):
                longitude = information[1].strip()

            elif (information[0] == '- Latitude'):
                latitude = information[1].strip()
            elif (information[0] == '- Creation date'):
                date = information[1].strip()
                date = date.split('-')
                year = date[0]
                month = int(date[1])
                month = calendar.month_name[month]


        if (latitude != 0) and (longitude != 0):
            g = geocoder.google([latitude, longitude], method='reverse')
            city = g.city
            state = g.state
            country = g.country
            valueofsetx =  "/images/" + country+','+state+','+city+'/' + year +'/'+month +'/'
            default_path_for_images = rootdir + valueofsetx
            destination = default_path_for_images + fname
            if path != destination:
                cmd = default_path_for_images
                print cmd
                try:
                    os.makedirs(cmd, 0755)
                except OSError as e:
                    if e.errno == 17:
                        os.chmod(cmd, 0755)
                shutil.move(path, destination)
                print 'file moved to dir ', destination
        else :
            #global year
            valueofsetx = "/images/" + year +'/'+month +'/'
            default_path_for_images = rootdir + valueofsetx
            destination = default_path_for_images+fname
            if path != destination:
                cmd = default_path_for_images
                print cmd
                try:
                    os.makedirs(cmd,0755)
                except OSError as e:
                    if e.errno == 17:
                        os.chmod(cmd,0755)
                shutil.move(path, destination)
                print 'file moved to dir ', destination
    else:

        if mimetype in ('application/pdf', 'txt/x-c', 'application/vnd.oasis.opendocument.text'):
            valueofsetx = '/docs/'+mimetype+'/'+fname
            destination = rootdir+ valueofsetx
            if path != destination:
                cmd = rootdir + '/docs/'+mimetype+'/'
                try:
                    os.makedirs(cmd,0755)
                except OSError as e:
                    if e.errno == 17:
                        os.chmod(cmd,0755)
                shutil.move(path, destination)
                print 'file moved to dir ', destination
    try:
        xattr.setxattr(destination, 'user.fpath', valueofsetx)
        xattr.setxattr(destination, 'user.scfsC', 'Y')
    except IOError:
        pass

path = sys.argv[1]
xattr.setxattr(path, 'user.scfsC', 'N')
rootdir = sys.argv[2]
try:
    wasitclassified = sys.argv[3]
    if wasitclassified == 'YES': #

        fpath = xattr.getxattr(path, 'user.fpath')
        fpath = rootdir+fpath
        print fpath
        print 'moving directly to: ',fpath
        cmd = fpath.rsplit('/',1)
        fname = cmd[1]
        print cmd
        print fname
        cmd = cmd[0]+'/'
        print cmd
        try:
            os.makedirs(cmd, 0755)
        except OSError as e:
            if e.errno == 17:
                os.chmod(cmd, 0755)
        print fpath
        try:
            shutil.move(path, fpath)
        except:
            classify(path, rootdir)
        exit(1)
except IndexError:
    pass


classify(path, rootdir)
