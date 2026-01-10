<?php
function show_gm_table()
{
   $current_session_id = $GLOBALS['current_session_id'];
   $gm_on  = $GLOBALS['gm_on'];
   $gm_set = $GLOBALS['gm_set'];

   if (!$gm_on) return;

   $title = "All Save Game Files";
   $sql = "SELECT gm.muid, dt_start, dt_end, duration, filename FROM gm ";

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
                    
   $col_list = array("muid",  "Start Time", "Duration", "Set Current", "Download", "Filename");
           
      echo "<div id=\"gm_table\"  class=\"div-section-sub-section-gm_table\">";
         echo "<table id='mdw'>";
            echo "<thead'>";
               echo "<tr>";
                  foreach($col_list as $col)
                  {
                     if ($col == "Filename") echo "<th  id='mh_left'>$col</th>";
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
                  $duration = $row['duration'];
                  $filename = $row['filename'];
                  $fullpath = "/downloads/$filename";

                  echo "<tr>";
                     echo "<td id='md_center'; >$muid</td>";
                     echo "<td id='md_center'; >$dt_start</td>";
                     echo "<td id='md_center'; >$duration</td>";
                     echo "<td id='md_center'; ><a href=\"sessions.php?current_gm_muid=$muid\">set current</a></td>";
                     echo "<td id='md_center'; ><a href=\"$fullpath\" download=\"$filename\">download</a></td>";
                     echo "<td id='md_left'; >$filename</td>";
                  echo "</tr>";
               }
            echo "</tbody>";
         echo "</table>";
      echo "</div>";
   echo "</div>";
}
?>



