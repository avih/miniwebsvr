#!/usr/bin/perl

use strict;
use warnings;
use IO::Socket;
use Time::HiRes qw(usleep);

my $host = shift || die "Usage: perl $0 host port test\n";
my $port = shift || 8080;

my $tests = 0;
my $failed = 0;

print STDERR "\nStarting tests:\n\n";

my $test = shift;
while ($test) {
	my $header = undef;
	my $response = undef;
	my $shouldbe = undef;

	print STDERR "Running test: $test ... ";
	++$tests;

	if (open REQ,"<  test/tests/$test.req")
	{
		read REQ,$header,100000;
		close REQ;
	}

	if (open RESP,"<  test/tests/$test.resp")
	{
		read RESP,$shouldbe,100000;
		close RESP;
	}

	if ($header and $shouldbe) {
		my $sock = new IO::Socket::INET(PeerAddr => $host, PeerPort => $port, PeerProto => 'tcp') or die "Error: $!\n";

		#print $header,"\n";
		$sock->send($header);
		while (<$sock>) {
			$response .= $_;
		}
		#print $response,"\n";
		$sock->close;


		# Mangle response so that date & version issues does not bother us:
		$response =~ s/\r\nLast-Modified:[^\r]*\r\n/\r\nLast-Modified:\r\n/;
		$response =~ s/MiniWebSvr\/[0-9\.a-zA-Z]*/MiniWebSvr\//g;
		$response =~ s/\r\nDate:[^\r]*\r\n/\r\nDate:\r\n/;
		$response =~ s/\n[^\n]*\.svn[^\n]*\n/\n/;

		if ($response ne $shouldbe) {
			print STDERR "failed - Result mismatch\n";
			++$failed;
			print "Sent: \n";
			print $header;
			print "Got:\n";
			print $response;
			print "\nShould be:\n$shouldbe\n";
		} else {
			print STDERR "succeed\n";
		}
	} else {
		print STDERR "failed - Test not complete\n";
		++$failed;
	}

	$test = shift;
}

print STDERR "\nRan $tests tests, where $failed failed\n\n";
