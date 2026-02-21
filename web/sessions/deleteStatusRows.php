<?php
header('Content-Type: application/json');

// database setup
$db_filepath = "/home/m/dev/purple_martians/data/status.db";
if (!file_exists($db_filepath)) { echo "Database file: $filename not found."; return; }
$db = new PDO("sqlite:/home/m/dev/purple_martians/data/status.db");



// how many records
$res = $db->query("SELECT COUNT(*) FROM client_status");
if ($row = $res->fetch(PDO::FETCH_ASSOC)) $num_rows = $row['COUNT(*)'];
$limit = (int) ($num_rows / 10);

$sql = "DELETE FROM client_status WHERE id IN (SELECT id FROM client_status ORDER BY id ASC LIMIT $limit)";
//echo $sql;

$db->exec($sql);

echo json_encode($limit);

//echo json_encode($sql);


?>


