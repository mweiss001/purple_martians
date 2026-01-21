<?php

// convert sql date to java format
function getJDate($d)
{

   $dt = date('U', strtotime($d)) * 1000; 
   $st = "Date( $dt )";
   return $st;
}

function getTimelineData()
{
   $mode = $_SESSION['tl_mode'];

   if ($mode == 0) $sql_where = "";  

   if ($mode == 1)
   {
      // get start and end times from current session
      if (isset($_SESSION['current_session_id'])) $current_session_id = $_SESSION['current_session_id'];
      else $current_session_id = 0;

      $res = $GLOBALS['db']->query("SELECT dt_start, dt_end FROM sessions WHERE id=$current_session_id");
      if ($row = $res->fetch(PDO::FETCH_ASSOC))
      {
         $dt_start = $row['dt_start'];
         $dt_end = $row['dt_end'];
      }
   }

   if ($mode == 2)
   {
      // get start and end times from current gm
      if (isset($_SESSION['current_gm_muid'])) $current_gm_muid = $_SESSION['current_gm_muid'];
      else $current_gm_muid = 0;

      $res = $GLOBALS['db']->query("SELECT dt_start, dt_end FROM gm WHERE muid='$current_gm_muid'");
      if ($row = $res->fetch(PDO::FETCH_ASSOC))
      {
         $dt_start = $row['dt_start'];
         $dt_end = $row['dt_end'];
      }
   }

   if ($mode == 3)
   {
      $d2 = new DateTimeImmutable("now");
      $d1 = $d2->modify('-1 hour');
      $dt_start = $d1->format('Ymd-His');
      $dt_end   = $d2->format('Ymd-His');
   }
   if ($mode == 4)
   {
      $d2 = new DateTimeImmutable("now");
      $d1 = $d2->modify('-1 day');
      $dt_start = $d1->format('Ymd-His');
      $dt_end   = $d2->format('Ymd-His');
   }


   if ($mode)
   {
      $GLOBALS['tl_start']  = $dt_start;
      $GLOBALS['tl_end']    = $dt_end;
      $sql_where = " WHERE dt_start BETWEEN '$dt_start' AND '$dt_end'";  
   }



   $dataTable = [
       'cols' => [
           ['id' => 'RowLabel', 'label' => 'RowLabel',   'type' => 'string'],
           ['id' => 'BarLabel', 'label' => 'BarLabel',   'type' => 'string'],
           ['id' => 'Tooltip',  'label' => 'Tooltip',    'type' => 'string', 'role' => 'tooltip'],
           ['id' => 'Start',    'label' => 'Start Date', 'type' => 'datetime'],
           ['id' => 'End',      'label' => 'End Date',   'type' => 'datetime'],
           ['id' => 'Color',    'label' => 'Color',      'type' => 'string', 'role' => 'user']

       ],
       'rows' => []
   ];

   // get gm
   $sql = "SELECT gm.muid, dt_start, dt_end, level, duration, filename FROM gm $sql_where";
   $res = $GLOBALS['db']->query($sql);
   while ($row = $res->fetch(PDO::FETCH_ASSOC))
   {
      $muid        = $row['muid'];
      $st = getJDate($row['dt_start']);
      $en = getJDate($row['dt_end']);

       $temp = [
          ['v' => 'gm'],
          ['v' => $muid],
          ['v' => ''],
          ['v' => $st],
          ['v' => $en],
          ['v' => '15']

      ];
      $dataTable['rows'][] = ['c' => $temp];
   }

   // get sessions
   $sql = "SELECT id, dt_start, dt_end, player_name, player_color FROM sessions $sql_where";
   //echo $sql;

   $res = $GLOBALS['db']->query($sql);
   while ($row = $res->fetch(PDO::FETCH_ASSOC))
   {
      $id       = $row['id'];
      $name     = $row['player_name'];
      $col      = $row['player_color'];
            
      $st = getJDate($row['dt_start']);
      $en = getJDate($row['dt_end']);

      $temp = [
          ['v' => $name],
          ['v' => $id],
          ['v' => ''],
          ['v' => $st],
          ['v' => $en],
          ['v' => $col]
      ];
      $dataTable['rows'][] = ['c' => $temp];
   }

   // get full range values
   if ($mode == 0)
   {
      $sql = "SELECT MIN(t.dt_start) AS min_val FROM ( SELECT dt_start FROM gm UNION ALL SELECT dt_start FROM sessions) AS t";
      $res = $GLOBALS['db']->query($sql);
      if ($row = $res->fetch(PDO::FETCH_ASSOC)) $GLOBALS['tl_start'] = $row['min_val'];         

      $sql = "SELECT MAX(t.dt_end) AS max_val FROM ( SELECT dt_end FROM gm UNION ALL SELECT dt_end FROM sessions) AS t";
      $res = $GLOBALS['db']->query($sql);
      if ($row = $res->fetch(PDO::FETCH_ASSOC)) $GLOBALS['tl_end'] = $row['max_val'];         
   }

   return $dataTable;
}


function setup_timeline()
{
?>

   <script type="text/javascript" src="https://www.gstatic.com/charts/loader.js"></script>
   <script type="text/javascript">
      var jsonData = JSON.parse('<?php echo json_encode(getTimelineData()); ?>');
      google.charts.load('current', {'packages':['timeline']});

      // Set a callback to run when the Google Visualization API is loaded.
      google.charts.setOnLoadCallback(drawChart);



      function drawChart()
      {
         var container = document.getElementById('chart_div');
         var chart = new google.visualization.Timeline(container);
         var data = new google.visualization.DataTable(jsonData);

         const myArray = data.getDistinctValues(0);
         const numRows = myArray.length;

         var paddingHeight = 50;
         var rowHeight = numRows * 41;
         var chartHeight = rowHeight + paddingHeight;
         var options =
         {
            backgroundColor: '#666600',
            alternatingRowStyle: true,

            height: chartHeight,
            chartArea: { height: rowHeight },

            timeline: { rowLabelStyle: {color: 'white'} }



         };
         chart.draw(data, options);
      }
           
      // Add event listener to redraw the chart on window resize
      window.addEventListener('resize', drawChart, false);
  
   </script>

<?php
}


function show_timeline()
{
   $but = "class=\"button\" id=\"timeline_button\""; 

   $tl_mode   = $GLOBALS['tl_mode'];
   $tl_start  = $GLOBALS['tl_start'];
   $tl_end    = $GLOBALS['tl_end'];


   echo "<div id=\"timeline\"  class=\"div-section-container\">";
      echo "<div id=\"timeline\"  class=\"div-section-title-section-frame\">";
         echo "<div id=\"timeline\"  class=\"div-section-title-section-container\">";
            echo "<div id=\"timeline\"  class=\"div-section-title-frame\">Sessions Timeline</div>";
            echo "<div id=\"timeline\"  class=\"div-section-title-frame-buttons-frame\">";

               echo "<a href=\"#\" $but >Start:$tl_start</a>";
               echo "<a href=\"#\" $but >End:$tl_end</a>";

               if ($tl_mode == 0) echo "<a href=\"sessions.php?tl_mode=1\" $but >Show All</a>";
               if ($tl_mode == 1) echo "<a href=\"sessions.php?tl_mode=2\" $but >Current Session</a>";
               if ($tl_mode == 2) echo "<a href=\"sessions.php?tl_mode=3\" $but >Current Game</a>";
               if ($tl_mode == 3) echo "<a href=\"sessions.php?tl_mode=4\" $but >Last Hour</a>";
               if ($tl_mode == 4) echo "<a href=\"sessions.php?tl_mode=0\" $but >Last Day</a>";

            echo "</div>";

         echo "</div>";
      echo "</div>";
         echo "<div id=\"chart_div\" class=\"div-section-sub-section-timeline\">";
      echo "</div>";
   echo "</div>";
}

?>

