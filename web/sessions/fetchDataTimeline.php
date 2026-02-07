<?php
header('Content-Type: application/json');
include 'database.php';

// for command line testing, pass args on commnd line with spaces 
// m@o3000:/var/www/html/sessions$ php fetch_data_timeline.php range=session val=11
/*
if (isset($argv)) {
    // Combine all arguments after the script name into a single string
    $queryString = implode('&', array_slice($argv, 1));
    // Parse the string into the $_GET superglobal
    parse_str($queryString, $_GET);
}
*/


$sqlWhere = "";
$range = 'all';
$val = 60;

$dt_start;
$dt_end;


if (isset($_GET['range'])) $range = $_GET['range'];
if (isset($_GET['val']))   $val   = $_GET['val'];

if ($range == 'last')
{
   $d2 = new DateTimeImmutable("now", new DateTimeZone("UTC"));
   $d1 = $d2->modify("-$val minutes");
   $dt_start = $d1->format('Y-m-d\TH:i:s');
   $dt_end   = $d2->format('Y-m-d\TH:i:s');
   $sqlWhere = " WHERE (dt_start < '$dt_end') AND (dt_end > '$dt_start') ";  
}

if ($range == 'session')
{
   // get start and end times
   $res = $GLOBALS['db']->query("SELECT dt_start, dt_end FROM sessions WHERE id=" . $val);

   if ($row = $res->fetch(PDO::FETCH_ASSOC))
   {
      $dt_start = $row['dt_start'];
      $dt_end = $row['dt_end'];
   }
   $sqlWhere = " WHERE (dt_start < '$dt_end') AND (dt_end > '$dt_start') ";  
}

// convert sql date to java format
function getJDate($d)
{
   $dt = date('U', strtotime($d)) * 1000; 
   $st = "$dt";
   return $dt;
}

$data = array();
$ty = 0;

/*
// enter fake marker at range start
$dt = getJDate($dt_start);
$data[] = [ 'value' => [$dt, $dt, 0, 0, 'fk', '', '', '', 0, 0 ] ] ;
*/

// get game moves
$sql = "SELECT gm.muid, dt_start, dt_end, level, duration, filename FROM gm $sqlWhere";
$res = $db->query($sql);
while ($row = $res->fetch(PDO::FETCH_ASSOC))
{
   $st     = getJDate($row['dt_start']);
   $en     = getJDate($row['dt_end']);
   $ty     =  0;                // all gm's will have type 0
   $cl     =  0;                // color
   $rn     =  'gm';             // row name
   $nm     =  $row['muid'];     // element name
   $lv     =  $row['level'];    // level number 
   $fn     =  $row['filename']; // filename
   $data[] = [ 'value' => [$st, $en, $ty, $cl, $rn, $nm, $lv, $fn, 0, 0 ] ] ;
}

// get sessions
$sql = "SELECT id, dt_start, dt_end, player_name, player_color, hostname FROM sessions $sqlWhere";
$res = $db->query($sql);
while ($row = $res->fetch(PDO::FETCH_ASSOC))
{
   $st     =  getJDate($row['dt_start']);
   $en     =  getJDate($row['dt_end']);
   $ty++;                            // each session to have unique row
   $cl     =  $row['player_color'];  // color
   $rn     =  $row['player_name'];   // row name
   $nm     =  $row['player_name'];   // element name
   $id     =  $row['id'];            // session id
   $hn     =  $row['hostname'];      // hostame
   $data[] = [ 'value' => [$st, $en, $ty, $cl, $rn, $nm, $id, $hn, 0, 0 ] ] ;
}


// enter fake marker at range end
//$dt     = getJDate($dt_end);
//$data[] = [ 'value' => [$dt, $dt, 0, 0, 'fk', '', '', '', 0, 0 ] ] ;





echo json_encode($data);
?>


