<?php
header('Content-Type: application/json');

// database setup
$db_filepath = "/home/m/dev/purple_martians/data/client_status.db";
if (!file_exists($db_filepath)) { echo "Database file: $db_filepath not found.\n"; return; }
$db = new PDO("sqlite:$db_filepath");

// how many records
$res = $db->query("SELECT COUNT(*) FROM client_status");
if ($row = $res->fetch(PDO::FETCH_ASSOC)) $num_rows = $row['COUNT(*)'];

// get 10% 
$limit = (int) ($num_rows / 10);

$sql = "DELETE FROM client_status WHERE id IN (SELECT id FROM client_status ORDER BY id ASC LIMIT $limit)";
$db->exec($sql);
echo json_encode($num_rows-$limit);
?>

