<?php

include 'database.php';

function showPlayerNumIconNameHost($num, $col, $name, $sid, $host)
{
   $nm = str_pad($name, 8, " ", STR_PAD_RIGHT); 
   $ss = "Session Id:$sid";
   $ss = str_pad($ss, 15, " ", STR_PAD_RIGHT); 
   $hn = "Hostname:$host";
   $hn = str_pad($hn, 32, " ", STR_PAD_RIGHT); 

   $iconpath = "/assets/icons/player_icon_$col.png";
   $alt = "alt='icon not found'";
   echo "<div style='display:flex; gap:32px;'>";
      echo "<div class='iconTextContainer'>";
         echo "<span class='iconTextContainerText'>$num</span>";
         echo "<img class='iconTextContainerIcon' src=$iconpath $alt>";
         echo "<span class='iconTextContainerText'>$nm</span>";
     echo "</div>";
   echo "<span class='iconTextContainerText'>$ss</span>";
   echo "<span class='iconTextContainerText'>$hn</span>";
}



function doMainHTML($title, $filename, $validMuid)
{
   global $min;
 
   if (!$validMuid) $min = 1;

   $frame = 'divSectionContainer';
   if ($min) $frame = 'divSectionContainerMinimized';
   echo "<div class=\"currentGm $frame\">";
      echo '<div class="currentGm divSectionTitleSectionFrame">';
         echo '<div class="currentGm divSectionTitleSectionContainer">';
            echo "<div class='currentGm divSectionTitleFrame'>$title</div>";
            echo '<div class="currentGm divSectionTitleFrameButtonsFrame">';
         
            if ($validMuid)
            { 
               echo '<div style="display:flex; padding:0px 12px 0px 0px">';
                  echo "<a href=\"/downloads/$filename\" download class='button currentGm' >Download</a>";
               echo "</div>";
            }


               echo '<div id="currentGmControlsContainer"></div>';
            echo '</div>';
         echo '</div>';
      echo '</div>';


      if (!$validMuid) echo "</div>";
                
}





$min = 0;
if (isset($_GET['min'])) $min = $_GET['min'];


$err = 0;
if (isset($_GET['id'])) $currentGmMuid = $_GET['id'];
else { doMainHTML('Current Saved Game: muid not set', '',  0); return; } 

if ($currentGmMuid === 'undefined') { doMainHTML('Current Saved Game: muid not defined', '', 0);  return; }

$res = $db->query("SELECT COUNT(*) FROM gm WHERE muid='$currentGmMuid'");
if ($res->fetchColumn() == 0) { doMainHTML("Current Saved Game: muid: [$currentGmMuid] not found", '', 0); return; }

$res = $db->query("SELECT *, STRFTIME('%Y-%m-%d %H:%M:%S', dt_start, 'localtime') AS dts, STRFTIME('%Y-%m-%d %H:%M:%S', dt_end, 'localtime') AS dte FROM gm WHERE muid='$currentGmMuid'");
$row = $res->fetch(PDO::FETCH_ASSOC);
doMainHTML("Current Saved Game: $currentGmMuid", $row['filename'], 1); 


if (!$min)
{
   echo '<div style="display:flex; gap:2px; padding:4px;">';

      $iconpath = sprintf("/assets/icons/lev%03d.png", $row['level'] );
      echo "<img style=\" width: 160px; height: 160x;\" src=$iconpath alt=\"icon not found\" >";

      echo '<div class="currentGm divSectionSubSectionPretext">';
         echo "muid     : "            . $row['muid']             . "\n";
         echo "Filename : "            . $row['filename']         . "\n";
         echo "Level    : "            . $row['level']            . "\n";
         echo "Moves    : "            . $row['num_entries']      . "\n";
         echo "Start    : "            . date('Y-m-d h:i:s', strtotime($row['dts'])) . "\n";
         echo "End      : "            . date('Y-m-d h:i:s', strtotime($row['dte'])) . "\n";
         echo "Duration :            " . secondsToHMS($row['duration']) . "\n";
      echo '</div>';

   echo '</div>';

   echo '<div class="currentGm divSectionSubSectionTitleFrame">Players</div>';

   $sql = "SELECT sessions.id, player_name, player_num, player_color, hostname FROM sessions ";
   $sql .= "LEFT JOIN gm_sessions ON sessions.id = session_id ";
   $sql .= "WHERE gm_muid='$currentGmMuid'";

   echo '<div class="currentGm divSectionSubSectionTable">';
      echo '<table class="currentGmTable">';
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
            showPlayerNumIconNameHost($num, $col, $nam, $sid, $hst);
            echo "</td></tr>";
         }
      echo "</tbody></table>";
   echo "</div>";
}
echo "</div>";

?>


