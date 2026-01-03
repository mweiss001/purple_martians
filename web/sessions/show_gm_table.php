<?php
function show_gm_table($filter, $title)
{
   $col_list = array("id",    "Filename", "Start Time",              "Duration"  );
   $row_list =       "gm.id,   filename,  dt_start,       SEC_TO_TIME(duration)";

   $sql = "SELECT gm.id, filename, dt_start, dt_end, duration FROM gm ";

   if ($filter > 0)
   {
      $sql .= "LEFT JOIN gm_sessions ON gm.id = gm_sessions.gm_id ";
      $sql .= "WHERE gm_sessions.session_id=$filter";
   }
   if ($filter == -1)
   {
      $sql .= "LEFT JOIN gm_sessions ON gm.id = gm_sessions.gm_id ";
      $sql .= "WHERE gm_sessions.gm_id IS NULL";
   }

   echo "<div id=\"gm_table\"  class=\"div-section-container\">";
      echo "<div id=\"gm_table\"  class=\"div-section-title-frame\">$title</div>";
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
               $res = mysqli_query($GLOBALS['conn'], $sql);
               if ($res->num_rows == 0) echo "<tr><td id='md_center'; > not found </td></tr>";
               while ($row = $res->fetch_assoc())
               {
                  echo "<tr>";
                  foreach($row as $col => $val)
                  {
                     if ($col == "filename")
                     {
                        $filename = $val;  
                        $fullpath = "/downloads/$filename";
                        echo "<td id='md_left'; ><a href=\"$fullpath\" download=\"$filename\">$filename</a></td>";
                     }
                     else if (!($col == "dt_end")) echo "<td id='md_center'; >" . $val . "</td>";
                  }
                  echo "</tr>";
               }
            echo "</tbody>";
         echo "</table>";
      echo "</div>";
   echo "</div>";
}
?>



