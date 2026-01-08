<?php

//$srvrname = "localhost";
//$database = "pm";
//$username = "pmdb_rw";
//$password = "readwrite";

// Create a connection
//$conn = mysqli_connect($srvrname, $username, $password, $database);

// Check the connection
//if (!$conn) die("Database connection failed: " . mysqli_connect_error());

// database setup
$db_filepath = "/home/m/dev/purple_martians/data/database.db";
if (!file_exists($db_filepath))
{
   echo "Database file: $filename not found.";
   return;
}
$db = new PDO("sqlite:/home/m/dev/purple_martians/data/database.db");



$dt = new DateTime("now");
echo "\nSession log scrape: " . $dt->format('Ymd-His') . "\n";




// this function used to just get the last handfull of ints and find the largest
// now that I am embedding player name into moves, this no longer works
// there are some huge numbers that sometimes are falsely detected as frames
// my new method is to grab groups of 4 ints from the end
// the first int in one of those groups should be the frame number
// this seems to work well so far...
// seek to end of file -50
// read lines as ints and put in array
// get array count, then last index is count - 1
// last index -4 should be the largest frame
  //$max = $lines[count($lines)-5];
  //echo "last frame num: $max\n";
//  $cnt = count($lines);
//  $last = $cnt -1;
//  echo "count: $cnt\n";
//  echo "last: $lines[$last]\n";
//  $k = $last -12;
//  for ($j=$k; $j<$k+12; $j+=4)
//  {
//     for ($i=$j; $i<$j+4; $i++)
//        echo "[$lines[$i]]";
//     echo "\n";
//  }


function get_last_frame($filename)
{
  // echo "$filename  -  get last frame num\n";
  $fp = fopen($filename, 'r');
  if (!$fp) return 0;
  fseek($fp, -80, SEEK_END);
  $lines = array();  
  while (!feof($fp)) array_push($lines, (int)fgets($fp));
  fclose($fp);
  return $lines[count($lines)-5];
}


function add_to_db($filename)
{
   echo "\nAdd to db: filename: $filename - ";

   // first check if filename exists in db
   $sql = "SELECT COUNT(*) FROM gm WHERE filename='$filename'";
   $res = $GLOBALS['db']->query($sql);
   if ($res->fetchColumn())  { echo " session [$filename already in db\n";  return; } 



/*

   $res = $GLOBALS['db']->query($sql);
   while ($row = $res->fetch(PDO::FETCH_ASSOC))

old: this does not work
if ($result->num_rows == 0) { echo " gm not found \n";  return; }










   global $conn;
   $result = mysqli_query($conn, $sql);

   if ($result->num_rows)
   {
      echo " already in db\n";   
      return;
   }
   else
   {   
*/    

      echo " not found in db, inserting\n";

      // the filename should start with this timestamp '20251229-151621'

      $dateString = substr($filename, 0, 15); 
      //echo "dateString: $dateString\n";

      // test if we can create a valid DateTime
      if (!DateTime::createFromFormat('Ymd-His', $dateString))
      {
         echo "Error parsing date: $dateString\n";
         return;
      }
      $dt_end = DateTimeImmutable::createFromFormat('Ymd-His', $dateString);

      // open the file and get the last frame number
      $sec = (int) (get_last_frame("/home/m/dev/purple_martians/savegame/$filename") / 40);

      // create start as an offset from end
      $dt_start = $dt_end->sub(new DateInterval('PT' . $sec . 'S'));

      // convert the datetimes to something mysql likes
//      $dt_end   = $dt_end->  format('Y-m-d H:i:s');
  //    $dt_start = $dt_start->format('Y-m-d H:i:s');


      // convert the datetimes to something sqlite likes
      $dt_end   = $dt_end->   format('Ymd-His');
      $dt_start = $dt_start-> format('Ymd-His');



      // echo "dts: $dt_start   dte:$dt_end";

      $sql = "INSERT INTO gm VALUES (NULL, '$filename', '$dt_start', '$dt_end', $sec )";
      echo "sql: $sql\n";
      $res = $GLOBALS['db']->query($sql);


//      if (mysqli_query($conn, $sql)) echo "New record created successfully\n";
//      else echo "Error: " . $sql . "<br>" . mysqli_error($conn);



  // }

}


// Open directory and iterate files
$dir = "/home/m/dev/purple_martians/savegame";

if (is_dir($dir))
{
   if ($dh = opendir($dir))
   {
      while (($file = readdir($dh)) !== false)
         if (str_ends_with($file, ".gm")) add_to_db($file);
      closedir($dh);
   }
   else echo "error opening $dir\n";
}
else echo "$dir is not a directory\n";

?>