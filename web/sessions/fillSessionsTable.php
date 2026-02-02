<?php

include 'database.php';

function setupColumns()
{
   global $columnDisplayNames;
   global $columnNames;
   global $view;

   $columnDisplayNamesBasic = array("id", "Player",     "player_name", "Start Time",                                                       "duration", "duration (s)",     "ip", "host",     "Session End Reason", "cdat"    );
   $columnNamesBasic        =       "id,   player_color, player_name,   STRFTIME('%Y-%m-%d %H:%M:%S', dt_start, 'localtime') AS dt_start,   duration,   duration AS dur2,   ip,   hostname,   endreason,            cdat_rx";

   $columnDisplayNamesBasic2 = array("id", "Date",     "ip", "port", "host",     "endreason", "duration",  "cdat");
   $columnNamesBasic2        =       "id,   dt_start,   ip,   port,   hostname,   endreason,   duration,   cdat_rx";

   $columnDisplayNamesPlayer = array("player number", "next levels", "exits activated", "respawns", "shots fired", "enemy hits", "player hits", "self hits", "purple coins");
   $columnNamesPlayer        =       "player_num,      next_levels,   exits_activated,   respawns,   shots_fired,   enemy_hits,   player_hits,   self_hits,   purple_coins";

   $columnDisplayNamesBandwidthByte = array ("tx bytes total", "tx bytes avg per sec", "tx bytes max per frame", "rx bytes total", "rx bytes avg per sec", "rx bytes max per frame");
   $columnNamesBandwidthByte        =        "tx_bytes_total,   tx_bytes_avg_per_sec,   tx_bytes_max_per_frame,   rx_bytes_total,   rx_bytes_avg_per_sec,   rx_bytes_max_per_frame";

   $columnDisplayNamesBandwidthPacket = array ("tx packets total", "tx packets avg per sec", "tx packets max per frame", "rx packets total", "rx packets avg per sec", "rx packets max per frame");
   $columnNamesBandwidthPacket        =        "tx_packets_total,   tx_packets_avg_per_sec,   tx_packets_max_per_frame,   rx_packets_total,   rx_packets_avg_per_sec,   rx_packets_max_per_frame";

   $columnDisplayNamesBandwidth = array_merge($columnDisplayNamesBandwidthByte,   $columnDisplayNamesBandwidthPacket);
   $columnNamesBandwidth        =             $columnNamesBandwidthByte  . ", " . $columnNamesBandwidthPacket;

   if ($view == 0) // default
   {
      $columnDisplayNames = $columnDisplayNamesBasic;
      $columnNames = $columnNamesBasic;
   }

   if ($view == 1) // player
   {
      $columnDisplayNames = array_merge($columnDisplayNamesBasic,  $columnDisplayNamesPlayer);
      $columnNames =                    $columnNamesBasic . ", " . $columnNamesPlayer;
   }

   if ($view == 2) // bandwidth
   {
      $columnDisplayNames = array_merge($columnDisplayNamesBasic, $columnDisplayNamesBandwidth);
      $columnNames =                    $columnNamesBasic . ", " . $columnNamesBandwidth;
   }

   if ($view == 3) // all
   {
      $columnDisplayNames = array_merge($columnDisplayNamesBasic2,  $columnDisplayNamesPlayer, $columnDisplayNamesBandwidth);
      $columnNames =                    $columnNamesBasic2 . ", " . $columnNamesPlayer. ", " . $columnNamesBandwidth;
   }
}   


$columnDisplayNames = array();
$columnNames;

$min = 0;
$view = 0;

if (isset($_GET['min']))  $min  = $_GET['min'];
if (isset($_GET['view'])) $view = $_GET['view'];


$frame = 'divSectionContainer';
if ($min) $frame = 'divSectionContainerMinimized';
echo "<div class=\"sessionsTable $frame\">";
   echo '<div class="sessionsTable divSectionTitleSectionFrame">';
      echo '<div class="sessionsTable divSectionTitleSectionContainer">';
         echo '<div class="sessionsTable divSectionTitleFrame">Sessions Table</div>';
         echo '<div class="sessionsTable divSectionTitleFrameButtonsFrame">';
            echo '<div id="sessionsTableControlsContainer"></div>';
         echo '</div>';
      echo '</div>';
   echo '</div>';



if (!$min)
{
   setupColumns();

   echo '<div class="sessionsTable divSectionSubSectionSessionsTable">';
      echo '<table id="myTable" class="display cell-border compact sessionsTableTable" style="width:100%">';
         echo '<thead>';
            echo '<tr>';
               foreach($columnDisplayNames as $col)
               {
                  if ($col == 'player_name') continue;
                  echo "<th>$col</th>";
               }
            echo '</tr>';
         echo '</thead>';
         echo '<tbody>';
            $sql = "SELECT $columnNames FROM sessions";
            $res = $GLOBALS['db']->query($sql);
            while ($row = $res->fetch(PDO::FETCH_ASSOC))
            {
               $id = $row['id'];
               echo "<tr currentSessionId=$id>";
               foreach($row as $col => $val)
               {
                  if ($col == 'player_name') continue;
                  if ($col == 'player_color')
                  {
                     $iconpath = "/assets/icons/player_icon_$val.png";
                     $alt = 'alt="icon not found"';
                     $name = $row['player_name'];

                     echo '<td><div class="iconTextContainer">';
                     echo "<img class='iconTextContainerIcon' src=$iconpath $alt>";
                     echo "<span class='iconTextContainerText'>$name</span>";

                     echo '</div></td>';
                     continue;
                  }
                  if ($col == 'duration')
                  {
                     echo '<td>' . secondsToDHMS($val) . '</td>';
                     continue;
                  }
                  echo "<td>$val</td>";
               }
               echo '</tr>';
            }
         echo "</tbody>";
      echo "</table>";
   echo "</div>";
}
echo "</div>";
