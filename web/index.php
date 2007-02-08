<?$ldate = gmdate("D, d M Y H:i:s",getlastmod())." GMT";
$headers = apache_request_headers();
if ($headers['If-Modified-Since'] == $ldate) header("HTTP/1.1 304 Not Modified");
header("Last-Modified: $ldate");
?><!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2//EN">
<html>
<head>
<title>MiniWebsvr</title>
<meta name="classification" content="Web Services">
<meta name="description" content="MiniWebSvr is a small webserver that aims to one day be embeddable.">
<meta name="keywords" content="webserver, small, development, http, programming, software, windows, linux, unix, embedded">
<meta name="distribution" content="Global">
<meta name="rating" content="General">
<style type="text/css">
<!--
body
{
	font-family:Arial, Helvetica, sans-serif;
}

h1, h2, h3, b, hr, a
{
	color: #663300;
}

a:hover
{
	color: #663300;
	text-decoration: none;
}

-->
</style>
</head>
<body>
<img src="images/logo.png" width="295" height="63" alt="MiniWebsvr">
<hr  noshade="noshade" size="1">
Welcome to the MiniWebsvr home page.<br>
SourceFORGE.net project page:
<a href="http://sourceforge.net/projects/miniwebsvr/">http://sourceforge.net/projects/miniwebsvr/</a>
<br>

<hr  noshade="noshade" size="1">
<h2>Pie in the sky aim of the project:</h2>
<ul>
<li> Create a small ibrary you link into your app
<li> The configuration can be pushed from the app or have its own handlers to read conf files.
<li> This library provides you with HOOKS, into the server, so you can write your own functions that will override some functionality, e.g.
<ul>
<li> HOOK to override the default directory listing func
<li> HOOK to call a func with given parameters when a certain resource (file) is called
<li> HOOK to define fixed-content, and baybe even override disk IO with some VFS (so you can serve static content out of your own bundled resource file or a DB, etc...)
<li> HOOK to override a whole directory, and that returns the sub-path and paremeters to a func
</ul>
<li> This library provides you with a parse/generate XML functionality so that you can easily pass data up and down (kinda like SOAP, but I prefer simplicity) 
<li> User space (Authenticated), so that you can use it for reliable data transfer
</ul>
<br>

<hr  noshade="noshade" size="1">
<h2>Actual status of MiniWebsvr:</h2>
Currently still a stand-alone server, that serves static pages only.<br>
Supports:<br>
<ul>
<li> OPTIONS
<li> HEAD
<li> GET
<li> If-Modified-Since<sup>*</sup>
<li> Optionally threaded server
<li> Microsoft(r) Windows(r)
<li> UNIX variants, such as Linux<sup>**</sup>
<li> Resuming downloads<sup>* **</sup>
</ul>
* Saves on bandwidth!<br>
** In SVN repository<br>
<br>

<hr  noshade="noshade" size="1">
<h2>Latest release:</h2>
Version 0.05, available for download:
<a href="http://sourceforge.net/project/showfiles.php?group_id=187000">MiniWebsvr version 0.05</a><br>
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

<hr  noshade="noshade" size="1">
<h2>Roadmap:</h2>
This is a Roadmap meant to communicate with people the status of MiniWebsvr.<br>
<br>
Key:
<ul>
<li>Completed items
<li type=circle>Items not done yet
<li type=square>Partial implementation
</ul>
<br>
<b>0.04 Released (Jan 1 2007) &rarr; First LGPL release</b><br>

<ul>
<li>Add support for HEAD
<li>Add support for OPTIONS
<li>Harden server by preventing buffer-overflow vulnerabilities
<li>Add support for Conditional GET's (If-Modified-Since)
<li>Add support for command-line options
</ul>

<b>0.05 Released (Jan 18 2007)</b><br>

<ul>
<li>Port to UNIX sockets
<li>Add support for range-bound downloads (Support Resume) <sub>(HTTP/1.0 style &quot;Range:&quot; supported)</sub>
<li type=square>Implement fixed-size thread pool
</ul>

<b>Requirements for 0.0.6 met (Alpha)</b><br>

<ul>
<li type=circle>Implement self-adjusting size thread pool
<li type=circle>Add support for Pipelining (In a safe, compliant way)
<li type=circle>Add vhost support (For HTTP/1.0 and HTTP/1.1)
</ul>

<b>Requirements for 0.1.0 met (Beta)</b><br>
Version 0.1.0 should have a featureset to be a complete static content server<br>

<ul>
<li type=circle>Port to Library
<li type=circle>Provide sample application to replace stand-alone server
<li type=circle>Implement XML Parser/Generator
  <ul>
  <li type=circle>Implement Config parser using XML Parser
    <ul>
    <li type=circle>Add vhost support
    </ul>
  <li type=circle>Implement Directory Listing using XML Generator
  <li type=circle>Re-implement strign handling to suit
  </ul>
<li type=circle> Add preliminary HOOK support 
</ul>

<b>Requirements for 0.2.0 met (Stable)</b><br>

<br>
At this stage we plan to do the next round of unstable code changes.<br>
<br>
Features that we intend to implement, but have not given it much thought (as it is quite a bit into the future):<br>

<ul>
<li type=circle>The configuration can be pushed from the app or have its own handlers to read conf files.
<li type=circle>This library provides you with HOOKS, into the server, so you can write your own functions that will override some functionality
  <ul>
  <li type=circle>HOOK to override the default directory listing func
  <li type=circle>HOOK to call a func with given parameters when a certain resource (file) is called
  <li type=circle>HOOK to define fixed-content, and baybe even override disk IO with some VFS (so you can serve static content out of your own bundled resource file or a DB, etc...)
  <li type=circle>HOOK to override a whole directory, and that returns the sub-path and paremeters to a func
  </ul>
<li type=circle>POST support
<li type=circle>PUT support
<li type=circle>DIGEST authentication support
<li type=circle>Users
<li type=circle>Content-Encoding: gzip
</ul>
<br>

<hr  noshade="noshade" size="1">
<a href="http://sourceforge.net"><img src="http://sflogo.sourceforge.net/sflogo.php?group_id=187000&amp;type=5" width="210" height="62" border="0" alt="SourceForge.net Logo" /></a>
</body>
</html>
