<?php
header('Content-Type: application/json');

// database setup
$db_filepath = "/home/m/dev/purple_martians/data/status.db";
if (!file_exists($db_filepath)) { echo "Database file: $filename not found."; return; }
$db = new PDO("sqlite:/home/m/dev/purple_martians/data/status.db");

$max_records = 3200;

$start_ssid = -40;
if (isset($_GET['start_ssid'])) $start_ssid = $_GET['start_ssid'];

// if start_ssid < 0 then get last x values
if ($start_ssid < 0)
{
   // find last record
   $res = $db->query("SELECT MAX(ss_id) FROM client_status");
   if ($row = $res->fetch(PDO::FETCH_ASSOC)) $last_ssid = $row['MAX(ss_id)'];
   $start_ssid = $last_ssid + $start_ssid;
}

$data = array();
$sql = "SELECT * FROM client_status WHERE ss_id>$start_ssid ORDER BY ss_id LIMIT $max_records";
$res = $db->query($sql);
while ($row = $res->fetch(PDO::FETCH_ASSOC)) $data[] = $row;

echo json_encode($data);

?>


