<? include "lib.php"; startpage(); ?>
<h2>What is MiniWebsvr:</h2>
MiniWebsvr is a small web server that aims to one day be embeddable.<br>
Small footprint and light on resources is a major requirement.<br>
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
<li> HOOK to define fixed-content, and maybe even override disk IO with some VFS (so you can serve static content out of your own bundled resource file or a DB, etc...)
<li> HOOK to override a whole directory, and that returns the sub-path and paremeters to a func
</ul>

<li> This library provides you with a parse/generate XML functionality so that you can easily pass data up and down (kinda like SOAP, but I prefer simplicity) 
<li> User space (Authenticated), so that you can use it for reliable data transfer
</ul>
<br>
<?endpage();?>
