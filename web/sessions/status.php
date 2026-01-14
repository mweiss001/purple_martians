<?php

session_start();

echo <<<TEXT

<!DOCTYPE html>
<html class="dark">

<head>

<link rel="stylesheet" href="./sessions.css">

</head>

<body>
TEXT;



function show_player_num_icon_name_host($num, $col, $name, $sid, $host, $ip)
{
   $nm = str_pad($name, 8, " ", STR_PAD_RIGHT); 
   $ss = "Session Id:$sid";
   $ss = str_pad($ss, 15, " ", STR_PAD_RIGHT); 
   $ip = "IP:$ip";
   $ip = str_pad($ip, 20, " ", STR_PAD_RIGHT); 
   $hn = "Hostname:$host";
   $hn = str_pad($hn, 32, " ", STR_PAD_RIGHT); 


   $iconpath = "/assets/icons/player_icon_$col.png";
   $alt = "alt=\"icon not found\"";
   echo "<div style=\" display:flex; gap:32px; \">";
      echo "<div class=\"icon-text-container\">";
         echo "<span class=\"text\">$num</span>";
         echo "<img src=$iconpath $alt class=\"icon\">";
         echo "<span class=\"text\">$nm</span>";
     echo "</div>";
   echo "<span class=\"text\">$ss</span>";
   echo "<span class=\"text\">$ip</span>";
   echo "<span class=\"text\">$hn</span>";
}

function secondsToDHMS($seconds)
{
   $d = floor($seconds / 86400);
   $h = floor($seconds / 3600);
   $m = floor(($seconds % 3600) / 60);
   $s = $seconds % 60;
   return sprintf ("%02d:%02d:%02d:%02d", $d, $h, $m, $s);
}

function framesToHMS($frames)
{
   $seconds = $frames / 40;
   $h = floor($seconds / 3600);
   $m = floor(($seconds % 3600) / 60);
   $s = $seconds % 60;
   if ($h == 0) return sprintf ("%d:%02d", $m, $s);
   return sprintf ("%02d:%02d:%02d", $h, $m, $s);
}


// database setup
$db_filepath = "/home/m/dev/purple_martians/data/sessions.db";
if (!file_exists($db_filepath))
{
   echo "Database file: $filename not found.";
   return;
}
$db = new PDO("sqlite:/home/m/dev/purple_martians/data/sessions.db");

echo "<div class=\"div-sessions-page\">";

   $but = "class=\"button\" id=\"links_button\"";
   echo "<div class=\"div-sessions-page-links\">";
   echo "<a href=\"../index.html\" $but >Back to Main Page</a>";
   echo "<a href=\"sessions.php\"  $but >Sessions</a>";
   echo "<a href=\"status.php\"  $but >Status</a>";
   echo "</div>";

   $res = $GLOBALS['db']->query("SELECT MAX(timestamp), * FROM status");
   if ($res->fetchColumn() == 0)  { echo " current status not found \n";  return; } 

   $res = $GLOBALS['db']->query("SELECT MAX(timestamp), * FROM status");
   $row = $res->fetch(PDO::FETCH_ASSOC);

   $level      = $row['level'];
   $level_time = $row['level_time'];
   $moves      = $row['moves'];
   $enemies    = $row['enemies'];
   $clients    = $row['clients'];
   $cpu        = $row['cpu'];
   $version    = $row['version'];

   $but = "class=\"button\" id=\"status_page_button\""; 

   echo "<div id=\"status_page\"  class=\"div-section-container\">";
      echo "<div id=\"status_page\"  class=\"div-section-title-section-frame\">";
         echo "<div id=\"status_page\"  class=\"div-section-title-section-container\">";
            echo "<div id=\"status_page\"  class=\"div-section-title-frame\">Server Status</div>";
            
            echo "<div id=\"status_page\"  class=\"div-section-title-frame-buttons-frame\">";
               echo "<a href=\"status.php\" $but >Refresh</a>";
            echo "</div>";
         echo "</div>";
      echo "</div>";

      echo "<div style=\" display:flex; gap:2px;  padding:4px;\">";
         echo "<div id=\"status_page\"  class=\"div-section-sub-section-pretext\">";
            echo "Server Version : $version\n";
            echo "Last Update    : "         . date('Y-m-d h:i:s', strtotime($row['timestamp']))  . "\n";
            echo "Server Uptime  :         " . secondsToDHMS($row['uptime'])    . "\n";
            echo "Server Load    : $cpu%\n";
         echo "</div>";
      echo "</div>";

      if ($clients == 0) echo "<div id=\"status_page\" class=\"div-section-sub-section-title-frame\">No Current Clients</div>";
      else
      {
         echo "<div id=\"status_page\" class=\"div-section-sub-section-title-frame\">Current Clients</div>";
            echo "<div id=\"status_page\"  class=\"div-section-sub-section-gm_table\">";
            echo "<table id=\"status_page_table\"><tbody>";
               $sql = "SELECT id, player_name, player_num, player_color, hostname, ip FROM sessions WHERE endreason='open'";
               $res = $GLOBALS['db']->query($sql);
               while ($row = $res->fetch(PDO::FETCH_ASSOC))
               {
                  $num = $row['player_num'];
                  $col = $row['player_color'];
                  $nam = $row['player_name'];
                  $hst = $row['hostname'];
                  $sid = $row['id'];
                  $ip = $row['ip'];
                  echo "<tr><td style=\"text-align: left;\">";
                  show_player_num_icon_name_host($num, $col, $nam, $sid, $hst, $ip);
                  echo "</td></tr>";
               }
            echo "</tbody></table>";
         echo "</div>";
      }

      echo "<div id=\"status_page\" class=\"div-section-sub-section-title-frame\">Current Level</div>";
      echo "<div style=\" display:flex; gap:2px;  padding:4px;\">";

         $iconpath = sprintf("/assets/icons/lev%03d.png", $level );
         echo "<img style=\" width: 160px; height: 160x;\" src=$iconpath alt=\"icon not found\" >";

         echo "<div id=\"status_page\"  class=\"div-section-sub-section-pretext\">";
            echo "Level   : $level\n";
            echo "Time    : " . framesToHMS($level_time)    . "\n";
            echo "Moves   : $moves\n";
            echo "Enemies : $enemies\n";
         echo "</div>";
      echo "</div>";

   echo "</div>";
echo "</div>";

?>
</body></html>












