<?php

function secondsToHMS($seconds)
{
   $h = floor($seconds / 3600);
   $m = floor(($seconds % 3600) / 60);
   $s = $seconds % 60;
   return sprintf ("%02d:%02d:%02d", $h, $m, $s);
}

function show_player_icon_and_name($col, $name)
{
   $iconpath = "/assets/icons/player_icon_$col.png";
   $alt = "alt=\"icon not found\"";

   echo "<div class=\"icon-text-container\">";

   echo "<img src=$iconpath $alt class=\"icon\">";
   echo "<span class=\"text\">$name</span>";

   echo "</div>";
}

function show_current_session()
{
   $current_session_id = $GLOBALS['current_session_id'];

   $res = $GLOBALS['db']->query("SELECT COUNT(*) FROM sessions WHERE id=$current_session_id");
   if ($res->fetchColumn() == 0)  { echo " session [$current_session_id] not found \n";  return; } 

   $res = $GLOBALS['db']->query("SELECT * FROM sessions WHERE id=$current_session_id");
   $row = $res->fetch(PDO::FETCH_ASSOC);

   $col    = $row['player_color'];
   $name   = $row['player_name'];

   $bnd_on = $GLOBALS['bnd_on'];
   $plr_on = $GLOBALS['plr_on'];
   $gm_on  = $GLOBALS['gm_on'];

   $but = "class=\"button\" id=\"current_session_button\"";
   echo "<div id=\"current_session\"  class=\"div-section-container\">";
      echo "<div id=\"current_session\"  class=\"div-section-title-section-frame\">";
         echo "<div id=\"current_session\"  class=\"div-section-title-section-container\">";
            echo "<div id=\"current_session\"  class=\"div-section-title-frame\">";
               echo "Current Session";
               show_player_icon_and_name($col, $name);
               echo "</div>";
            echo "<div id=\"current_session\"  class=\"div-section-title-frame-buttons-frame\">";
               if ($bnd_on)      echo "<a href=\"sessions.php?bnd_on=0\" $but >Bandwidth On </a>";
               else              echo "<a href=\"sessions.php?bnd_on=1\" $but >Bandwidth Off </a>";
               if ($plr_on == 0) echo "<a href=\"sessions.php?plr_on=1\" $but >Player Off </a>";
               if ($plr_on == 1) echo "<a href=\"sessions.php?plr_on=2\" $but >Player Minimal </a>";
               if ($plr_on == 2) echo "<a href=\"sessions.php?plr_on=0\" $but >Player Detailed </a>";
               if ($gm_on)       echo "<a href=\"sessions.php?gm_on=0\"  $but >Saves On </a>";
               else              echo "<a href=\"sessions.php?gm_on=1\"  $but >Saves Off </a>";
            echo "</div>";
         echo "</div>";
      echo "</div>";

      echo "<div id=\"current_session\"  class=\"div-section-sub-section-pretext\">";

         echo "Session Id      : "            . $row['id']           . "\n";
         echo "Filename        : "            . $row['filename']     . "\n";
         echo "hostname        : "            . $row['hostname']  . "\n";
         echo "ip:port         : "            . $row['ip'] . ":" . $row['port']  . "\n";

         echo "<hr>";

         echo "Start           : "            . $row['dt_start']  . "\n";
         echo "End             : "            . $row['dt_end']    . "\n";
         $dur = secondsToHMS($row['duration']);
         echo "Duration        :            " . $dur              . "\n";
         echo "End Reason      : "            . $row['endreason']    . "\n";

         if ($plr_on)
         {
            echo "<hr>";
            echo "Player Number   : "            . $row['player_num']   . "\n";
            echo "Player Name     : "            . $row['player_name']  . "\n";
            echo "Player Color    : "            . $row['player_color'] . "\n";
         }
         if ($plr_on > 1)
         {
            echo "<hr>";
            echo "Moves           : "            . $row['cdat_rx']   . "\n";
            echo "Next Levels     : "            . $row['next_levels']   . "\n";
            echo "Exits Activated : "            . $row['exits_activated']   . "\n";
            echo "Respawns        : "            . $row['respawns']   . "\n";
            echo "Shots Fired     : "            . $row['shots_fired']   . "\n";
            echo "Enemy Hits      : "            . $row['enemy_hits']   . "\n";
            echo "Player Hits     : "            . $row['player_hits']   . "\n";
            echo "Self Hits       : "            . $row['self_hits']   . "\n";
            echo "Purple Coins    : "            . $row['purple_coins']   . "\n";
         }
      echo "</div>";
      if ($bnd_on)
      {
         echo "<div id=\"current_session\" class=\"div-section-sub-section-title-frame\">Bandwidth</div>";
         echo "<div id=\"current_session\" class=\"div-section-sub-section-pretext\">";
            echo "Transmit Bytes\n";
            echo "<hr>";
            echo "Total          : "            . $row['tx_bytes_total']         . "\n";
            echo "Avg per Second : "            . $row['tx_bytes_avg_per_sec']   . "\n";
            echo "Max per Frame  : "            . $row['tx_bytes_max_per_frame'] . "\n";
            echo "<hr>";

            echo "Receive Bytes\n";
            echo "<hr>";
            echo "Total          : "            . $row['rx_bytes_total']         . "\n";
            echo "Avg per Second : "            . $row['rx_bytes_avg_per_sec']   . "\n";
            echo "Max per Frame  : "            . $row['rx_bytes_max_per_frame'] . "\n";
            echo "<hr>";

            echo "Transmit Packets\n";
            echo "<hr>";
            echo "Total          : "            . $row['tx_packets_total']         . "\n";
            echo "Avg per Second : "            . $row['tx_packets_avg_per_sec']   . "\n";
            echo "Max per Frame  : "            . $row['tx_packets_max_per_frame'] . "\n";
            echo "<hr>";

            echo "Receive Packets\n";
            echo "<hr>";
            echo "Total          : "            . $row['rx_packets_total']         . "\n";
            echo "Avg per Second : "            . $row['rx_packets_avg_per_sec']   . "\n";
            echo "Max per Frame  : "            . $row['rx_packets_max_per_frame'] . "\n";
         echo "</div>";
      }
   echo "</div>";
}
?>

