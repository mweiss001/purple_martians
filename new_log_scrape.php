<?php

$start_time = microtime(true);

$dir = "/home/m/dev/purple_martians/logs/";

$srvrname = "localhost";
$database = "Syslog";
$username = "pete";
$password = "park";

$date = new DateTime("now");
echo "\nNew log scrape: " . $date->format('Ymd-His') . "\n";

// Create a connection
$conn = mysqli_connect($srvrname, $username, $password, $database);

// Check the connection
if (!$conn) die("Database connection failed: " . mysqli_connect_error());


#echo posix_getpwuid(posix_geteuid())['name'];



echo "checking user:\n";

echo exec('whoami');


$username = posix_getpwuid(posix_geteuid())['name'];

echo "checking user: $username\n";


// Open directory and iterate files
$dir = "/home/m/dev/purple_martians/logs";
if (is_dir($dir))
{
   if ($dh = opendir($dir))
   {
      while (($file = readdir($dh)) !== false)
      {
         if (str_ends_with($file, ".csv"))
         {
            echo "$dir/$file\n";

            $sql = "LOAD DATA INFILE '$dir/$file' INTO TABLE pm_log COLUMNS TERMINATED BY ',' LINES TERMINATED BY '\\n' (msg_type, sub_type, created, agt, frame_num, player, client, message);";

            echo "$sql\n";
 
            global $conn;

            if (mysqli_query($conn, $sql)) echo "Success!\n";
            else echo "Error: " . $sql . "<br>" . mysqli_error($conn);

         }
         // else echo "file does not end in .txt\n";
      }
      closedir($dh);
   }
   else echo "error opening $dir\n";
}
else echo "$dir is not a directory\n";

$et = microtime(true) - $start_time;

echo "\nSession scrape completed in: $et seconds\n\n";


