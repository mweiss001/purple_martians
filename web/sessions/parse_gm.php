<?php

$srvrname = "localhost";
$database = "pm";
$username = "pmdb_rw";
$password = "readwrite";

$dt = new DateTime("now");
echo "\nParse gm : " . $dt->format('Ymd-His') . "\n";

// Create a connection
$conn = mysqli_connect($srvrname, $username, $password, $database);

// Check the connection
if (!$conn) die("Database connection failed: " . mysqli_connect_error());



function gma_to_val($t, $d1, $d2)
{
   echo "gma_to_val($t $d1 $d2)  --  ";

   // get the player number and color from the bitfield
   $p =  $t & 0b0000000000000111;        // mask all except lowest 3 bits
   $c = ($t & 0b0000111100000000) / 256; // mask and shift 8

   // get the player name from the next 2 32bit ints
   
   // make sure this is declared as a string
   $n = "";
   
   // detect null and force all char after to also be null
   $found_null = 0;   

   // first 4 char from d1;
   for ($i=0; $i<4; $i++)
   {
      // shift byte into place and mask
      $ch = chr( ($d1 >> ($i*8)) & 0xFF ); 

      // detect null
      if (ord($ch) == 0) $found_null = 1;

      // force to null
      if ($found_null) $ch = chr(0);

      // save in array
      $n[$i] = $ch;
   }


   // second 4 char from d2;
   for ($i=0; $i<4; $i++)
   {
      // shift byte into place and mask
      $ch = chr( ($d2 >> ($i*8)) & 0xFF ); 

      // detect null
      if (ord($ch) == 0) $found_null = 1;

      // force to null
      if ($found_null) $ch = chr(0);

      // save in array
      $n[$i+4] = $ch;
   }

   // make sure 9th char is zero
   $n[8] = chr(0); 

   // show values
   //for ($i=0; $i<9; $i++)
   //   echo sprintf("[%d] - '%d' '%s' \n", $i, ord($n[$i]), $n[$i]);


   echo "player: $p color:$c name:$n\n";

}





function parse_gm($filename)
{
   echo "\nParse gm files: $filename - ";

   // open file and read all lines into array
   $fp = fopen($filename, 'r');
   if (!$fp) return 0;
   $lines = array();  
   while (!feof($fp)) array_push($lines, (int)fgets($fp));
   fclose($fp);



   $cnt = count($lines);
   echo "\nlines: : $cnt\n";

   for ($i=0; $i<$cnt; $i++)
   {
      if ($lines[$i] & 0b00000000100000000000000000000000)
      {
         echo "found player match at frame: $i\n";
         gma_to_val($lines[$i], $lines[$i+1], $lines[$i+2]);
      }
   }




}


// Open directory and iterate files
$dir = "/home/m/dev/purple_martians/savegame";

if (is_dir($dir))
{





   if ($dh = opendir($dir))
   {
      while (($file = readdir($dh)) !== false)
         if (str_ends_with($file, ".gm")) parse_gm("/home/m/dev/purple_martians/savegame/$file");
      closedir($dh);
   }
   else echo "error opening $dir\n";
}
else echo "$dir is not a directory\n";




?>