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


function get_tag(&$line, $tag, &$var)
{
   // find first occurance of closing bracket ']'
   $b2pos = strpos($line, ']');

//   echo "first ]:$b2pos\n";  
   
   // add to associative array
   $var[$tag] = substr($line, 1, $b2pos-1);

   // chop tag from line
   $line = substr($line, $b2pos+1);
}


function add_to_db(&$var)
{
//   echo "\n";

   // show the values in the array
   //foreach ($var as $x => $y) echo "$x $y\n";

//   echo "Inserting\n";
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
   $sql = "INSERT INTO pm_log ($ak) VALUES ($av)";
   //echo "sql: $sql\n";

   global $conn;

//   if (mysqli_query($conn, $sql)) echo "New record created successfully\n";
//   else echo "Error: " . $sql . "<br>" . mysqli_error($conn);

   if (!mysqli_query($conn, $sql)) echo "Error: " . $sql . "<br>" . mysqli_error($conn);






}




// Iterate rows in SystemEvents 
$sql = "SELECT ID, ReceivedAt, Message FROM SystemEvents WHERE SysLogTag ='pm'";
global $conn;
$result = mysqli_query($conn, $sql);
$nr = $result->num_rows;

echo "Found $nr new log lines in SystemEvents\n";

if ($nr > 0)
{

   while($obj = $result->fetch_object())
   {   
      $ID         = $obj->ID;
      $ReceivedAt = $obj->ReceivedAt;
      $Message    = $obj->Message;

      // echo "ID:$ID  ReceivedAt:$ReceivedAt Message:$Message\n";

      // make an associative array and clear it
      $aa_data;
      unset($aa_data);

      $aa_data["received_at"] = $ReceivedAt;

      get_tag($Message, "agt",           $aa_data);
      get_tag($Message, "message_type",  $aa_data);
      get_tag($Message, "player_num",    $aa_data);
      get_tag($Message, "frame_num",     $aa_data);

      $aa_data["message"] = $Message;

      // add to the pm_log table
      add_to_db($aa_data);

      // delete from SystemEvents
      $sql = "DELETE FROM SystemEvents WHERE ID = '$ID'";
      //echo "sql: $sql\n";
      mysqli_query($conn, $sql);




   }
}

$et = microtime(true) - $start_time;
echo "\nLog scrape completed in: $et seconds\n\n";

if ($nr)
{
   $tpr = $et / $nr;
   echo "$tpr per record\n\n";
}

