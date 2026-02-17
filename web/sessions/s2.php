<?php
header('Content-Type: application/json');

// database setup
$db_filepath = "/home/m/dev/purple_martians/data/status.db";
if (!file_exists($db_filepath)) { echo "Database file: $filename not found."; return; }
$db = new PDO("sqlite:/home/m/dev/purple_martians/data/status.db");





// get 40 most recent times
$ids = array(); 
$sql = "SELECT DISTINCT(ss_id) FROM client_status ORDER BY ss_id DESC LIMIT 40";
$res = $db->query($sql);
while ($row = $res->fetch(PDO::FETCH_ASSOC)) $ids[] = $row['ss_id'];
//var_dump($ids);

/*

$ids = [3503, 3502, 3501, 3500];

$ids = [];

for ($i = 3500; $i < 3800; $i++)
{
   $ids[] = $i;
}

*/

$data = array();
foreach($ids as $id)
{
   $sql = "SELECT * FROM client_status WHERE ss_id=$id ORDER BY pl_num";
   $res = $db->query($sql);
   while ($row = $res->fetch(PDO::FETCH_ASSOC)) $data[] = $row;
}
//var_dump($data);
echo json_encode($data);
?>
