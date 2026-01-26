
<?php

// database setup
$db_filepath = "/home/m/dev/purple_martians/data/sessions.db";
if (!file_exists($db_filepath)) { echo "Database file: $filename not found."; return; }
$db = new PDO("sqlite:/home/m/dev/purple_martians/data/sessions.db");

function secondsToHMS($seconds)
{
   $h = floor($seconds / 3600);
   $m = floor(($seconds % 3600) / 60);
   $s = $seconds % 60;
   return sprintf ("%02d:%02d:%02d", $h, $m, $s);
}






?>




