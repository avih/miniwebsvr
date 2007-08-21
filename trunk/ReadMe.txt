Welcome to the MiniWebSvr project
---------------------------------

MiniWebSvr is a small web server that aims to be able to be used stand-alone
or embedded into another application.
Small footprint and light on resources is a requirement.

To Use the server:
Usage: miniwebsvr.exe [options]
Options:
  --help                   Display this information
  --port <port>            Listen on port <port> (default 8080)
  --interface <ip>         Specify the interface the server listens on (default: ALL)
  --log <file>             Save the log file as <file> (default: miniwebsvr.log)
  --nolog                  Disables logging, overrides any '--log' setting
  --root <path>            Specify the document root directory (default: .)
  --default <filename>     Specify the default document filename in a directory (default: index.html)
  --nodirlist              Do not do any directory listings, just return a '404 File not found'
  --noname                 Do not specify server name in directory listings or HTTP headers
UNIX only:
  --threads <thread_nos>   Specify number of threads in thread pool (default 8)



The web-server supports the following:
- Serves static web pages
- Directory listing
- Supports HTTP MIME encodings
- Supports GET/HEAD/OPTIONS
- Logging to disk
- Small memory footprint
- Multi-threaded
- Written in plain C
- Win32 and UNIX
- Command line options
- Conditional GET:
  - If-Modified-Since:
- Range-bound downloads (Download resuming)
- Thread pooling (UNIX PThreads only)



Was originally designed as a small stand-alone web-server, but I think I'll
change the scope to a small embedded web server, so maybe serve pages to
browsers to support an game, maybe provide live stats of the game world
(would be useful for an online game) or used for downloading in-game content
to a client. So eventually it will be an embedded HTTP server.


MiniWebSvr is mostly developed by Nickolas Antonie Grigoriadis <grigi_ at users.sourceforge.net> with contributions from:
- Daniel John Walker <djwalker at users.sourceforge.net>
- Johan Strydom <fyrewolfe at users.sourceforge.net>
- Sergey Sytchewoj <brumal at users.sourceforge.net>


--- Release Notes ---

Version 0.0.9: ()
New Features:
- Command-line options checking
  MiniWebSvr will now complain if an incorrect parameter is specified
  - Cannot write to specified logfile
  - Cannot open 'root' directory
  - Port value out-of-bounds
  - Limits the no of threads in thread-pool to a sane amount
- Support for FreeBSD
  Please use gmake with gcc on this platform.
- Supports accessing large files (>2GiB)
- Added beginnings of a testing suite for regression testing
- Changed Win32 build to have a console & gui client.
  The gui client will not open any window, unless it needs to communicate with
  the user
Bug Fixes:
- Fixes a security issue where another application can hijack the listening
  port of the server
- Fixed issue where miniwebserver lists dir or returns default page when
  trailing slash is missing.

Version 0.0.8: (2007-05-28)
New Features:
- Extra command-line options to:
  - Set default page name
  - Disable directory listing
  - Disable reporting of server name & version
  - Disable logging
- Will list the command-line help when it fails to parse the command line
Bug Fixes:
- Fixes sub-root vulnerability introduced in 0.0.7
- Fixes default page not always working
- Does not show '../' on root directory listing anymore
- Fixed name/version reporting in generated errors
- Fixed a bug in reporting the data range when whole file requested

Version 0.0.7: (2007-03-10)
This version brings with it mostly security fixes.

Version 0.0.6: (2007-02-09)
For more platforms:
- Support for UNIX sockets and pthreads support.
- Support for download resuming.
- Several bug fixes.

Version 0.05: (2007-01-18)
This version supports:
- OPTIONS
- HEAD
- GET
  - Conditional GET (If-Modified-Since)
Also a considerable amount of bug fixes and possible buffer-overflow fixes has been made.

Version 0.0.4: (2007-01-01)
First LGPL release
