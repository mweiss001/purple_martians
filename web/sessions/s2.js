

function setupTestButtons()
{
   const button1 = document.createElement('button');
   button1.textContent = 'Test 1';
   button1.setAttribute('id', 'testButton1');
   testButtons.appendChild(button1);

   const button2 = document.createElement('button');
   button2.textContent = 'Test 2';
   button2.setAttribute('id', 'testButton2');
   testButtons.appendChild(button2);

   const in1 = document.createElement('input');
   in1.setAttribute('id', 'in1');
   in1.setAttribute('type', 'range');
   in1.setAttribute('min', '10');
   in1.setAttribute('max', '20');
   in1.setAttribute('step', '1');
   testButtons.appendChild(in1);
  
   // set initial value
   in1.value = 14;
   // listen for changes
   in1.addEventListener("input", () =>
   {
      console.log("slider val:", in1.value);   
   });


   button1.addEventListener("click", function()
   {
      console.log("test button 1 pressed");   
   });
   button2.addEventListener("click", function()
   {
      console.log("test button 2 pressed");   
   });
}


function setupControls()
{
   controlsContainer.style.display = 'flex';
   controlsContainer.style.flexDirection = 'column';
   controlsContainer.style.gap = '2px';
   controlsContainer.style.width = '300px';

//   controlsContainer.style.border = '1px solid #ff0000';



   // interval control
   const intervalSliderContainer = document.createElement('div');
   intervalSliderContainer.style.display = 'flex';
   intervalSliderContainer.style.gap = '4px';
   intervalSliderContainer.style.alignItems = 'center';


   // slider input element
   const intervalSlider = document.createElement('input');
   intervalSlider.setAttribute('id', "intervalSlider");
   intervalSlider.setAttribute('type', 'range');
   intervalSlider.setAttribute('min', '25');
   intervalSlider.setAttribute('max', '2000');
   intervalSliderContainer.appendChild(intervalSlider);

   // label element
   const intervalSliderLabel = document.createElement('label');
   intervalSliderLabel.textContent = 'Interval:';
   intervalSliderContainer.appendChild(intervalSliderLabel);


   // span element
   const intervalSliderSpan = document.createElement('span');
   intervalSliderSpan.setAttribute('id', "intervalSliderSpan");
   intervalSliderContainer.appendChild(intervalSliderSpan);

   intervalSliderSpan.innerHTML = updateInterval;

   controlsContainer.appendChild(intervalSliderContainer);

   intervalSlider.addEventListener("input", () =>
   {
      updateInterval = intervalSlider.value;
      intervalSliderSpan.innerHTML = updateInterval;
      changeTimer();
   });



   // range control

   const rangeSliderContainer = document.createElement('div');
   rangeSliderContainer.style.display = 'flex';
   rangeSliderContainer.style.gap = '4px';
   rangeSliderContainer.style.alignItems = 'center';


   // slider input element
   const rangeSlider = document.createElement('input');
   rangeSlider.setAttribute('id', "rangeSlider");
   rangeSlider.setAttribute('type', 'range');
   rangeSlider.setAttribute('min', '25');
   rangeSlider.setAttribute('max', '2000');
   rangeSliderContainer.appendChild(rangeSlider);

   // label element
   const rangeSliderLabel = document.createElement('label');
   rangeSliderLabel.textContent = 'Range:';
   rangeSliderContainer.appendChild(rangeSliderLabel);


   // span element
   const rangeSliderSpan = document.createElement('span');
   rangeSliderSpan.setAttribute('id', "rangeSliderSpan");
   rangeSliderContainer.appendChild(rangeSliderSpan);

   rangeSliderSpan.innerHTML = chartTimeRange;

   controlsContainer.appendChild(rangeSliderContainer);

   rangeSlider.addEventListener("input", () =>
   {
      chartTimeRange = rangeSlider.value;
      rangeSliderSpan.innerHTML = chartTimeRange;
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



}



function setupCharts()
{
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

   for (i=0; i<numCharts; i++)
   {
      // create div container for chart 
      var name = "chartContainer" + i;
      const container = document.createElement('div');
      container.setAttribute('id', name);
      container.style.height = '240px';
      chartArea.appendChild(container);

      // create chart and set initial options  
      chartArray[i] = echarts.init(container);
      chartArray[i].setOption(option);

      // initialize chart data array
      chartDataArray[i] = [];
      chartDataArray[i].push(['time', 'P0', 'P1', 'P2', 'P3', 'P4', 'P5', 'P6', 'P7']);
   }

   chartArray[0].setOption( { title: { text: 'cpu'  }, }, false );
   chartArray[1].setOption( { title: { text: 'sync' }, }, false );
   chartArray[2].setOption( { title: { text: 'tkbs' }, }, false );
}


















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

var t0, t1, t2, t3;


var data = [];
const chartArray = [];
const chartDataArray = [];
var numCharts = 3;

var curMaxSsid = 0;

var i, j, k;

var updateInterval = 200;
var chartTimeRange = 200;

let fetchRunning = false; 

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
   //setupTestButtons();
   setupControls();
   setupCharts();
   startTimer();
});





async function fetchData()
{
   if (fetchRunning)
   {
      console.log(`${Date.now()} ${Date.now()-t0} -- running`);
      return; // ensures only one instance
   }

   // data from last run
   console.log(`${t0} ${Date.now()-t0} ${t1-t0} ${t2-t1} - rows:${data.length}`);

   // reset all
   t2 = t1 = t0 = Date.now(); 




   if (fetchRunning) return; // ensures only one instance
   fetchRunning = true;
//   const startTime = Date.now(); // start timer
   try
   {
      var url = 'fetchDataStatus.php?start_ssid=';
      if (curMaxSsid == 0) url += '-100'; 
      else url += curMaxSsid;
      const response = await fetch(url);
      if (!response.ok) throw new Error(`HTTP error! Status: ${response.status}`); // Check if the response status is in the 200-299 range
      data = await response.json(); // Parse the response body as JSON
   } catch (error) { console.error("Fetch error:", error.message);
   } finally
   {
      t1 = Date.now();

//     const endTime = Date.now();
//     console.log(`Time:${endTime - startTime}ms - rows:${data.length}`);
     fetchRunning = false;
     if (data.length) do_stuff(); // only if data

     t2 = Date.now();

   }
}



function do_stuff()
{
   if (data.length == 0) return;


   // data to be processed consists of rows from client_status table
   // each row is for one frame and player and has muliple data values
   // ss_id is the time value
   // data is aleady sorted by ss_id and player_num   
   // (the player num sort is probably not required)

   // data is read one row at a time and added to temporay rows for each chart
   // when a new ss_id is read, temp rows are pushed to chart data


   const row = [];
   var ct = -1; // current t

   // player colors array
   const pc = Array(8).fill(0);


   for (const dat of data)
   {
      var t = dat.ss_id;  // t (ss_id)
      var p = dat.pl_num; // player num
      pc[p] = dat.pl_col; // player color

      var v = [];         // value array  
      v[0] = dat.cpu;
      v[1] = dat.sync; 
      v[2] = dat.tkbs; 


      // update current max ssid  
      if (t > curMaxSsid) curMaxSsid = t;

      if (t !== ct) // new t (or first)
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


   // remove elements from start of array to maintain chartTimeRange
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

      const series = [];
      const legend = [];

      // only add series and legend if valid data
      for (j=0; j<8; j++)
      {
         if (vd[j])
         {
            const tt = "P" + j;
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
