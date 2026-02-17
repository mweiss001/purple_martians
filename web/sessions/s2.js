

var data;
var chartArray = [];
var chartDataArray = [];
var numCharts = 3;


document.addEventListener('DOMContentLoaded', (event) =>
{
   fetchData();

   var option =
   {
      animation: false,
      tooltip: {},
      legend: {},
      xAxis:
      {
         type: 'value',
         min: 'dataMin', 
         max: 'dataMax'
      },
      yAxis: { type: 'value' },
  };

   const chartArea = document.getElementById("chartArea");

   for (var i=0; i<numCharts; i++)
   {
      var name = "chartContainer" + i;
      const chartContainer = document.createElement('div');
      chartContainer.setAttribute('id', name);
      chartContainer.style.height = '240px';
      chartArea.appendChild(chartContainer);

      chartArray[i] = echarts.init(document.getElementById(name));
      chartArray[i].setOption(option);
   }

   chartArray[0].setOption( { title: { text: 'cpu'  }, }, false );
   chartArray[1].setOption( { title: { text: 'sync' }, }, false );
   chartArray[2].setOption( { title: { text: 'tkbs' }, }, false );



   document.getElementById("testButton1").addEventListener("click", function()
   {
      //console.log("test button 1 pressed");   
      
      fetchData();
      
   });
   document.getElementById("testButton2").addEventListener("click", function()
   {
      console.log("test button 2 pressed");   
   });














});


async function fetchData()
{
   var url = 's2.php';

/*
   var range = timeline.view;
   if (range == 0) url += '?range=all'; 
   if (range == 1) url += '?range=last&val=60';
   if (range == 2) url += '?range=last&val=1440';
   if (range == 3) url += '?range=session&val=' + currentSessionId;
*/
   try
   {
      const response = await fetch(url);
      if (!response.ok)
      { // Check if the response status is in the 200-299 range
         throw new Error(`HTTP error! Status: ${response.status}`);
      }
      data = await response.json(); // Parse the response body as JSON

      do_stuff();

   } catch (error) { console.error("Fetch error:", error.message); } // Handles network errors or the error thrown above
}



function do_stuff()
{
   //console.log(data);

   for (var i=0; i<numCharts; i++)
   {
      chartDataArray[i] = [];
      chartDataArray[i].push(['time', 'P0', 'P1', 'P2', 'P3', 'P4', 'P5', 'P6', 'P7']);
   }


   var row = [];

   var ct = -1; // current t

   for (const dat of data)
   {
      var t = dat.ss_id;
      var p = dat.pl_num;
      var v = [];

      v[0] = dat.cpu;
      v[1] = dat.sync * 1000; 
      //v[2] = dat.ping * 1000; 
      v[2] = dat.tkbs; 


      if (t !== ct) // new t
      {
         for (var i=0; i<numCharts; i++)
         {
            // if not first row, push old row
            if (ct !== -1) chartDataArray[i].push(row[i]);

            // clear row and set all to null 
            row[i] = [];
            for (var j=0; j<9; j++) row[i][j] = null;

            // set t
            row[i][0] = t;
         }           
         ct = t; // new current t
      }         
     // set val in column by player num     
     for (var i=0; i<numCharts; i++) row[i][p+1] = v[i];
   }

   // push last row
   for (var i=0; i<2; i++) chartDataArray[i].push(row[i]);

/*
   // add all series
   const series = [];
   for (var i=0; i<8; i++)
   {
      var tt = "P" + i;
      series.push({ type: 'line', name: `${tt}`, encode: { y: `${tt}` }});
   }   
*/

   for (var i=0; i<numCharts; i++)
   {
      // search for valid data
      var vd = [0,0,0,0,0,0,0,0]; 
      for (var j=1; j<chartDataArray[i].length; j++)
         for (var k=1; k<9; k++)
            if (chartDataArray[i][j][k] !== null) vd[k-1] = 1;

      // only add series with valid data
      const series = [];
      for (var j=0; j<8; j++)
      {
         var tt = "P" + j;
         if (vd[j]) series.push({ type: 'line', name: `${tt}`, encode: { y: `${tt}` }});
      }   

      chartArray[i].setOption(
      {
         dataset: { source: chartDataArray[i] },
         series: series  
      }, false );
   }
}















