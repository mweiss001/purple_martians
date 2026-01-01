<?php session_start(); ?>

<script src="../assets/js/jquery-3.7.1.slim.min.js"></script>
<link rel="stylesheet" href="../assets/css/datatables.min.css" />
<script src="../assets/js/datatables.min.js"></script>

<?php

$col_set = 0; // default
if (isset($_SESSION['col_set'])) $col_set = $_SESSION['col_set'];

$col_list_basic = array("id", "Start Time",         "duration", "ip", "host",     "Session End Reason", "cdat"    );
$row_list_basic =       "id,   dt_start, SEC_TO_TIME(duration),  ip,   hostname,   endreason,            cdat_rx";

$col_list_basic2 = array("id", "Date",     "ip", "port", "host",     "endreason",            "duration",  "cdat");
$row_list_basic2 =       "id,   dt_start,   ip,   port,   hostname,   endreason,  SEC_TO_TIME(duration),   cdat_rx";

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


// make a list of column number to pass to table
//columnDefs: [ {  className: 'dt-head-center dt-body-center', targets: [0, 1, 2, 3, 4, 5, 6]  } ],
$cla = "[ ";
for ($i = 0; $i < count($col_list); $i++)
   $cla .= "$i, ";
$cla .= "]";
//var_dump($cla);

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



   echo "<table id='myTable' class='display cell-border compact' style='width:90%'>";
      echo "<thead>";
         echo "<tr>";
            foreach($col_list as $col)
               echo "<th>$col</th>";
         echo "</tr>";
      echo "</thead align='center'>";
      echo "<tbody>";
         $sql = "SELECT $row_list FROM sessions";
         $res = mysqli_query($conn, $sql);
         while ($row = $res->fetch_assoc())
         {
            echo "<tr>";
            foreach($row as $col => $val)
            {
               if ($col == "id") echo "<td><a href=\"sessions.php?current_session_id=" . $val . "\">" . $val . "</a></td>"; 
               else echo "<td>" . $val . "</td>";
            }
            echo "</tr>";
         }
      echo "</tbody>";
   echo "</table>";

?>
