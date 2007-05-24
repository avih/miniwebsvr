Welcome to the MiniWebsvr project
---------------------------------

MiniWebsvr is a small web server that aims to be able to be used stand-alone
or embedded into another application.
Small footprint and light on resources is a requirement.

To Use the server:
Usage: miniwebsvr.exe [options]
Options:
  --help                   Display this information
  --port <port>            Listen on port <port> (default 8080)
  --log <file>             Save the log file as <file> (default: miniwebsvr.log)
  --root <path>            Specify the document root directory (default: .)
  --interface <ip>         Specify the interface the server listens on (default: ALL)
  --threads <thread_nos>   Specify number of threads in thread pool (default 8)



The web-server supports the following:
- Serves static web pages
- Directory listing
- Supports HTTP MIME encodings
- Supports GET/HEAD/OPTIONS
- Logging to disk
- Small memory footprint
- Muti-threaded
- Written in plain C
- Win32 and UNIX
- Command line options
- Conditional GET:
  - If-Modified-Since:
- Range-bound downloads (Download resuming)
- Thread pooling (PThreads only)


Was originally designed as a small stand-alone web-server, but I think I'll
change the scope to a small embedded web serbver, so maybe serve pages to
browsers to support an game, maybe provide live stats of the game world
(would be usefull for an online game) or used for downloading in-game content
to a client. So eventually it will be an embedded HTTP server.


MiniWebsvr is mostly developed by Nickolas Antonie Grigoriadis <grigi_ at users.sourceforge.net> with contributions from:
- Daniel John Walker <djwalker at users.sourceforge.net>
- Johan Strydom <fyrewolfe at users.sourceforge.net>
- Sergey Sytchewoj <brumal at users.sourceforge.net>
