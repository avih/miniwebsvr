<? include "lib.php"; startpage(); ?>
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
<li>Add support for range-bound downloads (Support Resume)
</ul>

<b>0.0.6 Released (Feb 9 2007)</b><br>

<ul>
<li>Implement fixed-size thread pool<sub>(PThreads implementation only)</sub>
</ul>

<b>0.0.7 Released (Mar 10 2007)</b><br>

<ul>
<li>Implement custom default index
<li>Optional disabling of directory listing,reporting of name and version &amp; logging
</ul>

<b>0.0.8 Released (May 28 2007)</b><br>

<ul>
<li>Error checking with errors and/or warnings for command-line options
<li>Under Win32 architectures you could hijack the port the server listens on - Fixed
<li>Short versions of command-line parameter options


<b>0.0.9</b><br>

<ul>
<li type=circle>Config system (Optional config file)
<li type=circle>Implement XML parser for Config system
<li type=circle>Running as service for Win32 version
<li type=circle>Configurator GUI for Win32 version
<li type=circle>Installer for Win32 version
<li type=circle>Add vhost support (For HTTP/1.0 and HTTP/1.1)
</ul>

<b>Requirements for 0.1.0 met (Beta)</b><br>
Version 0.1.0 should have a feature set to be a complete static content server<br>

<ul>
<li type=circle>Implement self-adjusting size thread pool
<li type=circle>Add support for Pipelining (In a safe, compliant way)
<li type=square>Port to Library
<li type=circle>Provide sample application to replace stand-alone server
<li type=circle>Implement XML Generator
  <ul>
  <li type=circle>Implement Directory Listing using XML Generator
  <li type=circle>Re-implement string handling to suit
  </ul>
<li type=circle> Add preliminary HOOK support <sub>Need to revise if this is wise, a VFS might work much more elegantly</sub> 
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
  <li type=circle>HOOK to define fixed-content, and maybe even override disk IO with some VFS (so you can serve static content out of your own bundled resource file or a DB, etc...)
  <li type=circle>HOOK to override a whole directory, and that returns the sub-path and parameters to a func
  </ul>
<li type=circle>POST support
<li type=circle>PUT support
<li type=circle>DIGEST authentication support

<li type=circle>Users
<li type=circle>Content-Encoding: gzip
</ul>
<br>
<?endpage();?>
