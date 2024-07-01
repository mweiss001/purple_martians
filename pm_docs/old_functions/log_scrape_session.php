<?php

$start_time = microtime(true);

$srvrname = "localhost";
$database = "Syslog";
$username = "pete";
$password = "park";

$date = new DateTime("now");
echo "\nLog scrape: " . $date->format('Ymd-His') . "\n";

// Create a connection
$conn = mysqli_connect($srvrname, $username, $password, $database);

// Check the connection
if (!$conn) die("Database connection failed: " . mysqli_connect_error());




function add_to_pm_session_table($message)
{
   // make an associative array and clear it
   $aa_data;
   unset($aa_data);


   $pieces = explode(" ", $message);
   foreach ($pieces as $x )
   {
      $p2 = explode(":", $x);

      if ($p2[0] == "timestamp")
      {
         // new column name 
         $p2[0] = "dt_start"; 

         // convert datetime format from YYYYMMDD-HHMMSS to YYYY-MM-DD HH:MM:SS 
         $a = str_split($p2[1]);
         
         $rb[0] = $a[0];
         $rb[1] = $a[1];
         $rb[2] = $a[2];
         $rb[3] = $a[3];
         $rb[4] = "-";
         $rb[5] = $a[4];
         $rb[6] = $a[5];
         $rb[7] = "-";
         $rb[8] = $a[6];
         $rb[9] = $a[7];
         $rb[10] = " ";
         $rb[11] = $a[9];
         $rb[12] = $a[10];
         $rb[13] = ":";
         $rb[14] = $a[11];
         $rb[15] = $a[12];
         $rb[16] = ":";
         $rb[17] = $a[13];
         $rb[18] = $a[14];

         $p2[1] = implode($rb);   
      }
      $aa_data[$p2[0]] = $p2[1];
   }

   // show the values in the array
   foreach ($aa_data as $x => $y) echo "$x $y\n";

   // check if there is already a row with the same filename

   $fn = $aa_data['filename'];

   echo "Filename $fn";

   $sql = "SELECT id FROM pm_session WHERE filename='$fn'";
   global $conn;
   $result = mysqli_query($conn, $sql);

   if ($result->num_rows == 0)
   {
      echo " not found in db, inserting\n";

      $ak = "";
      $av = "";
      // make a list of the key values and pairs
      foreach ($aa_data as $x => $y)
      {
         $ak .= $x . ", ";
         $av .= "'" . $y . "', ";
      }
      $ak = substr($ak, 0, -2); // chop last 2 char
      $av = substr($av, 0, -2); // chop last 2 char

      $sql = "INSERT INTO pm_session ($ak) VALUES ($av)";
      echo "sql: $sql\n";

      global $conn;

      if (mysqli_query($conn, $sql)) echo "New record created successfully\n";
      else echo "Error: " . $sql . "<br>" . mysqli_error($conn);

   }
   else
   {
      echo " found in db, updating\n";
      $row = $result->fetch_assoc();
      $id = $row['id'];
 
      echo "id: $id\n";
      $sql = "UPDATE pm_session SET ";
      foreach ($aa_data as $x => $y)
         $sql .= $x . "='" . $y . "', ";
      $sql = substr($sql, 0, -2); // chop last 2 char
      $sql .= " WHERE id=$id";
      echo "sql: $sql\n";
      if (mysqli_query($conn, $sql)) echo "Record updated successfully\n";
      else echo "Error: " . $sql . "<br>" . mysqli_error($conn);
   }
}


// Iterate rows in pm_log 
$sql = "SELECT id, message FROM pm_log WHERE message_type = 14";
global $conn;
$result = mysqli_query($conn, $sql);
$nr = $result->num_rows;

echo "Found $nr session log lines in pm_log\n";


if ($nr > 0)
{
   while($obj = $result->fetch_object())
   {   
      $id      = $obj->id;
      $message = $obj->message;

      // add to 'pm_session' table
      add_to_pm_session_table($message);


      // delete from 'pm_log' table
      $sql = "DELETE FROM pm_log WHERE ID = '$id'";
      echo "sql: $sql\n";
      // mysqli_query($conn, $sql);

   }
}

$et = microtime(true) - $start_time;
echo "\nCompleted in: $et seconds\n\n";

if ($nr)
{
   $tpr = $et / $nr;
   echo "$tpr per record\n\n";
}






