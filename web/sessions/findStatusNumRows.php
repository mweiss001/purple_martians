<?php
header('Content-Type: application/json');

// database setup
$db_filepath = "/home/m/dev/purple_martians/data/status.db";
if (!file_exists($db_filepath)) { echo "Database file: $db_filepath not found."; return; }
$db = new PDO("sqlite:$db_filepath");

$res = $db->query("SELECT COUNT(*) FROM client_status");
if ($row = $res->fetch(PDO::FETCH_ASSOC)) $num_rows = $row['COUNT(*)'];

echo json_encode($num_rows);

?>


