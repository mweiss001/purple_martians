<?php
include 'database.php';
$sql = "SELECT id FROM sessions ORDER BY id DESC LIMIT 1";
$res = $db->query($sql);
if ($row = $res->fetch(PDO::FETCH_ASSOC)) echo $row['id'];
?>



