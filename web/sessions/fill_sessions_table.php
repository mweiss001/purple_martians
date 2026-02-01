<?php

include 'database.php';

function setup_columns()
{
   global $col_list;
   global $row_list;
   global $view;

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

   if ($view == 0) // default
   {
      $col_list = $col_list_basic;
      $row_list = $row_list_basic;
   }

   if ($view == 1) // player
   {
      $col_list = array_merge($col_list_basic,         $col_list_player);
      $row_list =             $row_list_basic . ", " . $row_list_player;
   }

   if ($view == 2) // bandwidth
   {
      $col_list = array_merge($col_list_basic,         $col_list_bandwidth);
      $row_list =             $row_list_basic . ", " . $row_list_bandwidth;
   }

   if ($view == 3) // all
   {
      $col_list = array_merge($col_list_basic2,         $col_list_player,        $col_list_bandwidth);
      $row_list =             $row_list_basic2 . ", " . $row_list_player. ", " . $row_list_bandwidth;
   }
}   


   echo '<div class="sessions_table div-section-container">';
      echo '<div class="sessions_table div-section-title-section-frame">';
         echo '<div class="sessions_table div-section-title-section-container">';
            echo '<div class="sessions_table div-section-title-frame">Sessions Table</div>';
            echo '<div class="sessions_table div-section-title-frame-buttons-frame">';

               echo '<div id="sessions_table-cont"></div>';

            echo '</div>';
         echo '</div>';
      echo '</div>';
      echo '<div class="sessions_table div-section-sub-section-table">';


$col_list = array();
$row_list;

$min = 0;
$view = 0;

if (isset($_GET['min']))  $min  = $_GET['min'];
if (isset($_GET['view'])) $view = $_GET['view'];

if (!$min)
{

   setup_columns();

      echo '<div class="sessions_table div-section-sub-section-table">';
         echo '<table id="myTable" class="display cell-border compact" style="width:100%">';
            echo '<thead class="sessions_table_head">';
               echo '<tr>';
                  foreach($col_list as $col)
                  {
                     if ($col == "player_name") continue;
                     echo "<th>$col</th>";
                  }
               echo '</tr>';
            echo '</thead>';
            echo '<tbody>';
               $sql = "SELECT $row_list FROM sessions";
               $res = $GLOBALS['db']->query($sql);
               while ($row = $res->fetch(PDO::FETCH_ASSOC))
               {
                  $id = $row['id'];
                  echo "<tr current_session_id=$id  class='sessions_table_row'>";
                  foreach($row as $col => $val)
                  {
                     if ($col == "player_name") continue;
                     if ($col == "player_color")
                     {
                        $iconpath = "/assets/icons/player_icon_$val.png";
                        $alt = "alt=\"icon not found\"";
                        $name = $row['player_name'];

                        echo "<td><div class=\"icon-text-container\">";
                        echo "<img src=$iconpath $alt class=\"icon\">";
                        echo "<span class=\"text\">$name</span>";

                        echo "</div></td>";
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
