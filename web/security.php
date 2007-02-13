<? nclude "lib.php"; startpage(); ?>
<h2>Security Advisory</h2>
This page lists the security issues that the author knows about, and their resolutions.<br>
<br>
<h3>Version 0.0.6 and older:</h3>

<b>Description:</b><br>
Suffers from a directory traversal flaw, using a specially crafted url <code>"http://yoursite/..%00"</code>  one can list the parent folder<br>
<b>Risk:</b><br>
Medium (You can only list the folder contents one level up, not view the file contents)<br>
<b>Credits to:</b><br>
Daniel Nyström<br>
<b>Resolution:</b><br>

Fix submitted to SVN repository, will be fixed in next version.<br>

<br>

<h3>Version 0.04:</h3>
<b>Description:</b><br>
Multiple vulnerabilities have been identified, which could be exploited by remote attackers to take complete control of an affected system. These issues are due to various buffer overflow errors when processing specially crafted requests, which could be exploited by remote unauthenticated attackers to execute arbitrary commands with the privileges of the server.<br>
<b>Risk:</b><br>
Critical<br>
<b>Resolution:</b><br>
Upgrade to MiniWebsvr version 0.05<br>
<?endpage();?>
