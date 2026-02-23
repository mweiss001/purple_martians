<?php
header('Content-Type: application/json');

// database setup
$db_filepath = "/home/m/dev/purple_martians/data/server_status.db";
if (!file_exists($db_filepath)) { echo "Database file: $db_filepath not found."; return; }
$db = new PDO("sqlite:$db_filepath");

$data = array();
$res = $db->query("SELECT * FROM status");
while ($row = $res->fetch(PDO::FETCH_ASSOC)) $data[] = $row;

echo json_encode($data);

?>


