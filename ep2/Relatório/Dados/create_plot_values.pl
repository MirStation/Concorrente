#!/usr/bin/perl

use List::Util qw(sum);

open(DATA,"<output/output_5.csv") or die $!;
open(OUTD,">output/data.csv") or die $!;
	
my $i = 0;
while(<DATA>){
	$i++;
	my @vs = split(/,|;/,$_);
	delete @vs[-1];
	delete @vs[-1];

	my $mean = sum(@vs)/@vs;

	my $sqsum = 0;
	for (@vs) {
		$sqsum += ( $_ ** 2 );
	} 
	$sqsum /= @vs;
	$sqsum -= ( $mean ** 2 );
	my $stdev = sqrt($sqsum);

	printf OUTD "%3d %.3f %.3f\n",$i,$mean,$stdev;	
}
