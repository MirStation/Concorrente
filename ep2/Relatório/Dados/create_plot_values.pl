#!/usr/bin/perl

open(DATA,"<output/output_5.csv") or die $!;
	
while(<DATA>){
   @line_values = split(/,|;/,$_);
   print @line_values[-3],"\n";
}
