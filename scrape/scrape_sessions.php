<?php

// if using php < version 8
//function str_starts_with($str, $txt) {   return (substr($str, 0, strlen($txt) ) == $txt); }
//function str_ends_with($str, $txt)   {   return (substr($str,   -strlen($txt) ) == $txt); }

$start_time = microtime(true);

$dir = "/home/m/dev/purple_martians/logs/session/";

$srvrname = "localhost";
$database = "pm";
$username = "pmdb_rw";
$password = "readwrite";

$date = new DateTime("now");
echo "\nSession log scrape: " . $date->format('Ymd-His') . "\n";

// Create a connection
$conn = mysqli_connect($srvrname, $username, $password, $database);

// Check the connection
if (!$conn) die("Database connection failed: " . mysqli_connect_error());

function parse_line($line, $tag, &$var)
{
   if (str_starts_with($line, $tag))
   {
      $repl = array($tag, "\n");              // pattern to remove the tag and /n at the end
      $tmp = str_replace($repl, "", $line);   // do the replace
      $tag = str_replace(":", "", $tag);      // remove the : from tag
      $var[$tag] = $tmp;                      // add to array
   }
}

function add_to_db(&$var)
{
   echo "\n";

   // show the values in the array
   //foreach ($var as $x => $y) echo "$x $y\n";

   // echo "Add to db: filename: " . $var['filename'] . "\n";

   // test if we can create a valid DateTime
   if (!DateTime::createFromFormat('Ymd-His', $var['dt_start']))
   {
      echo "Error parsing dates\n";
      foreach ($var as $x => $y) echo "$x $y\n";
      return;
   }

   // convert the datetime to something mysql likes
   $date2 = DateTime::createFromFormat('Ymd-His', $var['dt_start']);
   $var['dt_start'] =  $date2->format('Y-m-d H:i:s');


   // calculate and set dt_end
   $dur = $var['duration'];
   $date3 = $date2->add(DateInterval::createFromDateString("$dur seconds"));
   $var['dt_end'] =  $date3->format('Y-m-d H:i:s');

   // show the values in the array
   //foreach ($var as $x => $y) echo "$x $y\n";


   // first check if filename is exists in db
   $fn = $var['filename'];

   echo "Filename $fn";

   $sql = "SELECT id FROM sessions WHERE filename='$fn'";
   global $conn;
   $result = mysqli_query($conn, $sql);

   if ($result->num_rows == 0)
   {
      echo " not found in db, inserting\n";
      $ak = "";
      $av = "";
      // make a list of the key values and pairs
      foreach ($var as $x => $y)
      {
         $ak .= $x . ", ";
         $av .= "'" . $y . "', ";
      }
      $ak = substr($ak, 0, -2); // chop last 2 char
      $av = substr($av, 0, -2); // chop last 2 char
      $sql = "INSERT INTO sessions ($ak) VALUES ($av)";
      //echo "sql: $sql\n";
      if (mysqli_query($conn, $sql)) echo "New record created successfully\n";
      else echo "Error: " . $sql . "<br>" . mysqli_error($conn);
   }
   else
   {
      echo " found in db, updating\n";
      $row = $result->fetch_assoc();
      $id = $row['id'];
      //echo "id: $id\n";
      $sql = "UPDATE sessions SET ";
      foreach ($var as $x => $y)
         $sql .= $x . "='" . $y . "', ";
      $sql = substr($sql, 0, -2); // chop last 2 char
      $sql .= " WHERE id=$id";
      //echo "sql: $sql\n";
      if (mysqli_query($conn, $sql)) echo "Record updated successfully\n";
      else echo "Error: " . $sql . "<br>" . mysqli_error($conn);
   }


   // after adding to db, check if session is closed
   // if it is, we are done with this file and will move it somewhere so it doesnt get scraped again..

   $move = 0;
   if ($var['endreason'] == "unknown")
   {
      echo "Session is still open!\n";
       // if session is really old give up and move it also

      $date1 = new DateTime("now");

      $date2 = DateTime::createFromFormat('Y-m-d H:i:s', $var['dt_start']);
      // echo $date2->format('Y-m-d H:i:s') . "\n";
      $interval = $date1->diff($date2);
      $age_in_hours = $interval->h;

      //echo "Session has been open for $age_in_hours hours.\n";
      //echo "Session has been open for $interval->i minutes.\n";
      //echo "Session has been open for $interval->s seconds.\n";


      $sec_since_session_start = $interval->s + $interval->i * 60 + $interval->h * 3600;
      //echo "Seconds since session start: $sec_since_session_start\n";

      $duration = $var['duration'];
      //echo "Session reports a duration of $duration seconds.\n";

      $dif = ($sec_since_session_start - $duration) / 60;

      echo "Session has been running $dif/120 minutes longer than reported duration.\n";

      if ($dif > 7200) $move = 1; // more than 2 hours difference
   }
   else
   {
      echo "Session is closed!\n";
      $move = 1;
   }
   if ($move)
   {
      global $dir;
      $fn = $dir . $var['filename'];
      echo "Moving and renaming session log file\n";
      rename($fn, $fn . ".bak");
   }
}

// Open directory and iterate files
$dir = "/home/m/dev/purple_martians/logs/session/";
if (is_dir($dir))
{
   if ($dh = opendir($dir))
   {
      while (($file = readdir($dh)) !== false)
      {
         if (str_ends_with($file, ".txt"))
         {
            // make an associative array and clear it
            $aa_data;

            unset($aa_data);
            $aa_data['filename'] = $file;

            $file_handle = fopen($dir.$file, "r");
            while (!feof($file_handle))
            {
               $line = fgets($file_handle);
               //echo $line;

               parse_line($line, "dt_start:",          $aa_data);
               parse_line($line, "duration:",          $aa_data);
               parse_line($line, "ip:",                $aa_data);
               parse_line($line, "port:",              $aa_data);
               parse_line($line, "hostname:",          $aa_data);
               parse_line($line, "endreason:",         $aa_data);
               parse_line($line, "cdat_rx:",           $aa_data);
               parse_line($line, "player_num:",        $aa_data);
               parse_line($line, "player_name:",       $aa_data);
               parse_line($line, "player_color:",      $aa_data);
               parse_line($line, "next_levels:",       $aa_data);
               parse_line($line, "exits_activated:",   $aa_data);
               parse_line($line, "respawns:",          $aa_data);
               parse_line($line, "shots_fired:",       $aa_data);
               parse_line($line, "enemy_hits:",        $aa_data);
               parse_line($line, "player_hits:",       $aa_data);
               parse_line($line, "self_hits:",         $aa_data);
               parse_line($line, "purple_coins:",      $aa_data);

               parse_line($line, "tx_bytes_total:",          $aa_data);
               parse_line($line, "tx_bytes_avg_per_sec:",    $aa_data);
               parse_line($line, "tx_bytes_max_per_frame:",  $aa_data);
               parse_line($line, "rx_bytes_total:",          $aa_data);
               parse_line($line, "rx_bytes_avg_per_sec:",    $aa_data);
               parse_line($line, "rx_bytes_max_per_frame:",  $aa_data);

               parse_line($line, "tx_packets_total:",          $aa_data);
               parse_line($line, "tx_packets_avg_per_sec:",    $aa_data);
               parse_line($line, "tx_packets_max_per_frame:",  $aa_data);
               parse_line($line, "rx_packets_total:",          $aa_data);
               parse_line($line, "rx_packets_avg_per_sec:",    $aa_data);
               parse_line($line, "rx_packets_max_per_frame:",  $aa_data);


            }
            fclose($file_handle);


            add_to_db($aa_data);

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
