<? include "lib.php"; startpage(); ?>
<h2>Features of MiniWebsvr:</h2>
Currently still a stand-alone server, that serves static pages only.<br>
A minimal set of HTTP/1.0 and HTTP/1.1 commands is supported.<br>
Supports:<br>
<ul>
<li> OPTIONS
<li> HEAD
<li> GET
<li> If-Modified-Since<sup>*</sup>
<li> Multi-threaded server<sub> (Optional)</sub>
<li> Microsoft<sup>(r)</sup> Windows<sup>(r)</sup>
<li> UNIX variants, such as Linux
<li> Resuming downloads<sup>*</sup>
<li> Thread pooling <sub>(PThreads implementation only)</sub>
<li> Logging <sub>(Optional)</sub>
<li> Some options to lock down the site a bit
<li> Configurable default page
</ul>
* Saves on bandwidth!<br>
<br>
<?endpage();?>
