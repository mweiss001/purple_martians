<?php

echo "<hr><p><h3>Orphan Sessions</h3></p><hr>";

$sql  = "SELECT sessions.* FROM sessions ";
$sql .= "WHERE sessions.id NOT IN (SELECT gm_sessions.session_id FROM gm_sessions)";

$result = mysqli_query($conn, $sql);
if ($result->num_rows == 0) echo " none found \n";
else
{
   while ($row = $result->fetch_assoc())
   {
      $gm['id'] = $row['id'];  
      $gm['ts'] = $row['dt_start'];  
      $gm['te'] = $row['dt_end'];  
      $gm['fn'] = $row['filename'];  

      echo "<p>";

      foreach ($gm as $x => $y) echo "$x:$y   -   ";
      
      echo "</p>";
   }
}
?>

