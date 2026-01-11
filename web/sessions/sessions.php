<?php

session_start();

echo <<<TEXT

<!DOCTYPE html>
<html class="dark">

<head>

<link rel="stylesheet" href="./sessions.css">
<script src="../assets/js/jquery-3.7.1.slim.min.js"></script>
<link rel="stylesheet" href="../assets/css/datatables.min.css" />
<script src="../assets/js/datatables.min.js"></script>

</head>

<body>
TEXT;

include('show_sessions_table.php');
include('show_current_session.php');
include("show_gm_table.php");
include('show_current_gm.php');

// if passed as query string set the session variables
if (isset($_GET['current_session_id']))   $_SESSION['current_session_id']  = $_GET['current_session_id'];
if (isset($_GET['current_gm_muid']))      $_SESSION['current_gm_muid']     = $_GET['current_gm_muid'];


if (isset($_GET['col_set']))              $_SESSION['col_set']             = $_GET['col_set'];
if (isset($_GET['bnd_on']))               $_SESSION['bnd_on']              = $_GET['bnd_on'];
if (isset($_GET['plr_on']))               $_SESSION['plr_on']              = $_GET['plr_on'];
if (isset($_GET['gm_on']))                $_SESSION['gm_on']               = $_GET['gm_on'];
if (isset($_GET['gm_set']))               $_SESSION['gm_set']              = $_GET['gm_set'];

// set global variables
$current_session_id = 0;
if (isset($_SESSION['current_session_id'])) $current_session_id = $_SESSION['current_session_id'];
$current_gm_muid = 0;
if (isset($_SESSION['current_gm_muid']))    $current_gm_muid    = $_SESSION['current_gm_muid'];
$plr_on = 1;
if (isset($_SESSION['plr_on']))             $plr_on             = $_SESSION['plr_on'];
$bnd_on = 0;
if (isset($_SESSION['bnd_on']))             $bnd_on             = $_SESSION['bnd_on'];
$gm_set = 1;
if (isset($_SESSION['gm_set']))             $gm_set             = $_SESSION['gm_set'];
$gm_on = 1;
if (isset($_SESSION['gm_on']))              $gm_on              = $_SESSION['gm_on'];


// database setup
$db_filepath = "/home/m/dev/purple_martians/data/sessions.db";
if (!file_exists($db_filepath))
{
   echo "Database file: $filename not found.";
   return;
}
$db = new PDO("sqlite:/home/m/dev/purple_martians/data/sessions.db");



echo "<div class=\"div-sessions-page\">";

$but = "class=\"button\" id=\"links_button\"";
   echo "<div class=\"div-sessions-page-links\">";
   echo "<a href=\"../index.html\" $but >Back to Main Page   </a>";
   echo "<a href=\"sessions.php\"  $but >Sessions    </a>";
   echo "<a href=\"sessions.php\"  $but >Saved Game   </a>";
   echo "</div>";


   show_sessions_table();
   
   if(!isset($_SESSION['current_session_id']))
   {
      $res = $GLOBALS['db']->query("SELECT COUNT(*) FROM sessions");
      if ($res->fetchColumn() == 0)  { echo " no sessions found \n";  return; } 
      else
      {         
         $res = $GLOBALS['db']->query("SELECT MAX(id) FROM sessions");
         $_SESSION['current_session_id'] = $current_session_id = $res->fetchColumn();
      }
   }
   if(!isset($_SESSION['current_session_id'])) echo "<hr><h3>no current session set</h3><hr>";
   else
   {
      show_current_session();
      show_gm_table();
      show_current_gm();
   }

echo "</div>";

?>

</body></html>
