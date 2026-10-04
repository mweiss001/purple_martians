<?php
header('Content-Type: application/json');

// database setup
$db_filepath = "/home/m/dev/purple_martians/data/server_status.db";
if (!file_exists($db_filepath)) { echo "Database file: $db_filepath not found.\n"; return; }
$db = new PDO("sqlite:$db_filepath");

$key = 'unset';
$val = 0;
$mod = 0;

if (isset($_GET['key'])) $key = $_GET['key'];
if (isset($_GET['val'])) $val = $_GET['val'];
if (isset($_GET['mod'])) $mod = $_GET['mod'];

$sql = "INSERT INTO control VALUES(NULL, '$key', $val, $mod)";
$db->exec($sql);

exit();

?>

