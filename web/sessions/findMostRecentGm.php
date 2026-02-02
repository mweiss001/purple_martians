<?php
include 'database.php';
$sql = "SELECT dt_start, muid FROM gm ORDER BY dt_start DESC LIMIT 1";
$res = $db->query($sql);
if ($row = $res->fetch(PDO::FETCH_ASSOC)) echo $row['muid'];
?>



