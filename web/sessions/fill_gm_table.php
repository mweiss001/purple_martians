<?php

include 'database.php';


function show_player_cell($muid)
{
   $sql = "SELECT sessions.id, player_name, player_num, player_color, hostname FROM sessions ";
   $sql .= "LEFT JOIN gm_sessions ON sessions.id = session_id ";
   $sql .= "WHERE gm_muid='$muid'";

   echo "<div class=\"players-cell\">";

   $res = $GLOBALS['db']->query($sql);
   while ($row = $res->fetch(PDO::FETCH_ASSOC))
   {
      $sid  = $row['id'];
      $num  = $row['player_num'];
      $col  = $row['player_color'];
      $name = $row['player_name'];
      $host = $row['hostname'];
      $iconpath = "/assets/icons/player_icon_$col.png";
      $alt = "alt=\"icon not found\" ";
      $title = " title=\" Player number: $num\nPlayer name: $name\nHostname: $host\nSession id: $sid\"  ";
      echo "<img src=$iconpath $alt $title class=\"players-cell-icon\">";
      echo "</a>";
   }
   echo "</div>";
}
 
function show_level_cell($level)
{
   echo "<div class=\"level-cell\">";
   $iconpath = sprintf("/assets/icons/lev%03d.png", $level );
   $alt = "alt=\"icon not found\" ";
   echo "<img src=$iconpath $alt class=\"players-cell-icon\" >";
   echo sprintf("[%02d]", $level);
   echo "</div>";
}


$id = 0;

if (isset($_GET['id']))   $id     = $_GET['id'];
if (isset($_GET['view'])) $gm_set = $_GET['view'];

if ($id)
{

   $sql = "SELECT gm.muid, STRFTIME('%Y-%m-%d %H:%M:%S', dt_start, 'localtime') AS dts, level, duration, filename FROM gm ";

   if ($gm_set == 0) $title = "Save Game Files";
   if ($gm_set == 2) $title = "All Save Game Files";   
   if ($gm_set == 1) // current
   {
      $title = "Save Game Files for Current Session";
      $sql .= "LEFT JOIN gm_sessions ON gm.muid = gm_sessions.gm_muid ";
      $sql .= "WHERE gm_sessions.session_id=$id";
   }
   if ($gm_set == 3)
   {
      $title = "Orphaned Save Game Files (no matching session)";
      $sql .= "LEFT JOIN gm_sessions ON gm.muid = gm_sessions.gm_muid ";
      $sql .= "WHERE gm_sessions.gm_muid IS NULL";
   }

   $col_list = array("Start Time", "Duration", "Level", "Players", "Set Current", "Download", "Filename", "muid" );
           
   $but = "class=\"button\" id=\"gm_table_button\"";

   echo "<div id=\"gm_table\"  class=\"div-section-container\">";
      echo "<div id=\"gm_table\"  class=\"div-section-title-section-frame\">";
         echo "<div id=\"gm_table\"  class=\"div-section-title-section-container\">";
            echo "<div id=\"gm_table\"  class=\"div-section-title-frame\">$title</div>";
            echo "<div id=\"gm_table\"  class=\"div-section-title-frame-buttons-frame\">";


               echo '<label>View:</label>';
               echo '<select name="range" id="gm_table_view_select">';
                   echo '<option value="0">Title Only</option>';
                   echo '<option value="1">Current</option>';
                   echo '<option value="2">All</option>';
                   echo '<option value="3">Orphan</option>';
               echo '</select>';

            echo "</div>";
         echo "</div>";
      echo "</div>";


      if ($gm_set)
      {

      echo "<div id=\"gm_table\"  class=\"div-section-sub-section-gm_table\">";
         echo "<table id='gm_tablet'>";
            echo "<thead'>";
               echo "<tr>";
                  foreach($col_list as $col)
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


                  echo "<tr current_gm_muid=$muid >";
                     echo "<td>$dts</td>";
                     echo "<td>$duration</td>";

                     echo "<td>";
                     show_level_cell($level);
                     echo "</td>";

                     echo "<td style=\"text-align: left;\">";
                     show_player_cell($muid);
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



