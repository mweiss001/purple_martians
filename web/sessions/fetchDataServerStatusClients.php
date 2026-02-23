<?php
header('Content-Type: application/json');

// database setup
$db_filepath = "/home/m/dev/purple_martians/data/sessions.db";
if (!file_exists($db_filepath)) { echo "Database file: $db_filepath not found."; return; }
$db = new PDO("sqlite:$db_filepath");

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

$res = $db->query("SELECT id, player_name, player_num, player_color, hostname, ip FROM sessions WHERE endreason='open'");
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

?>


