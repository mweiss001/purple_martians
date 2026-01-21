<?php
header('Content-Type: application/json');

// convert sql date to java format
function getJDate($d)
{
   $dt = date('U', strtotime($d)) * 1000; 
   $st = "$dt";
   return $dt;
}

// database setup
$db_filepath = "/home/m/dev/purple_martians/data/sessions.db";
if (!file_exists($db_filepath))
{
   echo "Database file: $filename not found.";
   return;
}
$db = new PDO("sqlite:/home/m/dev/purple_martians/data/sessions.db");

$data = array();

// get game moves
$sql = "SELECT gm.muid, dt_start, dt_end, level, duration, filename FROM gm ";
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
$sql = "SELECT id, dt_start, dt_end, player_name, player_color FROM sessions";
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


