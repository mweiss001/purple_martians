<?php

function show_player_num_icon_name_host($num, $col, $name, $host)
{
   $iconpath = "/assets/icons/player_icon_$col.png";
   $alt = "alt=\"icon not found\"";
   echo "<div class=\"icon-text-container\">";
   echo "<span class=\"text\">$num</span>";
   echo "<img src=$iconpath $alt class=\"icon\">";
   echo "<span class=\"text\">$name</span>";
   echo "<span class=\"text\">($host)</span>";
   echo "</div>";
}

function show_current_gm()
{
   $current_gm_muid = $GLOBALS['current_gm_muid'];

   $res = $GLOBALS['db']->query("SELECT COUNT(*) FROM gm WHERE muid='$current_gm_muid'");
   if ($res->fetchColumn() == 0)  { echo " session [$current_gm_muid] not found \n";  return; } 

   $res = $GLOBALS['db']->query("SELECT * FROM gm WHERE muid='$current_gm_muid'");
   $row = $res->fetch(PDO::FETCH_ASSOC);

   echo "<div id=\"current_session\"  class=\"div-section-container\">";

      echo "<div id=\"current_session\"  class=\"div-section-title-section-frame\">";
         echo "<div id=\"current_session\"  class=\"div-section-title-section-container\">";
            echo "<div id=\"current_session\"  class=\"div-section-title-frame\">Current Saved Game</div>";
         echo "</div>";
      echo "</div>";

      echo "<div id=\"current_session\"  class=\"div-section-sub-section-pretext\">";
         echo "muid            : "            . $row['muid']             . "\n";
         echo "Filename        : "            . $row['filename']         . "\n";
         echo "Num Entries     : "            . $row['num_entries']      . "\n";
         echo "Level           : "            . $row['level']            . "\n";
         echo "Start           : "            . date('Y-m-d h:i:s', strtotime($row['dt_start'])) . "\n";
         echo "End             : "            . date('Y-m-d h:i:s', strtotime($row['dt_end'])) . "\n";
         echo "Duration        :            " . secondsToHMS($row['duration']) . "\n";
      echo "</div>";

      echo "<div id=\"current_session\" class=\"div-section-sub-section-title-frame\">Players</div>";

      $sql = "SELECT sessions.id, player_name, player_num, player_color, hostname FROM sessions ";
      $sql .= "LEFT JOIN gm_sessions ON sessions.id = session_id ";
      $sql .= "WHERE gm_muid='$current_gm_muid'";
      

      echo "<div id=\"gm_table\"  class=\"div-section-sub-section-gm_table\">";
         echo "<table id='mdw2'><tbody>";
            $res = $GLOBALS['db']->query($sql);
            while ($row = $res->fetch(PDO::FETCH_ASSOC))
            {
                echo "<tr><td id='md_left2';>";
                show_player_num_icon_name_host($row['player_num'], $row['player_color'], $row['player_name'], $row['hostname']);
                echo "</td></tr>";
            }
         echo "</tbody></table>";
      echo "</div>";
   echo "</div>";
}
?>

