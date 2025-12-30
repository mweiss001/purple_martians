<!DOCTYPE html>
<html>

<head>
   <table>
      <tbody>
         <tr>
            <td style='padding:2px 8px;' > <a href="sessions.php"      >Main</a> </td>
            <td style='padding:2px 8px;' > <a href="sessions_plr.php"  >Player</a> </td>
            <td style='padding:2px 8px;' > <a href="sessions_bnd.php"  >Bandwidth</a></td>
            <td style='padding:2px 8px;' > <a href="sessions_all.php"  >All</a> </td>
            <td style='padding:2px 12px;'>  </td>
            <td style='padding:2px 8px;' > <a href="../index.html"     >Home</a> </td>
         </tr>
      </tbody>
   </table>


   <script src="../assets/js/jquery-3.7.1.slim.min.js"></script>
   <link rel="stylesheet" href="../assets/css/datatables.min.css" />
   <script src="../assets/js/datatables.min.js"></script>
   <script>$(document).ready( function ()
   {
      $('#myTable').DataTable(
      {
         layout: {  topStart: { buttons: ['colvis'] }, bottomStart: 'pageLength' },
         colReorder: true,
         columnDefs: [ {  className: 'dt-head-center dt-body-center', targets: [0, 1, 2, 3, 4, 5, 6]  } ],
         order: [  [0, 'desc'] ]
      }  );
   } );</script>
</head>


<body>


<?php

   $srvrname = "localhost";
   $database = "pm";
   $username = "pmdb_ro";
   $password = "readonly";

   // Create a connection
   $conn = mysqli_connect($srvrname, $username, $password, $database);

   // Check the connection
   if (!$conn) die("Connection failed: " . mysqli_connect_error());
   // echo "Connected to database\n";

   echo "<hr><h2>Sessions - Main</h2><hr>";

   $col_list = array("id", "Date",   "ip", "port", "host",     "endreason",            "duration", "cdat");
   $row_list =       "id,   dt_start, ip,   port,   hostname,   endreason,  SEC_TO_TIME(duration),  cdat_rx";

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
               if ($col == "id") echo "<td><a href=\"sessions.php?data=" . $val . "\">" . $val . "</a></td>"; 
               else echo "<td>" . $val . "</td>";
            }
            echo "</tr>";
         }
      echo "</tbody>";
   echo "</table>";

   echo "<hr>";

  include('fill.php');

?>

</body>
</html>
