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
         columnDefs: [ {  className: 'dt-head-center dt-body-center', targets: [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29] } ]
      }  );
   } );</script>

</head>
<body>
<?php

   $srvrname = "localhost";
   $database = "session_db";
   $username = "php_ro";
   $password = "readonly";

   // Create a connection
   $conn = mysqli_connect($srvrname, $username, $password, $database);

   // Check the connection
   if (!$conn) die("Connection failed: " . mysqli_connect_error());
   // echo "Connected to database\n";

   echo "<hr><h2>Sessions (all columns)</h2><hr>";

   echo "<table id='myTable' class='display cell-border compact' style='width:95%'>";
      echo "<thead>";
         echo "<tr>";
            $sql = "SELECT * FROM session_tb";
            $res = mysqli_query($conn, $sql);
            $row = $res->fetch_assoc();
            foreach($row as $col => $val)
               echo "<th>" . $col . "</th>";
         echo "</tr>";
      echo "</thead>";
      echo "<tbody>";
         $sql = "SELECT * FROM session_tb";
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
