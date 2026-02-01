<?php

include 'database.php';

function show_player_num_icon_name_host($num, $col, $name, $sid, $host)
{
   $nm = str_pad($name, 8, " ", STR_PAD_RIGHT); 
   $ss = "Session Id:$sid";
   $ss = str_pad($ss, 15, " ", STR_PAD_RIGHT); 
   $hn = "Hostname:$host";
   $hn = str_pad($hn, 32, " ", STR_PAD_RIGHT); 

   $iconpath = "/assets/icons/player_icon_$col.png";
   $alt = "alt=\"icon not found\"";
   echo "<div style=\" display:flex; gap:32px; \">";
      echo "<div class=\"icon-text-container\">";
         echo "<span class=\"text\">$num</span>";
         echo "<img src=$iconpath $alt class=\"icon\">";
         echo "<span class=\"text\">$nm</span>";
     echo "</div>";
   echo "<span class=\"text\">$ss</span>";
   echo "<span class=\"text\">$hn</span>";
}


if (isset($_GET['id']))   $current_gm_muid   = $_GET['id'];
else { echo " muid not set \n";  return; } 
if ($current_gm_muid === 'undefined') { echo " muid not defined\n";  return; } 



$min = 0;
if (isset($_GET['min']))   $min   = $_GET['min'];



   $res = $db->query("SELECT COUNT(*) FROM gm WHERE muid='$current_gm_muid'");
   if ($res->fetchColumn() == 0)  { echo " muid: [$current_gm_muid] not found \n";  return; } 

   $res = $db->query("SELECT *, STRFTIME('%Y-%m-%d %H:%M:%S', dt_start, 'localtime') AS dts, STRFTIME('%Y-%m-%d %H:%M:%S', dt_end, 'localtime') AS dte FROM gm WHERE muid='$current_gm_muid'");
   $row = $res->fetch(PDO::FETCH_ASSOC);

   $but = 'class="button current_gm_button"'; 

   echo '<div class="div-section-container current_gm">';
      echo '<div class="div-section-title-section-frame current_gm">';
         echo '<div class="div-section-title-section-container current_gm">';
            echo "<div class=\"div-section-title-frame current_gm\">Current Saved Game: $current_gm_muid</div>";

            echo '<div class="current_gm div-section-title-frame-buttons-frame">';

               $fullpath = "/downloads/" . $row['filename'];
               echo "<a href=\"$fullpath\" download=\"$filename\" $but >Download</a>";

               echo '<div id="current_gm-cont"></div>';

            echo '</div>';

         echo '</div>';
      echo '</div>';

if (!$min)
{

      echo '<div style="display:flex; gap:2px; padding:4px;">';

         $iconpath = sprintf("/assets/icons/lev%03d.png", $row['level'] );
         echo "<img style=\" width: 160px; height: 160x;\" src=$iconpath alt=\"icon not found\" >";

         echo '<div class="current_gm div-section-sub-section-pretext">';
            echo "muid     : "            . $row['muid']             . "\n";
            echo "Filename : "            . $row['filename']         . "\n";
            echo "Level    : "            . $row['level']            . "\n";
            echo "Moves    : "            . $row['num_entries']      . "\n";
            echo "Start    : "            . date('Y-m-d h:i:s', strtotime($row['dts'])) . "\n";
            echo "End      : "            . date('Y-m-d h:i:s', strtotime($row['dte'])) . "\n";
            echo "Duration :            " . secondsToHMS($row['duration']) . "\n";
         echo '</div>';

      echo '</div>';

      echo '<div class="current_gm div-section-sub-section-title-frame">Players</div>';

      $sql = "SELECT sessions.id, player_name, player_num, player_color, hostname FROM sessions ";
      $sql .= "LEFT JOIN gm_sessions ON sessions.id = session_id ";
      $sql .= "WHERE gm_muid='$current_gm_muid'";

      echo '<div class="current_gm div-section-sub-section-gm_table">';
         echo '<table class="current_gm_table">';
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

}

   echo "</div>";

?>


