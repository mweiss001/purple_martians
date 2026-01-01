<!DOCTYPE html>
<html>

<head>
   <table>
      <tbody>
         <tr>
            <td style='padding:2px 8px;' > <a href="../index.html"  >Back to Main Page        </a> </td>
         </tr>
      </tbody>
   </table>
   <hr>
</head>

<body>





<?php

session_start();

echo <<<EOT
<div style="background-color: #edf6f7;">
   <hr><h1>Sessions</h1><hr>

   <table>
      <tbody>
         <tr>
            <td style='padding:2px 8px;' > <a href="sessions.php?col_set=0">Default Columns   </a> </td>
            <td style='padding:2px 8px;' > <a href="sessions.php?col_set=1">Player Columns    </a> </td>
            <td style='padding:2px 8px;' > <a href="sessions.php?col_set=2">Bandwidth Columns </a> </td>
            <td style='padding:2px 8px;' > <a href="sessions.php?col_set=3">All Columns       </a> </td>
         </tr>
      </tbody>
   </table>
   <hr>
EOT;


include("show_gm_table.php");
include('show_current_session.php');

// if passed as query string set the session variable
if (isset($_GET['current_session_id'])) $_SESSION['current_session_id'] = $_GET['current_session_id'];
if (isset($_GET['col_set'])) $_SESSION['col_set'] = $_GET['col_set'];


$srvrname = "localhost";
$database = "pm";
$username = "pmdb_ro";
$password = "readonly";
$conn = mysqli_connect($srvrname, $username, $password, $database);
if (!$conn) die("Connection failed: " . mysqli_connect_error());


include('show_sessions_table.php');

echo "<hr></div>";



if(!isset($_SESSION['current_session_id'])) echo "<hr>no current session set<hr>";
else
{

   echo '<div style="background-color: #daebe1;">';
   $current_session_id = $_SESSION['current_session_id'];
   show_current_session($current_session_id);
   echo "</div>";



   echo '<div style="background-color: #f1edf7;"><hr>';
   echo "<p><h3>Save Game Files for Current Session</h3></p><hr>";
   show_gm_table($current_session_id);
   echo "<hr></div>";


   echo '<div style="background-color: #f1edf7;"><hr>';
   echo "<p><h3>All Save Game Files</h3></p><hr>";
   show_gm_table(0);
   echo "<hr></div>";

   echo '<div style="background-color: #f1edf7;"><hr>';
   echo "<p><h3>Orphaned Save Game Files (no matching session)</h3></p><hr>";
   show_gm_table(-1);
   echo "<hr></div>";







}






//   include('show_all_gm.php');


//   include('show_orphan_gm.php');


//   include('show_orphan_session.php');


?>

</body>
</html>
