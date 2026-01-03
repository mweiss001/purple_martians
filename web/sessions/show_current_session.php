<?php

function secondsToHMS($seconds)
{
   $h = floor($seconds / 3600);
   $m = floor(($seconds % 3600) / 60);
   $s = $seconds % 60;
   return sprintf ("%02d:%02d:%02d", $h, $m, $s);
}

function show_current_session($current_session_id)
{
   // get info about session
   $result = mysqli_query($GLOBALS['conn'], "SELECT * FROM sessions WHERE id=$current_session_id");

   if ($result->num_rows == 0)
   {
      echo " session not found \n";
      return;
   }

   $row = $result->fetch_assoc();

   echo "<div id=\"current_session\"  class=\"div-section-sub-section-pretext\">";

   echo "Session Id      : "            . $row['id']           . "\n";
   echo "Filename        : "            . $row['filename']     . "\n";
   echo "hostname        : "            . $row['hostname']  . "\n";
   echo "ip:port         : "            . $row['ip'] . ":" . $row['port']  . "\n";
   echo "End Reason      : "            . $row['endreason']    . "\n";
   
   echo "<hr>";

   echo "Player Number   : "            . $row['player_num']   . "\n";
   echo "Player Name     : "            . $row['player_name']  . "\n";
   echo "Player Color    : "            . $row['player_color'] . "\n";


   echo "<hr>";

   echo "Start           : "            . $row['dt_start']  . "\n";
   echo "End             : "            . $row['dt_end']    . "\n";
   $dur = secondsToHMS($row['duration']);
   echo "Duration        :            " . $dur              . "\n";

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

   echo "</div>";
   if (0)
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
}
?>

