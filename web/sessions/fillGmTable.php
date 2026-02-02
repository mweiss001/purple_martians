<?php

include 'database.php';

function showPlayerCell($muid)
{
   $sql = "SELECT sessions.id, player_name, player_num, player_color, hostname FROM sessions ";
   $sql .= "LEFT JOIN gm_sessions ON sessions.id = session_id ";
   $sql .= "WHERE gm_muid='$muid'";
   $res = $GLOBALS['db']->query($sql);

   echo "<div class='playersCell'>";
      while ($row = $res->fetch(PDO::FETCH_ASSOC))
      {
         $sid  = $row['id'];
         $num  = $row['player_num'];
         $col  = $row['player_color'];
         $name = $row['player_name'];
         $host = $row['hostname'];
         $iconpath = "/assets/icons/player_icon_$col.png";
         $alt = 'alt="icon not found" ';
         $title = " title=\"Player number: $num\nPlayer name: $name\nHostname: $host\nSession id: $sid\"  ";
         echo "<img src=$iconpath $alt $title class='playersCellIcon'>";
         echo "</a>";
      }
   echo "</div>";
}
  
 
function showLevelCell($level)
{
   echo "<div class='levelCell'>";
   $iconpath = sprintf("/assets/icons/lev%03d.png", $level );
   $alt = "alt='icon not found'";
   echo "<img src=$iconpath $alt class='playersCellIcon'>";
   echo sprintf("[%02d]", $level);
   echo "</div>";
}


$id = 0;
$min = 0;
$view = 0;

if (isset($_GET['id']))   $id   = $_GET['id'];
if (isset($_GET['min']))  $min  = $_GET['min'];
if (isset($_GET['view'])) $view = $_GET['view'];

if ($id)
{
   $sql = "SELECT gm.muid, STRFTIME('%Y-%m-%d %H:%M:%S', dt_start, 'localtime') AS dts, level, duration, filename FROM gm ";

   if ($view == 1) $title = "All Save Game Files";   
   if ($view == 0)
   {
      $title = "Save Game Files for Current Session";
      $sql .= "LEFT JOIN gm_sessions ON gm.muid = gm_sessions.gm_muid ";
      $sql .= "WHERE gm_sessions.session_id=$id";
   }
   if ($view == 2)
   {
      $title = "Orphaned Save Game Files (no matching session)";
      $sql .= "LEFT JOIN gm_sessions ON gm.muid = gm_sessions.gm_muid ";
      $sql .= "WHERE gm_sessions.gm_muid IS NULL";
   }

   $columnList = array("Start Time", "Duration", "Level", "Players", "Set Current", "Download", "Filename", "muid" );
   
   
   $frame = 'divSectionContainer';
   if ($min) $frame = 'divSectionContainerMinimized';
   echo "<div class=\"gmTable $frame\">";
      echo "<div class=\"gmTable divSectionTitleSectionFrame\">";
         echo "<div class=\"gmTable divSectionTitleSectionContainer\">";
            echo "<div class=\"gmTable divSectionTitleFrame\">$title</div>";
            echo "<div class=\"gmTable divSectionTitleFrameButtonsFrame\">";
               echo '<div id="gmTableControlsContainer"></div>';
            echo "</div>";
         echo "</div>";
      echo "</div>";

   if (!$min)
   {
      echo "<div class='gmTable divSectionSubSectionTable'>";
         echo "<table class='gmTableTable'>";
            echo "<thead'>";
               echo "<tr>";
                  foreach($columnList as $col)
                  {
                     if (($col == "Filename") || ($col == "Players"))
                        echo "<th style=\"text-align: left;\">$col</th>";
                     else echo "<th>$col</th>";
                  }
               echo "</tr>";
            echo "</thead>";
            echo "<tbody>";

               $res = $db->query($sql);
               while ($row = $res->fetch(PDO::FETCH_ASSOC))
               {
                  $muid     = $row['muid'];
                  $dts      = $row['dts'];
                  $duration = secondsToHMS($row['duration']);
                  $level    = $row['level'];

                  $filename = $row['filename'];
                  $fullpath = "/downloads/$filename";


                  echo "<tr currentGmMuid=$muid >";
                     echo "<td>$dts</td>";
                     echo "<td>$duration</td>";

                     echo "<td>";
                     showLevelCell($level);
                     echo "</td>";

                     echo "<td style=\"text-align: left;\">";
                     showPlayerCell($muid);
                     echo "</td>";

                     echo "<td><a href=\"#\">set current</a></td>";

                     echo "<td><a href=\"$fullpath\" download=\"$filename\">download</a></td>";
                     echo "<td style=\"text-align: left;\">$filename</td>";
                     echo "<td>$muid</td>";

                  echo "</tr>";
               }
            echo "</tbody>";
         echo "</table>";
      echo "</div>";
   }
   echo "</div>";
}
?>
