<? include "lib.php"; startpage(); ?>
<h2>Security Advisory</h2>
This page lists the security issues that the author knows about, and their resolutions.<br>

<br>
<h3>Version 0.0.7:</h3>

<b>Description:</b><br>
Suffers from a directory traversal flaw, using a specially crafted URL one can list the parent folders<br>
<b>Risk:</b><br>
Critical<br>
<b>Credits to:</b><br>
shinnai x<br>
<a href="http://www.shinnai.altervista.org/viewtopic.php?id=41&t_id=47">http://www.shinnai.altervista.org/viewtopic.php?id=41&t_id=47</a><br>
<a href="http://www.milw0rm.com/exploits/3708">http://www.milw0rm.com/exploits/3708</a><br>
<b>Resolution:</b><br>
Fixed in 0.0.8<br>

<br>
<h3>Version 0.0.6 and older:</h3>

<b>Description:</b><br>
Suffers from a directory traversal flaw, using a specially crafted URL <code>"http://yoursite/..%00"</code>  one can list the parent folder<br>
<b>Risk:</b><br>
Medium (You can only list the folder contents one level up, not view the file contents)<br>
<b>Credits to:</b><br>
Daniel Nystr&ouml;m<br>
<b>Resolution:</b><br>
Fixed in 0.0.7<br>
<br>
<b>Description:</b><br>
Suffers from multiple 0-Day exploits, accessing almost any of the special MS-DOS device files (NUL fixed in 0.05) under windows can cause the CPU usage to rise to 100%<br>
<b>Risk:</b><br>
Less Critical (Denial of service)<br>
<b>Credits to:</b><br>
shinnai and rgod<br>
<b>Resolution:</b><br>
Fixed in o.o.7<br>

<br>

<h3>Version 0.04:</h3>
<b>Description:</b><br>
Multiple vulnerabilities have been identified, which could be exploited by remote attackers to take complete control of an affected system. These issues are due to various buffer overflow errors when processing specially crafted requests, which could be exploited by remote unauthenticated attackers to execute arbitrary commands with the privileges of the server.<br>
<b>Risk:</b><br>
Critical<br>
<b>Resolution:</b><br>
Upgrade to MiniWebsvr version 0.05<br>
<?endpage();?>
