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


function add_to_db($filename)
{
   echo "\nAdd to db: filename: $filename\n";

// 20251229-151621-[32]

   $dateString = substr($filename, 0, 15); 
   echo "dateString: $dateString\n";

   // test if we can create a valid DateTime
   if (!DateTime::createFromFormat('Ymd-His', $dateString))
   {
      echo "Error parsing date: $dateString\n";
      return;
   }

   // convert the datetime to something mysql likes
   $date = DateTime::createFromFormat('Ymd-His', $dateString);
   $date = $date->format('Y-m-d H:i:s');

   // first check if filename is exists in db
   $sql = "SELECT id FROM gm WHERE filename='$filename'";
   global $conn;
   $result = mysqli_query($conn, $sql);
   if ($result->num_rows == 0)
   {
      echo " not found in db, inserting\n";
      $sql = "INSERT INTO gm VALUES (NULL, '$filename', '$date')";
      echo "sql: $sql\n";
      if (mysqli_query($conn, $sql)) echo "New record created successfully\n";
      else echo "Error: " . $sql . "<br>" . mysqli_error($conn);
   }
   else echo " already in db\n";
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