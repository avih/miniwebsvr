<?
function startpage() {
	$ldate = gmdate("D, d M Y H:i:s",getlastmod())." GMT";
	$headers = apache_request_headers();
	header("Last-Modified: $ldate");
	if ($headers['If-Modified-Since'] == $ldate) {
		header("HTTP/1.1 304 Not Modified");
		exit();
	}
?><!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0//EN">
<html>
<head>
<title>MiniWebsvr</title>
<meta name="classification" content="Web Services">
<meta name="description" content="MiniWebSvr is a small webserver that aims to one day be embeddable.">
<meta name="keywords" content="webserver, small, development, http, programming, software, windows, linux, unix, embedded">
<meta name="distribution" content="Global">
<meta name="rating" content="General">
<style type="text/css"><!--
body { 
	font-family:Arial, Helvetica, sans-serif; 
	font-size: 10pt;
	margin: 0 auto;
	padding: 0px;
}
h1, h2, h3, b, a {
	color: #663300;
}

a:hover {
	color: #663300;
	text-decoration: none;
}

td.sidebar { 
	vertical-align: top;
	color: white;
	background-color: #663300;
	width: 125px;
}
a.sidebar:link { color: #afafaf; }
a.sidebar:visited { color: #afafaf; }
a.sidebar:hover { color: yellow; }
--></style>
</head>
<body>
<table cellspacing=0 cellpadding=8 width="100%">
<tr><td bgcolor="#EFEFEF">&nbsp;</td><td bgcolor="#EFEFEF">
	<img src="images/logo.png" width="295" height="63" alt="MiniWebsvr">
</td></tr>
<tr><td class=sidebar>
	Sitemap:<br>
	<hr noshade="noshade" size="1" color="white">
	<a class=sidebar href="/">Home</a><br>
	<a class=sidebar href="/features.php">Features</a><br>
	<a class=sidebar href="/download.php">Download</a><br>
	<a class=sidebar href="/roadmap.php">Roadmap</a><br>
	<a class=sidebar href="http://sourceforge.net/projects/miniwebsvr/">SF.net page</a><br>
	<br>
	Hosted by<br>SourceForge:<br>
	<a href="http://sourceforge.net"><img src="http://sflogo.sourceforge.net/sflogo.php?group_id=187000&amp;type=2" width="125" height="37" border="0" alt="SourceForge.net Logo" /></a><br>
	<br>
	Affiliations:<br>	
	<hr noshade="noshade" size="1" color="white">
	<a href="http://www.sagamedev.com/"><img src="images/sagd-btn.gif" border="0" height="31" width="88"></a><br>
	<a href="http://www.linuxlinks.com"><img src="images/linuxlinks.gif" alt="Linux Links" border="0" height="24" width="86"></a><br>
</td><td style="vertical-align: top;">
<?}
function endpage() { ?>
</td>
</table>

</body>
</html>
<?}?>
