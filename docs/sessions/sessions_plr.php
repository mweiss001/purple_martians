<!DOCTYPE html>
<html>
<head>
   <script src="DataTables/jquery-3.7.1.min.js"></script>
   <link rel="stylesheet" href="/DataTables/datatables.css" />
   <script src="/DataTables/datatables.js"></script>
   <script>$(document).ready( function ()
   {
      $('#myTable').DataTable(
      {
         layout: { topStart: { buttons: ['colvis'] } },
         colReorder: true,
         columnDefs: [ {  className: 'dt-head-center dt-body-center', targets: [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15] } ]
      }  );
   } );</script>

</head>
<body>
<?php

   $srvrname = "localhost";
   $database = "session_db";
   $username = "php_ro";
   $password = "readonly";

   $col_list = array("Start Time", "IP address", "port", "hostname", "Session End Reason", "duration",            "cdat",  "player_num", "next_levels", "exits_activated", "respawns", "shots_fired", "enemy_hits", "player_hits", "self_hits", "purple_coins");
   $row_list =       "timestamp,    ip,           port,   hostname,   endreason,            SEC_TO_TIME(duration), cdat_rx, player_num,   next_levels,   exits_activated,   respawns,   shots_fired,   enemy_hits,   player_hits,   self_hits,   purple_coins";

   // Create a connection
   $conn = mysqli_connect($srvrname, $username, $password, $database);

   // Check the connection
   if (!$conn) die("Connection failed: " . mysqli_connect_error());
   // echo "Connected to database\n";

   echo "<hr><h2>Sessions (player) </h2><hr>";

   echo "<table id='myTable' class='display cell-border compact' style='width:90%'>";
      echo "<thead>";
         echo "<tr>";
            foreach($col_list as $col)
               echo "<th>$col</th>";
         echo "</tr>";
      echo "</thead align='center'>";
      echo "<tbody>";
         $sql = "SELECT $row_list FROM session_tb";
         $res = mysqli_query($conn, $sql);
         while ($row = $res->fetch_assoc())
         {
            echo "<tr>";
            foreach($row as $col => $val)
               echo "<td>" . $val . "</td>";
            echo "</tr>";
         }
      echo "</tbody>";
   echo "</table>";
?>
<hr>
</body>
</html>
