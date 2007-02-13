<? include "lib.php"; startpage(); ?>
<h2>Latest release:</h2>
The latest release is version 0.0.6, available for download:
<a href="http://sourceforge.net/project/showfiles.php?group_id=187000&package_id=218154&release_id=485186">MiniWebsvr version 0.0.6</a><br>

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
<pre>gcc -o miniwebsvr.exe src/*.c -Wall -lws2_32 -O2 -fomit-frame-pointer -march=i486 -mtune=i686 -s</pre>
Using GCC:
<pre>gcc -o miniwebsvr src/*.c -Wall -O2 -fomit-frame-pointer -march=i486 -mtune=i686 -s</pre>
<br>
To enable debug information just define &quot;_DEBUG&quot;<br>
<br>
<?endpage();?>
