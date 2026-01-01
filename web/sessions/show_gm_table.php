<?php
function show_gm_table($filter)
{
   $col_list = array("id",    "Filename", "Start Time",              "Duration"  );
   $row_list =       "gm.id,   filename,  dt_start,       SEC_TO_TIME(duration)";

   //$filter = 0;

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


   // echo "sql: $sql\n";


?>
    <style>

        table#mdw
        {
            border-collapse: collapse;
            width: 90%;
            margin: 12px 12px;
            font-family: sans-serif;
        }

        thead#mdw
        {
            background-color: #ded3f0; // nice faded purple
            color: black;
        }

        tr:hover
        {
           background-color: #ddd; /* Gray background when hovering over a row */
        }

        th#mh_center
        {
            border: 1px solid #ddd;
            padding: 10px;
            text-align: center;
        }

        th#mh_left
        {
            border: 1px solid #ddd;
            padding: 10px;
            text-align: left;
        }

        td#md_center
        {
            border: 1px solid #ddd;
            padding: 10px;
            text-align: center;
        }

        td#md_left
        {
            border: 1px solid #ddd;
            padding: 10px;
            text-align: left;
        }
    </style>


<?php



   echo "<table id='mdw'>";
      echo "<thead id='mdw'>";
         echo "<tr>";
            foreach($col_list as $col)
            {
               if ($col == "Filename") echo "<th  id='mh_left'>$col</th>";
               else echo "<th  id='mh_center'>$col</th>";
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
}
?>



