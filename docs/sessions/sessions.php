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
         layout: { topStart: { buttons: ['colvis'] } },
         colReorder: true,
         columnDefs: [ {  className: 'dt-head-center dt-body-center', targets: [0, 1, 2, 3, 4, 5, 6]  } ]
      }  );
   } );</script>

</head>
<body>
<?php

   $srvrname = "localhost";
   $database = "session_db";
   $username = "php_ro";
   $password = "readonly";

   $col_list = array("Start Time", "IP address", "port", "hostname", "Session End Reason", "duration",            "cdat");
   $row_list =      "timestamp,     ip,           port,   hostname,   endreason,            SEC_TO_TIME(duration), cdat_rx";

   // Create a connection
   $conn = mysqli_connect($srvrname, $username, $password, $database);

   // Check the connection
   if (!$conn) die("Connection failed: " . mysqli_connect_error());
   // echo "Connected to database\n";

   echo "<hr><h2>Sessions - Main</h2><hr>";

   echo "<table>";
      echo "<tbody>";
         echo "<tr>";
            echo "<td style='padding:8px; border: 1px solid black;'> <a href=\"sessions.php\"       target=\"_top\">Main</a> </td>";
            echo "<td style='padding:8px; border: 1px solid black;'> <a href=\"sessions_plr.php\"   target=\"_top\">Player</a> </td>";
            echo "<td style='padding:8px; border: 1px solid black;'> <a href=\"sessions_bnd.php\"   target=\"_top\">Bandwidth</a></td>";
            echo "<td style='padding:8px; border: 1px solid black;'> <a href=\"sessions_all.php\"   target=\"_top\">All</a> </td>";
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
