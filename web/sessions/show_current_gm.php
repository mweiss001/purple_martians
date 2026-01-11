<?php

function show_player_num_icon_name_host($num, $col, $name, $sid, $host)
{
   $iconpath = "/assets/icons/player_icon_$col.png";
   $alt = "alt=\"icon not found\"";
   echo "<div style=\" display:flex; gap:40px; \">";
      echo "<div class=\"icon-text-container\">";
         echo "<span class=\"text\">$num</span>";
         echo "<img src=$iconpath $alt class=\"icon\">";
         echo "<span class=\"text\">$name</span>";
     echo "</div>";

   echo "<span class=\"text\">Session Id:$sid</span>";
   echo "<span class=\"text\">Hostname:$host</span>";
}

function show_current_gm()
{
   $current_gm_muid = $GLOBALS['current_gm_muid'];
   $gm_on  = $GLOBALS['gm_on'];
   if (!$gm_on) return;


   $res = $GLOBALS['db']->query("SELECT COUNT(*) FROM gm WHERE muid='$current_gm_muid'");
   if ($res->fetchColumn() == 0)  { echo " muid: [$current_gm_muid] not found \n";  return; } 

   $res = $GLOBALS['db']->query("SELECT * FROM gm WHERE muid='$current_gm_muid'");
   $row = $res->fetch(PDO::FETCH_ASSOC);



   $but = "class=\"button\" id=\"current_gm_button\""; 

   echo "<div id=\"current_gm\"  class=\"div-section-container\">";
      echo "<div id=\"current_gm\"  class=\"div-section-title-section-frame\">";
         echo "<div id=\"current_gm\"  class=\"div-section-title-section-container\">";
            echo "<div id=\"current_gm\"  class=\"div-section-title-frame\">Current Saved Game</div>";

            echo "<div id=\"current_gm\"  class=\"div-section-title-frame-buttons-frame\">";
               $fullpath = "/downloads/" . $row['filename'];
               if ($gm_set == 0) echo "<a href=\"$fullpath\" download=\"$filename\" $but >Download</a>";

            echo "</div>";

         echo "</div>";
      echo "</div>";

      echo "<div style=\" display:flex; gap:2px;  padding:4px;\">";

         $iconpath = sprintf("/assets/icons/lev%03d.png", $row['level'] );
         echo "<img style=\" width: 160px; height: 160x;\" src=$iconpath alt=\"icon not found\" >";

         echo "<div id=\"current_gm\"  class=\"div-section-sub-section-pretext\">";
            echo "muid     : "            . $row['muid']             . "\n";
            echo "Filename : "            . $row['filename']         . "\n";
            echo "Level    : "            . $row['level']            . "\n";
            echo "Moves    : "            . $row['num_entries']      . "\n";
            echo "Start    : "            . date('Y-m-d h:i:s', strtotime($row['dt_start'])) . "\n";
            echo "End      : "            . date('Y-m-d h:i:s', strtotime($row['dt_end'])) . "\n";
            echo "Duration :            " . secondsToHMS($row['duration']) . "\n";
         echo "</div>";

      echo "</div>";

      echo "<div id=\"current_gm\" class=\"div-section-sub-section-title-frame\">Players</div>";

      $sql = "SELECT sessions.id, player_name, player_num, player_color, hostname FROM sessions ";
      $sql .= "LEFT JOIN gm_sessions ON sessions.id = session_id ";
      $sql .= "WHERE gm_muid='$current_gm_muid'";

      echo "<div id=\"current_gm\"  class=\"div-section-sub-section-gm_table\">";
         echo "<table id=\"current_gm_table\">";
            echo "<tbody>";
            $res = $GLOBALS['db']->query($sql);
            while ($row = $res->fetch(PDO::FETCH_ASSOC))
            {
               $num = $row['player_num'];
               $col = $row['player_color'];
               $nam = $row['player_name'];
               $hst = $row['hostname'];
               $sid = $row['id'];
               echo "<tr><td style=\"text-align: left;\">";
               show_player_num_icon_name_host($num, $col, $nam, $sid, $hst);
               echo "</td></tr>";
            }
         echo "</tbody></table>";
      echo "</div>";
   echo "</div>";
}
?>


