<?php
header('Content-Type: application/json');



if (isset($argv)) {
    // Combine all arguments after the script name into a single string
    $queryString = implode('&', array_slice($argv, 1));
    // Parse the string into the $_GET superglobal
    parse_str($queryString, $_GET);
}



$sql_where = "";
$range = 'all';
$min = 3600;

if (isset($_GET['range'])) $range = $_GET['range'];
if (isset($_GET['min'])) $min = $_GET['min'];

if ($range == 'min')
{
   $d2 = new DateTimeImmutable("now", new DateTimeZone("UTC"));
   $d1 = $d2->modify("-$min minutes");
   $dt_start = $d1->format('Y-m-d\TH:i:s');
   $dt_end   = $d2->format('Y-m-d\TH:i:s');
   $sql_where = " WHERE dt_start BETWEEN '$dt_start' AND '$dt_end'";  
}


// database setup
$db_filepath = "/home/m/dev/purple_martians/data/sessions.db";
if (!file_exists($db_filepath))
{
   echo "Database file: $filename not found.";
   return;
}
$db = new PDO("sqlite:/home/m/dev/purple_martians/data/sessions.db");


if ($range == 'session')
{
   // get start and end times from most recent session
   $res = $GLOBALS['db']->query("SELECT dt_start, dt_end FROM sessions ORDER by dt_start DESC LIMIT 1");
   if ($row = $res->fetch(PDO::FETCH_ASSOC))
   {
      $dt_start = $row['dt_start'];
      $dt_end = $row['dt_end'];
   }
   $sql_where = " WHERE dt_start BETWEEN '$dt_start' AND '$dt_end'";  
}




// convert sql date to java format
function getJDate($d)
{
   $dt = date('U', strtotime($d)) * 1000; 
   $st = "$dt";
   return $dt;
}


$data = array();



/*
$mode = 4;


   if ($mode == 0) $sql_where = "";  



   if ($mode == 1)
   {
      // get start and end times from current session
      if (isset($_SESSION['current_session_id'])) $current_session_id = $_SESSION['current_session_id'];
      else $current_session_id = 0;

      $res = $GLOBALS['db']->query("SELECT dt_start, dt_end FROM sessions WHERE id=$current_session_id");
      if ($row = $res->fetch(PDO::FETCH_ASSOC))
      {
         $dt_start = $row['dt_start'];
         $dt_end = $row['dt_end'];
      }
   }

   if ($mode == 2)
   {
      // get start and end times from current gm
      if (isset($_SESSION['current_gm_muid'])) $current_gm_muid = $_SESSION['current_gm_muid'];
      else $current_gm_muid = 0;

      $res = $GLOBALS['db']->query("SELECT dt_start, dt_end FROM gm WHERE muid='$current_gm_muid'");
      if ($row = $res->fetch(PDO::FETCH_ASSOC))
      {
         $dt_start = $row['dt_start'];
         $dt_end = $row['dt_end'];
      }
   }



   if ($mode == 3)
   {
      $d2 = new DateTimeImmutable("now");
      $d1 = $d2->modify('-1 hour');
      $dt_start = $d1->format('Y-m-d\TH:i:s');
      $dt_end   = $d2->format('Y-m-d\TH:i:s');
   }
   if ($mode == 4)
   {
      $d2 = new DateTimeImmutable("now");
      $d1 = $d2->modify('-2 day');
      $dt_start = $d1->format('Y-m-d\TH:i:s');
      $dt_end   = $d2->format('Y-m-d\TH:i:s');
   }


   if ($mode)
   {
//      $GLOBALS['tl_start']  = $dt_start;
//      $GLOBALS['tl_end']    = $dt_end;
      $sql_where = " WHERE dt_start BETWEEN '$dt_start' AND '$dt_end'";  
   }



echo ($sql_where);

*/



$ty = 0;

// get game moves
$sql = "SELECT gm.muid, dt_start, dt_end, level, duration, filename FROM gm $sql_where";
$res = $db->query($sql);
while ($row = $res->fetch(PDO::FETCH_ASSOC))
{
   $st     = getJDate($row['dt_start']);
   $en     = getJDate($row['dt_end']);
   $ty     =          0;                // all gm's will have type 0
   $cl     =          0;                // color
   $rn     =          'gm';             // row name
   $nm     =          $row['muid'];     // element name
   $lv     =          $row['level'];
   $data[] = [ 'value' => [$st, $en, $ty, $cl, $rn, $nm, $lv ] ] ;
}

// get sessions
$sql = "SELECT id, dt_start, dt_end, player_name, player_color FROM sessions $sql_where";
$res = $db->query($sql);
while ($row = $res->fetch(PDO::FETCH_ASSOC))
{
   $st     = getJDate($row['dt_start']);
   $en     = getJDate($row['dt_end']);
   $ty++;                                    // each session to have unique row
   $cl     =          $row['player_color'];  // color
   $rn     =          $row['player_name'];   // row name
   $nm     =          $row['player_name'];   // element name
   $id     =          $row['id'];
   $data[] = [ 'value' => [$st, $en, $ty, $cl, $rn, $nm, $id ] ] ;
}
   echo json_encode($data);
?>


