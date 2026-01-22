<?php

//session_start();

$col_list = array();
$row_list;
$cla;

function setup_columns()
{
   global $col_list;
   global $row_list;
   global $cla;


   $col_set = 0; // default
   if (isset($_SESSION['col_set'])) $col_set = $_SESSION['col_set'];

   $col_list_basic = array("id", "Player",     "player_name", "Start Time",                                                       "duration", "ip", "host",     "Session End Reason", "cdat"    );
   $row_list_basic =       "id,   player_color, player_name,   STRFTIME('%Y-%m-%d %H:%M:%S', dt_start, 'localtime') AS dt_start,   duration,  ip,   hostname,   endreason,            cdat_rx";


   $col_list_basic2 = array("id", "Date",     "ip", "port", "host",     "endreason", "duration",  "cdat");
   $row_list_basic2 =       "id,   dt_start,   ip,   port,   hostname,   endreason,   duration,   cdat_rx";

   $col_list_player = array("player number", "next levels", "exits activated", "respawns", "shots fired", "enemy hits", "player hits", "self hits", "purple coins");
   $row_list_player =       "player_num,      next_levels,   exits_activated,   respawns,   shots_fired,   enemy_hits,   player_hits,   self_hits,   purple_coins";

   $col_list_bandwidth_byte = array ("tx bytes total", "tx bytes avg per sec", "tx bytes max per frame", "rx bytes total", "rx bytes avg per sec", "rx bytes max per frame");
   $row_list_bandwidth_byte =        "tx_bytes_total,   tx_bytes_avg_per_sec,   tx_bytes_max_per_frame,   rx_bytes_total,   rx_bytes_avg_per_sec,   rx_bytes_max_per_frame";

   $col_list_bandwidth_pack = array ("tx packets total", "tx packets avg per sec", "tx packets max per frame", "rx packets total", "rx packets avg per sec", "rx packets max per frame");
   $row_list_bandwidth_pack =        "tx_packets_total,   tx_packets_avg_per_sec,   tx_packets_max_per_frame,   rx_packets_total,   rx_packets_avg_per_sec,   rx_packets_max_per_frame";

   $col_list_bandwidth = array_merge($col_list_bandwidth_byte,         $col_list_bandwidth_pack);
   $row_list_bandwidth =             $row_list_bandwidth_byte . ", " . $row_list_bandwidth_pack;

   if ($col_set == 0) // default
   {
      $col_list = $col_list_basic;
      $row_list = $row_list_basic;
   }

   if ($col_set == 1) // player
   {
      $col_list = array_merge($col_list_basic,         $col_list_player);
      $row_list =             $row_list_basic . ", " . $row_list_player;
   }

   if ($col_set == 2) // bandwidth
   {
      $col_list = array_merge($col_list_basic,         $col_list_bandwidth);
      $row_list =             $row_list_basic . ", " . $row_list_bandwidth;
   }

   if ($col_set == 3) // all
   {
      $col_list = array_merge($col_list_basic2,         $col_list_player,        $col_list_bandwidth);
      $row_list =             $row_list_basic2 . ", " . $row_list_player. ", " . $row_list_bandwidth;
   }

      // make a list of column numbers to pass to table after 'targets: '
      //columnDefs: [ {  className: 'dt-head-center dt-body-center', targets: [0, 1, 2, 3, 4, 5, 6]  } ],
      $cla = "[ ";
      for ($i = 0; $i < (count($col_list)-1)  ; $i++) $cla .= "$i, ";
      $cla .= "]";
      //var_dump($cla);




 
}   




function show_sessions_table()
{
   global $col_list;
   global $row_list;
   global $cla;

   $but = "class=\"button\" id=\"sessions_button\"";

   echo "<div id=\"sessions\"  class=\"div-section-container\">";


      echo "<div id=\"sessions\"  class=\"div-section-title-section-frame\">";
         echo "<div id=\"sessions\"  class=\"div-section-title-section-container\">";
            echo "<div id=\"sessions\"  class=\"div-section-title-frame\">Sessions</div>";
            echo "<div id=\"sessions\"  class=\"div-section-title-frame-buttons-frame\">";
               echo "<a href=\"sessions.php?col_set=0\" $but >Default Columns   </a>";
               echo "<a href=\"sessions.php?col_set=1\" $but >Player Columns    </a>";
               echo "<a href=\"sessions.php?col_set=2\" $but >Bandwidth Columns </a>";
               echo "<a href=\"sessions.php?col_set=3\" $but >All Columns       </a>";
            echo "</div>";
         echo "</div>";
      echo "</div>";


      setup_columns();

      // setup the table
      echo "<script>$(document).ready( function ()
      {
         $('#myTable').DataTable(
         {
            layout: {  topStart: { buttons: ['colvis'] }, bottomStart: 'pageLength' },
            colReorder: true,
            columnDefs: [ {  className: 'dt-head-center dt-body-center', targets: $cla } ],
            order: [  [0, 'desc'] ]
         }  );
      } );
      </script> ";

      echo "<div id=\"sessions\"  class=\"div-section-sub-section-table\">";
         echo "<table id='myTable' class='display cell-border compact'  style='width:100%'>";
            echo "<thead id=\"sessions_table_head\">";
               echo "<tr>";
                  foreach($col_list as $col)
                  {
                     if ($col == "player_name") continue;
                     echo "<th>$col</th>";
                  }
               echo "</tr>";
            echo "</thead align='center'>";
            echo "<tbody>";
               $sql = "SELECT $row_list FROM sessions";
               $res = $GLOBALS['db']->query($sql);
               while ($row = $res->fetch(PDO::FETCH_ASSOC))
               {
                  echo "<tr  id=\"sessions_table_row\">";
                  foreach($row as $col => $val)
                  {
                     $id = $row['id'];

                     if ($col == "player_name") continue;
                     if ($col == "id")
                     {
                        echo "<td><a href=\"sessions.php?current_session_id=" . $val . "\">" . $val . "</a></td>"; 
                        continue;
                     }
                     if ($col == "player_color")
                     {
                        $iconpath = "/assets/icons/player_icon_$val.png";
                        $alt = "alt=\"icon not found\"";
                        $name = $row['player_name'];
                        echo "<td><a href=\"sessions.php?current_session_id=$id\"><div class=\"icon-text-container\">";

                        echo "<img src=$iconpath $alt class=\"icon\">";
                        echo "<span class=\"text\">$name</span>";

                        echo "</div></a></td>";
                        continue;
                     }
                     if ($col == "duration")
                     {
                        echo "<td>" . secondsToHMS($val) . "</td>";
                        continue;
                     }
                                  
                                  
                                  
                     echo "<td>$val</td>";
                  }
                  echo "</tr>";
               }
            echo "</tbody>";
         echo "</table>";

      echo "</div>";
   echo "</div>";
}
?>
