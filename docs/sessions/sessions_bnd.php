<!DOCTYPE html>
<html>
<head>
   <script src="../assets/js/jquery-3.7.1.min.js"></script>
   <link rel="stylesheet" href="../assets/css/datatables.css" />
   <script src="../assets/js/datatables.js"></script>
   <script>$(document).ready( function ()
   {
      $('#myTable').DataTable(
      {
         layout: {  topStart: { buttons: ['colvis'] }, bottomStart: 'pageLength' },
         colReorder: true,
         columnDefs: [ {  className: 'dt-head-center dt-body-center', targets: [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18] } ],
         order: [  [0, 'desc'] ]
      }  );
   } );</script>

</head>
<body>
<?php

   $srvrname = "localhost";
   $database = "session_db";
   $username = "php_ro";
   $password = "readonly";

   $col_list = array("Start Time", "IP address", "port", "hostname", "Session End Reason", "duration",            "cdat",  "tx_bytes_total", "tx_bytes_avg_per_sec", "tx_bytes_max_per_frame", "rx_bytes_total", "rx_bytes_avg_per_sec", "rx_bytes_max_per_frame", "tx_packets_total", "tx_packets_avg_per_sec", "tx_packets_max_per_frame", "rx_packets_total", "rx_packets_avg_per_sec", "rx_packets_max_per_frame");
   $row_list =       "timestamp,    ip,           port,   hostname,   endreason,            SEC_TO_TIME(duration), cdat_rx, tx_bytes_total,   tx_bytes_avg_per_sec,   tx_bytes_max_per_frame,   rx_bytes_total,   rx_bytes_avg_per_sec,   rx_bytes_max_per_frame,   tx_packets_total,   tx_packets_avg_per_sec,   tx_packets_max_per_frame,   rx_packets_total,   rx_packets_avg_per_sec,   rx_packets_max_per_frame";

//   $col_list .= ", tx_bytes_total", "tx_bytes_avg_per_sec", "tx_bytes_max_per_frame", "rx_bytes_total", "rx_bytes_avg_per_sec", "rx_bytes_max_per_frame";
//   $row_list .= ", tx_bytes_total,   tx_bytes_avg_per_sec,   tx_bytes_max_per_frame,   rx_bytes_total,   rx_bytes_avg_per_sec,   rx_bytes_max_per_frame";

//   $col_list .= ", tx_packets_total", "tx_packets_avg_per_sec", "tx_packets_max_per_frame", "rx_packets_total", "rx_packets_avg_per_sec", "rx_packets_max_per_frame";
//   $row_list .= ", tx_packets_total,   tx_packets_avg_per_sec,   tx_packets_max_per_frame,   rx_packets_total,   rx_packets_avg_per_sec,   rx_packets_max_per_frame";

   // Create a connection
   $conn = mysqli_connect($srvrname, $username, $password, $database);

   // Check the connection
   if (!$conn) die("Connection failed: " . mysqli_connect_error());
   // echo "Connected to database\n";

   echo "<hr><h2>Sessions - Bandwidth</h2><hr>";

   echo "<table>";
      echo "<tbody>";
         echo "<tr>";
            echo "<td style='padding:2px 8px;' > <a href=\"sessions.php\"      >Main</a> </td>";
            echo "<td style='padding:2px 8px;' > <a href=\"sessions_plr.php\"  >Player</a> </td>";
            echo "<td style='padding:2px 8px;' > <a href=\"sessions_bnd.php\"  >Bandwidth</a></td>";
            echo "<td style='padding:2px 8px;' > <a href=\"sessions_all.php\"  >All</a> </td>";
            echo "<td style='padding:2px 12px;'>  </td>";
            echo "<td style='padding:2px 8px;' > <a href=\"../index.html\"     >Home</a> </td>";
         echo "</tr>";
      echo "</tbody>";
   echo "</table>";

   echo "<hr>";




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
