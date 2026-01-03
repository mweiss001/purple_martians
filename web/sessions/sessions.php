<!DOCTYPE html>
<html class="dark">

<head>



<link rel="stylesheet" href="./sessions.css">

</head>


<body>

<!-- ---------- link bar at the very top of the page ----------  -->
<table><tbody><tr>
<td style='padding:2px 8px;' > <a href="../index.html"  >Back to Main Page        </a> </td>
</tr></tbody></table><hr>


<?php

session_start();

// if passed as query string set the session variables
if (isset($_GET['current_session_id'])) $_SESSION['current_session_id'] = $_GET['current_session_id'];
if (isset($_GET['col_set'])) $_SESSION['col_set'] = $_GET['col_set'];

// database setup
$srvrname = "localhost";
$database = "pm";
$username = "pmdb_ro";
$password = "readonly";
$conn = mysqli_connect($srvrname, $username, $password, $database);
if (!$conn) die("Connection failed: " . mysqli_connect_error());

echo "<div class=\"div-sessions-page\">";
   echo "<div id=\"sessions\"  class=\"div-section-container\">";
      echo "<div id=\"sessions\"  class=\"div-section-title-frame\">Sessions</div>";
      echo "<div id=\"sessions\"  class=\"div-section-sub-section-links\">";
         echo "<a href=\"sessions.php?col_set=0\">Default Columns   </a>";
         echo "<a href=\"sessions.php?col_set=1\">Player Columns    </a>";
         echo "<a href=\"sessions.php?col_set=2\">Bandwidth Columns </a>";
         echo "<a href=\"sessions.php?col_set=3\">All Columns       </a></div>";
      echo "<div id=\"sessions\"  class=\"div-section-sub-section-table\">";
         include('show_sessions_table.php');
      echo "</div>";
   echo "</div>";

   if(!isset($_SESSION['current_session_id'])) echo "<hr>no current session set<hr>";
   else
   {
      include('show_current_session.php');
      include("show_gm_table.php");


      echo "<div id=\"current_session\"  class=\"div-section-container\">";
      echo "<div id=\"current_session\"  class=\"div-section-title-frame\">Current Session</div>";
      $current_session_id = $_SESSION['current_session_id'];
      show_current_session($current_session_id);
      echo "</div>";

      show_gm_table($current_session_id, "Save Game Files for Current Session");
      show_gm_table(0,                   "All Save Game Files");
      show_gm_table(-1,                  "Orphaned Save Game Files (no matching session)");
   }

echo "</div>";
?>
</body>
</html>
