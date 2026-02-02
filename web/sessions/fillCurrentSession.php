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

   $frame = 'divSectionContainer';
   if ($min) $frame = 'divSectionContainerMinimized';
   echo "<div class=\"currentSession $frame\">";
      echo '<div class="currentSession divSectionTitleSectionFrame">';
         echo '<div class="currentSession divSectionTitleSectionContainer">';
            echo '<div class="currentSession divSectionTitleFrame">';
               $iconpath = "/assets/icons/player_icon_$col.png";
               $alt = "alt='icon not found'";
               echo "Current Session: $id<div class='iconTextContainer'>";
               echo "<img class='iconTextContainerIcon' src=$iconpath $alt>";
               echo "<span class='iconTextContainerText'>$name</span>";
               echo "<span class='iconTextContainerText'>($host)</span>";
               echo "</div>";
            echo '</div>';
            echo '<div class="currentSession divSectionTitleFrameButtonsFrame">';
               echo '<div id="currentSessionControlsContainer"></div>';
            echo '</div>';
         echo '</div>';
      echo '</div>';

      if (!$min)
      {
         $hr = "<hr class='currentSession'>";   
         echo "<div class='currentSession divSectionSubSectionPretext'>";
            echo "Session Id      : "         . $row['id']                                  . "\n";
            echo "hostname        : "         . $row['hostname']                            . "\n";
            echo "ip:port         : "         . $row['ip'] . ":" . $row['port']             . "\n";
            echo $hr;
            echo "Start           : "         . date('Y-m-d h:i:s', strtotime($row['dts'])) . "\n";
            echo "End             : "         . date('Y-m-d h:i:s', strtotime($row['dte'])) . "\n";
            echo "Duration        :        "  . secondsToDHMS($row['duration'])             . "\n";
            echo "End Reason      : "         . $row['endreason']                           . "\n";
            if ($view)
            {
               echo $hr;
               echo "Player Number   : "      . $row['player_num']      . "\n";
               echo "Player Name     : "      . $row['player_name']     . "\n";
               echo "Player Color    : "      . $row['player_color']    . "\n";
               echo $hr;
               echo "Moves           : "      . $row['cdat_rx']         . "\n";
               echo "Next Levels     : "      . $row['next_levels']     . "\n";
               echo "Exits Activated : "      . $row['exits_activated'] . "\n";
               echo "Respawns        : "      . $row['respawns']        . "\n";
               echo "Shots Fired     : "      . $row['shots_fired']     . "\n";
               echo "Enemy Hits      : "      . $row['enemy_hits']      . "\n";
               echo "Player Hits     : "      . $row['player_hits']     . "\n";
               echo "Self Hits       : "      . $row['self_hits']       . "\n";
               echo "Purple Coins    : "      . $row['purple_coins']    . "\n";
            }
         echo "</div>";
         if ($view > 1)
         {
            echo "<div class='currentSession divSectionSubSectionTitleFrame'>Bandwidth</div>";
            echo "<div class='currentSession divSectionSubSectionPretext'>";
               echo "Transmit Bytes\n";
               echo $hr;
               echo "Total          : "       . $row['tx_bytes_total']         . "\n";
               echo "Avg per Second : "       . $row['tx_bytes_avg_per_sec']   . "\n";
               echo "Max per Frame  : "       . $row['tx_bytes_max_per_frame'] . "\n";
               echo $hr;
               echo "Receive Bytes\n";
               echo $hr;
               echo "Total          : "       . $row['rx_bytes_total']         . "\n";
               echo "Avg per Second : "       . $row['rx_bytes_avg_per_sec']   . "\n";
               echo "Max per Frame  : "       . $row['rx_bytes_max_per_frame'] . "\n";
               echo $hr;
               echo "Transmit Packets\n";
               echo $hr;
               echo "Total          : "       . $row['tx_packets_total']         . "\n";
               echo "Avg per Second : "       . $row['tx_packets_avg_per_sec']   . "\n";
               echo "Max per Frame  : "       . $row['tx_packets_max_per_frame'] . "\n";
               echo $hr;
               echo "Receive Packets\n";
               echo $hr;
               echo "Total          : "       . $row['rx_packets_total']         . "\n";
               echo "Avg per Second : "       . $row['rx_packets_avg_per_sec']   . "\n";
               echo "Max per Frame  : "       . $row['rx_packets_max_per_frame'] . "\n";
            echo "</div>";
         }
      } // end of if (!$min)
   echo '</div>';
}
?>

