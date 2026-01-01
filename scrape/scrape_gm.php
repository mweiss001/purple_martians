<?php

$srvrname = "localhost";
$database = "pm";
$username = "pmdb_rw";
$password = "readwrite";

$dt = new DateTime("now");
echo "\nSession log scrape: " . $dt->format('Ymd-His') . "\n";

// Create a connection
$conn = mysqli_connect($srvrname, $username, $password, $database);

// Check the connection
if (!$conn) die("Database connection failed: " . mysqli_connect_error());

function get_last_frame($filename)
{
  $fp = fopen($filename, 'r');
  if (!$fp) return 0;
  fseek($fp, -80, SEEK_END);
  $lines = array();  
  while (!feof($fp)) array_push($lines, fgets($fp));
  fclose($fp);
  return max($lines);
}


function add_to_db($filename)
{
   echo "\nAdd to db: filename: $filename - ";

   // first check if filename exists in db
   $sql = "SELECT id FROM gm WHERE filename='$filename'";
   global $conn;
   $result = mysqli_query($conn, $sql);

   if ($result->num_rows)
   {
      echo " already in db\n";   
      return;
   }
   else
   {   
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
      $dt_end   = $dt_end->  format('Y-m-d H:i:s');
      $dt_start = $dt_start->format('Y-m-d H:i:s');

      // echo "dts: $dt_start   dte:$dt_end";

      $sql = "INSERT INTO gm VALUES (NULL, '$filename', '$dt_start', '$dt_end', $sec )";
      echo "sql: $sql\n";
      if (mysqli_query($conn, $sql)) echo "New record created successfully\n";
      else echo "Error: " . $sql . "<br>" . mysqli_error($conn);
   }

}


// Open directory and iterate files
$dir = "/home/m/dev/purple_martians/savegame/";
if (is_dir($dir))
{
   if ($dh = opendir($dir))
   {
      while (($file = readdir($dh)) !== false)
      {
         if (str_ends_with($file, ".gm")) add_to_db($file);
         // else echo "file does not end in .gm\n";
      }
      closedir($dh);
   }
   else echo "error opening $dir\n";
}
else echo "$dir is not a directory\n";

?>