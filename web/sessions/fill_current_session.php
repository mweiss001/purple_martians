<?php

include 'database.php';

$id = 0;
$min = 0;
$view = 0;

if (isset($_GET['id']))   $id   = $_GET['id'];
if (isset($_GET['min']))  $min  = $_GET['min'];
if (isset($_GET['view'])) $view = $_GET['view'];

if ($id)
{

   $res = $db->query("SELECT *, STRFTIME('%Y-%m-%d %H:%M:%S', dt_start, 'localtime') AS dts, STRFTIME('%Y-%m-%d %H:%M:%S', dt_end, 'localtime') AS dte FROM sessions WHERE id=$id");
      
   $row = $res->fetch(PDO::FETCH_ASSOC);

   $col    = $row['player_color'];
   $name   = $row['player_name'];
   $host   = $row['hostname'];

   echo '<div class="current_session div-section-container">';
      echo '<div class="current_session div-section-title-section-frame">';
         echo '<div class="current_session div-section-title-section-container">';
            echo '<div class="current_session div-section-title-frame">';
               $iconpath = "/assets/icons/player_icon_$col.png";
               $alt = "alt=\"icon not found\"";
               echo "Current Session: $id<div class=\"icon-text-container\">";
               echo "<img src=$iconpath $alt class=\"icon\">";
               echo "<span class=\"text\">$name</span>";
               echo "<span class=\"text\">($host)</span>";
               echo "</div>";
            echo '</div>';
            echo '<div class="current_session div-section-title-frame-buttons-frame">';
               echo '<div id="current_session-cont"></div>';
            echo '</div>';
         echo '</div>';
      echo '</div>';

      if (!$min)
      {
         echo "<div class='current_session div-section-sub-section-pretext'>";
            echo "Session Id      : "            . $row['id']                       . "\n";
            echo "hostname        : "            . $row['hostname']                 . "\n";
            echo "ip:port         : "            . $row['ip'] . ":" . $row['port']  . "\n";
            echo "<hr class='current_session' >";
            echo "Start           : "            . date('Y-m-d h:i:s', strtotime($row['dts'])) . "\n";
            echo "End             : "            . date('Y-m-d h:i:s', strtotime($row['dte'])) . "\n";
            echo "Duration        :            " . secondsToHMS($row['duration']) . "\n";
            echo "End Reason      : "            . $row['endreason']    . "\n";
            if ($view)
            {
               echo "<hr class='current_session' >";
               echo "Player Number   : "            . $row['player_num']   . "\n";
               echo "Player Name     : "            . $row['player_name']  . "\n";
               echo "Player Color    : "            . $row['player_color'] . "\n";
               echo "<hr class='current_session' >";
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
         if ($view > 1)
         {
            echo "<div class=\"current_session div-section-sub-section-title-frame\">Bandwidth</div>";
            echo "<div class=\"current_session div-section-sub-section-pretext\">";
               echo "Transmit Bytes\n";
               echo "<hr class='current_session' >";
               echo "Total          : "            . $row['tx_bytes_total']         . "\n";
               echo "Avg per Second : "            . $row['tx_bytes_avg_per_sec']   . "\n";
               echo "Max per Frame  : "            . $row['tx_bytes_max_per_frame'] . "\n";
               echo "<hr class='current_session' >";
               echo "Receive Bytes\n";
               echo "<hr class='current_session' >";
               echo "Total          : "            . $row['rx_bytes_total']         . "\n";
               echo "Avg per Second : "            . $row['rx_bytes_avg_per_sec']   . "\n";
               echo "Max per Frame  : "            . $row['rx_bytes_max_per_frame'] . "\n";
               echo "<hr class='current_session' >";
               echo "Transmit Packets\n";
               echo "<hr class='current_session' >";
               echo "Total          : "            . $row['tx_packets_total']         . "\n";
               echo "Avg per Second : "            . $row['tx_packets_avg_per_sec']   . "\n";
               echo "Max per Frame  : "            . $row['tx_packets_max_per_frame'] . "\n";
               echo "<hr class='current_session' >";
               echo "Receive Packets\n";
               echo "<hr class='current_session' >";
               echo "Total          : "            . $row['rx_packets_total']         . "\n";
               echo "Avg per Second : "            . $row['rx_packets_avg_per_sec']   . "\n";
               echo "Max per Frame  : "            . $row['rx_packets_max_per_frame'] . "\n";
            echo "</div>";
         }
      } // end of if (!$min)
   echo '</div>';
}

?>

