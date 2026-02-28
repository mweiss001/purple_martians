<?php
header('Content-Type: application/json');

// database setup
$db_filepath = "/home/m/dev/purple_martians/data/client_status.db";
if (!file_exists($db_filepath)) { echo "Database file: $db_filepath not found."; return; }
$db = new PDO("sqlite:$db_filepath");


$max_records = 3200;

$start_timestamp = -40000;
if (isset($_GET['start_timestamp'])) $start_timestamp = $_GET['start_timestamp'];

// if start_timestamp < 0 then subtract from last timestamp
if ($start_timestamp < 0)
{
   // find last_timestamp
   $res = $db->query("SELECT MAX(timestamp) FROM client_status");
   if ($row = $res->fetch(PDO::FETCH_ASSOC)) $last_timestamp = $row['MAX(timestamp)'];
   $start_timestamp = $last_timestamp + $start_timestamp;
}

$data = array();
$sql = "SELECT * FROM client_status WHERE timestamp>=$start_timestamp ORDER BY timestamp LIMIT $max_records";
$res = $db->query($sql);
while ($row = $res->fetch(PDO::FETCH_ASSOC)) $data[] = $row;

echo json_encode($data);

?>


