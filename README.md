# spotifyd(8) - A spotify daemon
A **Spotify** daemon in the spirit of MPD, **Music Player Daemon**.

## Features
Right now, spotifyd can search, add to a queue, play the queue
(randomly or in order), add playlists, remove playlists, edit playlists.
For my use it's pretty much feature complete but if you feel something is missing,
please open an issue (or better yet, send a pull request with a patch for the new feature!).

## Build
Building is done with a single command:
```
make
```
As this software is not done yet, there's no install target. Do it
manually if you want to install it globally.

## Installation
Installation is done with a simple `make install`. If you're running Arch Linux,
there's a package on [AUR](https://aur.archlinux.org/packages/spotifyd/) as well.

### Dependencies
libspotify and alsa (as far as I know!).

## Connection
For now spotifyd only accepts conenctions on a socket, or a pair of sockets,
specified in `~/.spotifyd.rc`. The example config listens uses `/tmp/spotifyd`
as a unix socket and `127.0.0.1:12345` as an network socket. For
testing purposes, one can connect to spotifyd with socat:
```
socat UNIX:/tmp/spotifyd -
```
or with telnet:
```
telnet 127.0.0.1 12345
```

## Commands
The following commands are implemented in spotifyd. The same
list can be generated by sending the command "HELP" to the sever:
* SEARCH str  - Searches spotify for str.
* CUR_PLAYING - Returns the currently playing song.
* QLIST       - List content of the queue.
* SLIST       - List search results.
* QRAND       - Toggle queue randomness on/off.
* QADD n      - Add song n from search results to queue.
* QCLEAR      - Clear the queue.
* QRM n       - Remove track n from queue.
* PLAY n      - Play song n in queue.
* PREV        - Play previous song.
* NEXT        - Play next song.
* PAUSE       - Toggle play/pause.
* PL          - List available playlists.
* PLCREATE s  - Create new playlist with name s.
* PLDELETE n  - Delete playlist n.
* SADDPL n    - Put playlist n in search list.
* QADDPL n    - Put playlist n in queue.
* PLADD n p   - Add track n from queue to playlist p. 
* PLRM n p    - Remove track n from playlist p.

## Usage
I create a bash function so that I don't have to type the socat command
all the time:
```
sc () {
	echo $@ | socat - UNIX-CONNECT:/tmp/spotifyd 2>/dev/null
}
```

I the use it from the shell like this:
```
~/spotifyd $ sc search art blakey | head -n4 # search for art blakey, show the top 4 hits.
0 | Moanin' | Art Blakey & The Jazz Messengers
1 | Moanin' | Art Blakey
2 | Along Came Betty | Art Blakey
3 | Along Came Betty | Art Blakey
~/spotifyd $ sc qadd 0 # add search hit 0 to queue
Adding: Moanin' | Art Blakey & The Jazz Messengers
~/spotifyd $ sc play 0 # play song 0 on queue
Playing: Moanin' | Art Blakey & The Jazz Messengers
~/spotifyd $
```
The queue now contains one song, wich can be shown by:
```
~/spotifyd $ sc qlist
0 | Moanin' | Art Blakey & The Jazz Messengers
~/spotifyd $
```
If I want to add many songs, I just use my bash skills:
```
~/spotifyd $ sc search rolling stones | head
0 | Gimme Shelter | The Rolling Stones
1 | Paint It Black | The Rolling Stones
2 | (I Can't Get No) Satisfaction - (Original Single Mono Version) | The Rolling Stones
3 | Start Me Up - Remastered | The Rolling Stones
4 | Sympathy For The Devil | The Rolling Stones
5 | Angie - Remastered | The Rolling Stones
6 | Sympathy For The Devil | The Rolling Stones
7 | Rolling Stones T-Shirt - Radio Edit | Dada Life
8 | (I Can't Get No) Satisfaction - Mono Version | The Rolling Stones
9 | Brown Sugar - 2009 Re-Mastered Digital Version | The Rolling Stones
~/spotifyd $ for i in `seq 0 9`; do sc qadd $i; done
Adding: Gimme Shelter | The Rolling Stones
Adding: Paint It Black | The Rolling Stones
Adding: (I Can't Get No) Satisfaction - (Original Single Mono Version) | The Rolling Stones
Adding: Start Me Up - Remastered | The Rolling Stones
Adding: Sympathy For The Devil | The Rolling Stones
Adding: Angie - Remastered | The Rolling Stones
Adding: Sympathy For The Devil | The Rolling Stones
Adding: Rolling Stones T-Shirt - Radio Edit | Dada Life
Adding: (I Can't Get No) Satisfaction - Mono Version | The Rolling Stones
Adding: Brown Sugar - 2009 Re-Mastered Digital Version | The Rolling Stones
~/spotifyd $ sc qlist
0 | Gimme Shelter | The Rolling Stones
1 | Paint It Black | The Rolling Stones
2 | (I Can't Get No) Satisfaction - (Original Single Mono Version) | The Rolling Stones
3 | Start Me Up - Remastered | The Rolling Stones
4 | Sympathy For The Devil | The Rolling Stones
5 | Angie - Remastered | The Rolling Stones
6 | Sympathy For The Devil | The Rolling Stones
7 | Rolling Stones T-Shirt - Radio Edit | Dada Life
8 | (I Can't Get No) Satisfaction - Mono Version | The Rolling Stones
9 | Brown Sugar - 2009 Re-Mastered Digital Version | The Rolling Stones
~/spotifyd $
```
That didn't require so much skills. But lets say that
I don't want those two duplicate songs?
```
~/spotifyd $ for i in $(sc search rolling stones | head | sort -t'|' -k2,2 | uniq -f1 -w10 | sort | cut -d'|' -f1); do sc qadd $i; done
Adding: Gimme Shelter | The Rolling Stones
Adding: Paint It Black | The Rolling Stones
Adding: Start Me Up - Remastered | The Rolling Stones
Adding: Sympathy For The Devil | The Rolling Stones
Adding: Angie - Remastered | The Rolling Stones
Adding: Rolling Stones T-Shirt - Radio Edit | Dada Life
Adding: (I Can't Get No) Satisfaction - Mono Version | The Rolling Stones
Adding: Brown Sugar - 2009 Re-Mastered Digital Version | The Rolling Stones
~/spotifyd $ sc qlist
0 | Gimme Shelter | The Rolling Stones
1 | Paint It Black | The Rolling Stones
2 | Start Me Up - Remastered | The Rolling Stones
3 | Sympathy For The Devil | The Rolling Stones
4 | Angie - Remastered | The Rolling Stones
5 | Rolling Stones T-Shirt - Radio Edit | Dada Life
6 | (I Can't Get No) Satisfaction - Mono Version | The Rolling Stones
7 | Brown Sugar - 2009 Re-Mastered Digital Version | The Rolling Stones
~/spotifyd $
```
Okay, so no one would do that in practice. The point is just that
the standard shell along with the unix tools are really powerful
and allows you to do manage the queue efficiently.

## Configuration
Configuration is done by placing a config file called `.spotifyd.rc` in the `$HOME` dir of whoever
runs spotifyd. An example is provided. This file contains the port (or path if unix socket) that
spotifyd should listen on and, optionally, the username and password to sign in with.

## There's a weird binary among the source files, why!?
Every application using libspotify requires an appkey.
However, spotify don't allow you to share it openly in plain text,
only compiled as a part of your program. As I want anyone to be
able to build this program, I supply appkey.key, a binary version
of the appkey. You can feel free to inspect this binary, or request
your own appkey from spotify if you don't trust me.

## License
The project is licensed under GPLv3 with the exception of the files audio.h,
audio.c and alsa-audio.c that are from the libspotify examples. The function
`on_music_delivered` in callbacks.c is also from the libspotify documentation.

## Contributions
Found a bug or want to add a new feature? Great! Send a pull request and I will take a look.
Let's make this the greatest Spotify client out there!
