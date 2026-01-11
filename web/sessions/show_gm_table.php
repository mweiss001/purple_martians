<?php

function show_player_cell($muid)
{
   $sql = "SELECT sessions.id, player_name, player_num, player_color, hostname FROM sessions ";
   $sql .= "LEFT JOIN gm_sessions ON sessions.id = session_id ";
   $sql .= "WHERE gm_muid='$muid'";

   echo "<div class=\"players-cell\">";

   $res = $GLOBALS['db']->query($sql);
   while ($row = $res->fetch(PDO::FETCH_ASSOC))
   {
      $num  = $row['player_num'];
      $col  = $row['player_color'];
      $name = $row['player_name'];
      $host = $row['hostname'];
     
      $iconpath = "/assets/icons/player_icon_$col.png";
      $alt = "alt=\"icon not found\" ";
      $title = "title=\"$num $name\n($host)\" ";
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



function show_gm_table()
{
   $current_session_id = $GLOBALS['current_session_id'];
   $gm_on  = $GLOBALS['gm_on'];
   $gm_set = $GLOBALS['gm_set'];

   if (!$gm_on) return;

   $title = "All Save Game Files";
   $sql = "SELECT gm.muid, dt_start, dt_end, level, duration, filename FROM gm ";

   if ($gm_set == 1)
   {
      $title = "Save Game Files for Current Session";
      $sql .= "LEFT JOIN gm_sessions ON gm.muid = gm_sessions.gm_muid ";
      $sql .= "WHERE gm_sessions.session_id=$current_session_id";
      
   }
   if ($gm_set == 2)
   {
      $title = "Orphaned Save Game Files (no matching session)";
      $sql .= "LEFT JOIN gm_sessions ON gm.muid = gm_sessions.gm_muid ";
      $sql .= "WHERE gm_sessions.gm_id IS NULL";
   }


   $but = "class=\"button\" id=\"gm_table_button\"";

   echo "<div id=\"gm_table\"  class=\"div-section-container\">";
      echo "<div id=\"gm_table\"  class=\"div-section-title-section-frame\">";
         echo "<div id=\"gm_table\"  class=\"div-section-title-section-container\">";
            echo "<div id=\"gm_table\"  class=\"div-section-title-frame\">$title</div>";
            echo "<div id=\"gm_table\"  class=\"div-section-title-frame-buttons-frame\">";

               if ($gm_set == 0) echo "<a href=\"sessions.php?gm_set=1\" $but >Show All</a>";
               if ($gm_set == 1) echo "<a href=\"sessions.php?gm_set=2\" $but >Show Current</a>";
               if ($gm_set == 2) echo "<a href=\"sessions.php?gm_set=0\" $but >Show Orphan</a>";

            echo "</div>";
         echo "</div>";
      echo "</div>";
                    


   $col_list = array("Start Time", "Duration", "Level", "Players", "Set Current", "Download", "Filename", "muid" );


           
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

               $res = $GLOBALS['db']->query($sql);
               while ($row = $res->fetch(PDO::FETCH_ASSOC))
               {
                  $muid     = $row['muid'];
                  $dt_start = $row['dt_start'];
                  $duration = secondsToHMS($row['duration']);
                  $level    = $row['level'];


                  $filename = $row['filename'];
                  $fullpath = "/downloads/$filename";
                  // if not set, set to the first one in the table
                  if ($GLOBALS['current_gm_muid'] == 0) $GLOBALS['current_gm_muid'] = $muid;
                  echo "<tr>";
                     echo "<td>$dt_start</td>";
                     echo "<td>$duration</td>";

                     echo "<td>";
                     show_level_cell($level);
                     echo "</td>";

                     echo "<td style=\"text-align: left;\">";
                     show_player_cell($muid);
                     echo "</td>";

                     echo "<td><a href=\"sessions.php?current_gm_muid=$muid\">set current</a></td>";
                     echo "<td><a href=\"$fullpath\" download=\"$filename\">download</a></td>";
                     echo "<td style=\"text-align: left;\">$filename</td>";
                     echo "<td>$muid</td>";

                  echo "</tr>";
               }
            echo "</tbody>";
         echo "</table>";
      echo "</div>";
   echo "</div>";
}
?>



