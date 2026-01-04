<?php


function show_current_gm()
{
   $current_gm_id = $GLOBALS['current_gm_id'];

   $result = mysqli_query($GLOBALS['conn'], "SELECT * FROM gm WHERE id=$current_gm_id" );
   if ($result->num_rows == 0) { echo " gm not found \n";  return; }
   $row = $result->fetch_assoc();


   echo "<div id=\"current_session\"  class=\"div-section-container\">";
      echo "<div id=\"current_session\"  class=\"div-section-title-section-frame\">";
         echo "<div id=\"current_session\"  class=\"div-section-title-section-container\">";
            echo "<div id=\"current_session\"  class=\"div-section-title-frame\">Current Saved Game</div>";
         echo "</div>";
      echo "</div>";


      echo "<div id=\"current_session\"  class=\"div-section-sub-section-pretext\">";
         echo "Id              : "            . $row['id']           . "\n";
         echo "Filename        : "            . $row['filename']     . "\n";
         echo "Start           : "            . $row['dt_start']  . "\n";
         echo "End             : "            . $row['dt_end']    . "\n";
         $dur = secondsToHMS($row['duration']);
         echo "Duration        :            " . $dur              . "\n";
         echo "</div>";
      
   echo "</div>";
}




?>

