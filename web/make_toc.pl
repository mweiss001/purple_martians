#!/usr/bin/perl

# makes a toc using <mh7> tags 
# inserts the toc between <mdw_file_toc> tags

# iterate all .html files
@files = <*.html>;
foreach $file (@files)
{
   print "\nfile:$file\n";
   print "---------------------------------\n";
   
   # open the file
   open(DATA, "<", $file);
   my $a = do {local $/; <DATA>};

   # find <mh7> tags
   my @matches;
   push @matches, $1 while $a =~ /.*(<mh7>.*<\/mh7>)/g; 
   
   # multi line toc we are going to insert
   my @toc; 
	
   foreach (@matches)
   {
      #print $_ . "\n";

	  # strip tags
	  $_ =~  /<mh7>(.*)<\/mh7>/; 
      $_ = $1;
      print $_ . "\n";

      # add single line in toc
      $toci = "<a href=\"$file\#$_\" target=\"_top\">$_</a>\n";
	  push @toc, $toci;
   }
   #print @toc;

   # replace the toc between <mdw_file_toc> tags
   $a =~ s/<mdw_file_toc>(.*)<\/mdw_file_toc>/<mdw_file_toc> @toc<\/mdw_file_toc>/s;
 
   # save the changes to file
   open(DATA, ">", $file);
   print(DATA $a);
}



