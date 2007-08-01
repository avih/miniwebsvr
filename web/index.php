<? include "lib.php"; startpage(); ?>
<h2>What is MiniWebsvr:</h2>
MiniWebsvr is a tiny/small HTTP web server that aims to one day be embeddable.<br>
Small footprint and light on resources is a major requirement.<br>
<br>

<hr  noshade="noshade" size="1">
<h2>Pie in the sky aim of the project:</h2>

The aim of this project is firstly:
<ul>
<li> To create a small fully functional web-server
</ul>

And secondly:
<ul>
<li> Create a embeddable web server library you can link into your application.
<li> Provide mechanisms to serve pages easily.
<li> The idea is <b>NOT</b> to have the applets plug-in to the server, but the server to plug-in to the base application.
</ul>

Part of this project is a set of small helper libraries that make using/managing the web server easier:
<ul>
<li> Templating system, for easy creation of non-structured data like HTML pages
<li> Small XML parser/generator to read/serve configuration or SOAP-like functionality
<li> Any other utility that would make it much easier to use. 
</ul>

<br>
<?endpage();?>
