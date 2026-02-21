
<?php

// database setup
$db_filepath = "/home/m/dev/purple_martians/data/sessions.db";
if (!file_exists($db_filepath)) { echo "Database file: $db_filepath not found."; return; }
$db = new PDO("sqlite:$db_filepath");




function secondsToHMS($seconds)
{
   $h = floor($seconds / 3600);
   $m = floor(($seconds % 3600) / 60);
   $s = $seconds % 60;
   return sprintf ("%02d:%02d:%02d", $h, $m, $s);
}

// always pads to 12 char, only shows units that are not zero
function secondsToDHMS($seconds)
{
   $d = floor($seconds / 86400);
   $h = floor(($seconds % 86400) / 3600);
   $m = floor(($seconds % 3600) / 60);
   $s = $seconds % 60;

   if ($d)      $ret = sprintf ("%d %02d:%02d:%02d", $d, $h, $m, $s);
   else if ($h) $ret = sprintf ("%02d:%02d:%02d", $h, $m, $s);
   else if ($m) $ret = sprintf ("%02d:%02d", $m, $s);
   else         $ret = sprintf (":%02d", $s);
   $ret = str_pad($ret, 12, " ", STR_PAD_LEFT);
   return $ret;
}

?>




