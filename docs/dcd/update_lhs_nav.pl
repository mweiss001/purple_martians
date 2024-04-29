# this updates the left hand side navigation panel in all html files in the current directory

# get the new text
open(MDATA, "<new_lhs_nav.txt");
my $b = do {local $/; <MDATA>};

# iterate all .html files
@files = <*.html>;
foreach $file (@files)
{
   # print $file . "\n";

   # open the file to replace text in
   open(DATA, "<", $file);
   my $a = do {local $/; <DATA>};

   # replace the text
   $a =~ s/<mdw_lhs_nav>(.*)<\/mdw_lhs_nav>/<mdw_lhs_nav>$b<\/mdw_lhs_nav>/s;
 
   # save the changes to file
   open(DATA, ">", $file);
   print(DATA $a);
}



