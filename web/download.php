<? include "lib.php"; startpage(); ?>
<h2>Latest release:</h2>
The latest release is version 0.0.8, available for download:
<a href="http://sourceforge.net/project/showfiles.php?group_id=187000&package_id=218154&release_id=511741">MiniWebsvr version 0.0.8</a><br>

<br> 

<hr  noshade="noshade" size="1">
<h2>Getting the SVN version:</h2>
The SVN version is constantly in development, and MAY NOT WORK.<br>
To get access to the svn version point your favourite SVN client to:<br>
<pre>https://miniwebsvr.svn.sourceforge.net/svnroot/miniwebsvr/trunk</pre>
To compile the sources:<br>
Add all C files in the <code>src/</code> directory to your project, or:<br>

<br>
Using MinGW (Windows):
<pre>gcc -o miniwebsvr.exe src/*.c -Wall -lws2_32 -O2 -s</pre>
Using GCC:
<pre>gcc -o miniwebsvr src/*.c -Wall -O2 -s</pre>
<br>
To enable debug information just define <i>&quot;_DEBUG&quot;</i><br>
If you define <i>VERSION=&quot;someversion&quot;</i> it will override the version name/number<br>
<br>
A Makefile is also provided for UNIX/LINUX systems:
<pre>
Flags that affect compilation:
  CFLAGS        - Compiler optimization flags

Targets:'
  release       - Builds a release build of MiniWebSvr
  debug         - Builds a debug build of MiniWebSvr
  xwin32        - Builds a release build of MiniWebSvr for Windows using mingw32-gcc cross compiler
  xwin32d       - Builds a debug build of MiniWebSvr for Windows using mingw32-gcc cross compiler

Maintenance:
  clean         - Cleans up the files created by make
</pre>
<br>
<?endpage();?>
