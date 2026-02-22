<?php

session_start();

include "database.php";

// database setup
$db_filepath = "/home/m/dev/purple_martians/data/server_status.db";
if (!file_exists($db_filepath)) { echo "Database file: $db_filepath not found.\n"; return; }
$db = new PDO("sqlite:$db_filepath");


// database setup
$db_filepath = "/home/m/dev/purple_martians/data/sessions.db";
if (!file_exists($db_filepath)) { echo "Database file: $db_filepath not found.\n"; return; }
$db2 = new PDO("sqlite:$db_filepath");



echo <<<TEXT

<!DOCTYPE html>
<html class="dark">

<head>

<link rel="stylesheet" href="./sessions.css">

</head>

<script type="text/javascript"> setTimeout(function() { window.location.reload(); }, 1000);  </script>

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
      echo "<div class='iconTextContainer'>";
         echo "<span class=\"text\">$num</span>";
         echo "<img src=$iconpath $alt class='iconTextContainerIcon'>";
         echo "<span class='iconTextContainerText'>$nm</span>";
     echo "</div>";
   echo "<span class='iconTextContainerText'>$ss</span>";
   echo "<span class='iconTextContainerText'>$ip</span>";
   echo "<span class='iconTextContainerText'>$hn</span>";
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


echo "<div class='divSessionsPage'>";

   $but = 'class="button linksButton"';
   echo "<div class='divSessionsPageLinks'>";
   echo "<a href=\"../index.html\" $but >Back to Main Page</a>";
   echo "<a href=\"sessions.html\"  $but >Sessions</a>";
   echo "<a href=\"status.php\"  $but >Status</a>";
   echo "</div>";

   $res = $db->query("SELECT MAX(timestamp), * FROM status");
   if ($res->fetchColumn() == 0)  { echo " current status not found \n";  return; } 

   $res = $db->query("SELECT MAX(timestamp), * FROM status");
   $row = $res->fetch(PDO::FETCH_ASSOC);

   $level      = $row['level'];
   $level_time = $row['level_time'];
   $moves      = $row['moves'];
   $enemies    = $row['enemies'];
   $clients    = $row['clients'];
   $cpu        = $row['cpu'];
   $version    = $row['version'];

   $but = 'class="button statusPageButton"'; 

   echo "<div class='statusPage divSectionContainer'>";
      echo "<div class='statusPage divSectionTitleSectionFrame'>";
         echo "<div class='statusPage divSectionTitleSectionContainer'>";
            echo "<div class='statusPage divSectionTitleFrame'>Server Status</div>";
            
            echo "<div class='statusPage divSectionTitleFrameButtonsFrame'>";
               echo "<a href='status.php' $but >Refresh</a>";
            echo "</div>";
         echo "</div>";
      echo "</div>";

      echo "<div style=\" display:flex; gap:2px;  padding:4px;\">";
         echo "<div class='statusPage divSectionSubSectionPretext'>";
            echo "Server Version : $version\n";
            echo "Last Update    : "         . date('Y-m-d h:i:s', strtotime($row['timestamp']))  . "\n";
            echo "Server Uptime  :        " . secondsToDHMS($row['uptime'])    . "\n";
            echo "Server Load    : $cpu%\n";
         echo "</div>";
      echo "</div>";

      if ($clients == 0) echo "<div class='statusPage divSectionSubSectionTitleFrame'>No Current Clients</div>";
      else
      {
         echo "<div class='statusPage divSectionSubSectionTitleFrame'>Current Clients</div>";
            echo "<div class='statusPage divSectionSubSectionTable'>";
            echo "<table class='statusPageTable'><tbody>";
               $sql = "SELECT id, player_name, player_num, player_color, hostname, ip FROM sessions WHERE endreason='open'";
               $res = $db2->query($sql);
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

      echo "<div class='statusPage divSectionSubSectionTitleFrame'>Current Level</div>";
      echo "<div style='display:flex; gap:2px;  padding:4px;'>";

//         $iconpath = sprintf("/assets/icons/lev%03d.png", $level );
         $iconpath = sprintf("/downloads/lev_stat.bmp");

//         echo "<img style=\" width: 160px; height: 160x;\" src=$iconpath alt=\"icon not found\" >";

         echo "<img style=\" width: 400px; height: 400x;\" src=$iconpath alt=\"icon not found\" >";


         echo "<div class='statusPage divSectionSubSectionPretext'>";
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

