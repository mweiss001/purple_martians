<?php

$dt = new DateTime("now");
echo "\nSession log scrape: " . $dt->format('Ymd-His') . "\n";

//$srvrname = "localhost";
//$database = "pm";
//$username = "pmdb_rw";
//$password = "readwrite";

//$conn = mysqli_connect($srvrname, $username, $password, $database);
//if (!$conn) die("Database connection failed: " . mysqli_connect_error());


// database setup
$db_filepath = "/home/m/dev/purple_martians/data/database.db";
if (!file_exists($db_filepath))
{
   echo "Database file: $filename not found.";
   return;
}
$db = new PDO("sqlite:/home/m/dev/purple_martians/data/database.db");







function check_for_gm($se)
{
   echo "\nSession ";
   foreach ($se as $x => $y) echo "$x:$y   -   ";

   $s_id = $se['id'];
   $s = $se['start'];
   $e = $se['end'];

   // select all gm with either start or end within session datetime range
   $sql = "SELECT id, filename, dt_start, dt_end FROM gm";
   $sql .= " WHERE (dt_start>'$s' AND dt_start<'$e') ";
   $sql .= " OR    (dt_end  >'$s' AND dt_end  <'$e') ";

   echo "\n$sql ";

   $res = $GLOBALS['db']->query($sql);
   while ($row = $res->fetch(PDO::FETCH_ASSOC))
   {
      $gm_id = $row['id'];  
      $sql = "INSERT INTO gm_sessions VALUES (NULL, '$gm_id', '$s_id' )";
      echo "sql: $sql\n";
      $res = $GLOBALS['db']->query($sql);
   }
}




//   global $conn;
//   $result = mysqli_query($conn, $sql);
//   echo "\nFound $result->num_rows gm in date range\n";
//   if ($result->num_rows)
//   {
//      while ($row = $result->fetch_assoc())
//      {
//         $gm_id = $row['id'];  
//         $sql = "INSERT INTO gm_sessions VALUES (NULL, '$gm_id', '$s_id' )";
//         echo "sql: $sql\n";
//         $res = $GLOBALS['db']->query($sql);



//         $sql = "INSERT INTO gm_sessions VALUES (NULL, '$gm_id', '$s_id' )";
//         if (mysqli_query($conn, $sql)) echo "$sql - new record created successfully\n";
//         else echo "$sql error: " . mysqli_error($conn);
//      }
//   }


function remove_all_links()
{
   $sql = "DELETE FROM gm_sessions";
//   global $conn;
//   if (mysqli_query($conn, $sql))  echo "gm_sessions cleared successfully\n";
//   else echo "Error: " . $sql . "<br>" . mysqli_error($conn);


    $res = $GLOBALS['db']->query($sql);    






}

remove_all_links();

// iterate sessions

$res = $GLOBALS['db']->query("SELECT id, dt_start, dt_end FROM sessions");
while ($row = $res->fetch(PDO::FETCH_ASSOC))
{
   $se['id'] = $row['id'];  
   $se['start'] = $row['dt_start'];  
   $se['end'] = $row['dt_end'];  
   check_for_gm($se);
}

//$result = mysqli_query($conn, "SELECT id, dt_start, dt_end FROM sessions");
//if ($result->num_rows == 0) echo " none found \n";
//else
//{
//   while ($row = $result->fetch_assoc())
//   {
//      $se['id'] = $row['id'];  
//      $se['start'] = $row['dt_start'];  
//      $se['end'] = $row['dt_end'];  
//      check_for_gm($se);
//   }
//}

?>