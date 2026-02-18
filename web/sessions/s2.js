


const playerColors = [
   "#888888", 
   "#bf6ce8", 
   "#8820ac",
   "#3c7fff",
   "#e01c48",
   "#ff00e8",
   "#ffbf7f",
   "#ff7f00",
   "#7f00ff",
   "#01ff7f",
   "#ff0000",
   "#01ff00",
   "#0000ff",
   "#01ffff",
   "#ffff00",
   "#ffffff",
]


var data;
const chartArray = [];
const chartDataArray = [];
var numCharts = 3;

var curMaxSsid = 0;

var i, j, k;

var updateInterval = 200;
var chartTimeRange = 200;


let intervalId; // Variable to store the interval ID

function startTimer()
{
   // Prevent multiple intervals from running simultaneously
   if (intervalId) return; 
   intervalId = setInterval(fetchData, updateInterval);
   //console.log("Timer started/resumed");
}

function stopTimer()
{
   clearInterval(intervalId);
   intervalId = null; // Clear the ID to indicate the timer is stopped
   //console.log("Timer stopped");
}

function changeTimer()
{
   stopTimer(); // Stop the timer first
   startTimer();
   console.log("Timer changed:", updateInterval);
}





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

   const controlsContainer = document.createElement('div');
   controlsContainer.setAttribute('id', "controlsContainer");
   chartArea.appendChild(controlsContainer);

   // create slider
   const updateIntervalSlider = document.createElement('input');
   updateIntervalSlider.setAttribute('id', "updateIntervalSlider");
   updateIntervalSlider.setAttribute('type', 'range');
   updateIntervalSlider.setAttribute('min', '25');
   updateIntervalSlider.setAttribute('max', '2000');
   controlsContainer.appendChild(updateIntervalSlider);
   updateIntervalSlider.addEventListener("input", () =>
   {
      updateInterval = updateIntervalSlider.value;
      changeTimer();
   });

   // create slider
   const chartTimeRangeSlider = document.createElement('input');
   chartTimeRangeSlider.setAttribute('id', "chartTimeRangeSlider");
   chartTimeRangeSlider.setAttribute('type', 'range');
   chartTimeRangeSlider.setAttribute('min', '25');
   chartTimeRangeSlider.setAttribute('max', '2000');
   controlsContainer.appendChild(chartTimeRangeSlider);
   chartTimeRangeSlider.addEventListener("input", () =>
   {
      chartTimeRange = chartTimeRangeSlider.value;
   });

   const stopButton = document.createElement('button');
   stopButton.textContent = 'Stop';
   stopButton.setAttribute('id', 'stopButton');
   controlsContainer.appendChild(stopButton);
   stopButton.addEventListener("click", function()
   {
      if (intervalId)
      {
         stopTimer();
         stopButton.textContent = 'Start';
      }
      else
      {
         startTimer();
         stopButton.textContent = 'Stop';
      }
   });




   for (i=0; i<numCharts; i++)
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


   for (i=0; i<numCharts; i++)
   {
      chartDataArray[i] = [];
      chartDataArray[i].push(['time', 'P0', 'P1', 'P2', 'P3', 'P4', 'P5', 'P6', 'P7']);
   }

//   setInterval(fetchData, updateInterval);
   startTimer();

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
   var url = 'fetchDataStatus.php?start_ssid=';
   if (curMaxSsid == 0) url += '-100'; 
   else url += curMaxSsid;

   try
   {
      const response = await fetch(url);
      if (!response.ok)
      { // Check if the response status is in the 200-299 range
         throw new Error(`HTTP error! Status: ${response.status}`);
      }
      data = await response.json(); // Parse the response body as JSON
//   console.log(data);
      do_stuff();

   } catch (error) { console.error("Fetch error:", error.message); } // Handles network errors or the error thrown above
}


function do_stuff()
{

   const row = [];

   var ct = -1; // current t


   // player colors array
   const pc = Array(8).fill(0);

   for (const dat of data)
   {
      var t = dat.ss_id;
      var p = dat.pl_num; // player num
      pc[p] = dat.pl_col; // plater color
      var v = [];         // value array  

      v[0] = dat.cpu;
      v[1] = dat.sync; 
      v[2] = dat.tkbs; 

      // get the current max ssid  
      if (t > curMaxSsid) curMaxSsid = t;

      if (t !== ct) // new t
      {
         for (i=0; i<numCharts; i++)
         {
            // if not first row, push old row
            if (ct !== -1) chartDataArray[i].push(row[i]);

            // clear row and set all to null 
            row[i] = [];
            for (j=0; j<9; j++) row[i][j] = null;

            // set t
            row[i][0] = t;
         }           
         ct = t; // new current t
      }         
     // set val in column by player num     
     for (i=0; i<numCharts; i++) row[i][p+1] = v[i];
   }

   // push last row
   for (i=0; i<numCharts; i++) chartDataArray[i].push(row[i]);

   // remove elements from start of array if too big
   //var length = 400;
   for (i=0; i<numCharts; i++)
   {
      const extra_length = chartDataArray[i].length - chartTimeRange;
      // start from 1 so we do not remove the column header row
      if (extra_length > 0) chartDataArray[i].splice(1, extra_length);
   }


   for (i=0; i<numCharts; i++)
   {
      // search for valid data
      const vd = Array(8).fill(0);

      for (j=1; j<chartDataArray[i].length; j++)
         for (k=1; k<9; k++)
            if (chartDataArray[i][j][k] !== null) vd[k-1] = 1;

      // only add series and legend if valid data
      const series = [];
      const legend = [];
      for (j=0; j<8; j++)
      {
         const tt = "P" + j;
         if (vd[j])
         {
            series.push(
            { 
               type: 'line',
               encode: { y: `${tt}` },
               name: `${tt}`,

               symbolSize: 1,

               lineStyle: { width: 1 },  




               itemStyle: { color: playerColors[pc[j]] },
            });
            legend.push(tt); 
         }
      }   

      chartArray[i].setOption(
      {
         dataset: { source: chartDataArray[i] },
         series: series,  
         legend: { data: legend },
      }, false );
   }
}

/*

series: [{
    type: 'line',
    data: [120, 200, 150],
    lineStyle: {
        width: 3,      // Sets line thickness to 3px
        type: 'solid', // Options: 'solid', 'dashed', 'dotted'
        color: 'blue'  // Sets line color
    }
}]




series: [{
    type: 'line',
    data: [120, 200, 150],
    symbol: 'circle', // Options: 'emptyCircle', 'circle', 'rect', 'roundRect', 'triangle', 'diamond', 'pin', 'arrow', 'none'
    symbolSize: 10,   // Sets all points to 10px
    // symbolSize: function (value, params) { return value / 10; }, // Example of dynamic size
}]
*/
