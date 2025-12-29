# this updates the header in all html files in the current directory

# new text
# get the new text
open(MDATA, "<new_header.txt");
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
#  $a =~ s/<footer>(.*)<\/footer>/<footer>$b<\/footer>/s;

   $a =~ s/<html>(.*)<body>/<html>$b<body>/s;

 
   # save the changes to file
   open(DATA, ">", $file);
   print(DATA $a);
}



