# shows all tags of a specfic type by filename


$tag = $ARGV[0];




# iterate all .html files
@files = <*.html>;
foreach $file (@files)
{
#   print "\n-----------------\n";
#   print $file . "\n";
#   print "-----------------\n";

   # open the file
   open(DATA, "<", $file);
   my $a = do {local $/; <DATA>};


   my @matches;
   push @matches, $1 while $a =~ /.*(<$tag>.*<\/$tag>)/g; 
   
   
   foreach (@matches)
   {
      #print $_ . "\n";

	  $_ =~  /<mh7>(.*)<\/mh7>/; 
   
      $_ = $1;
   
      print "$file: $_ \n";
   }

   
   
}



