

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
//      fetchNumRows();

      insertControl("fakekey", 0, 0);



   });
   button2.addEventListener("click", function()
   {
      console.log("test button 2 pressed");   
//      deleteRows10();
   });
}



function createDiv()
{
   const container = document.createElement('div');
   container.style.display = 'flex';
   container.style.gap = '4px';
   container.style.alignItems = 'center';
   container.style.border = '1px solid #8000FF';
   return container;   
}

function setupRangeControl()
{
   const container = createDiv();

   // slider input element
   const slider = document.createElement('input');
   slider.setAttribute('id', "rangeSlider");
   slider.setAttribute('type', 'range');
   slider.setAttribute('min', '1');
   slider.setAttribute('max', '60');
   slider.setAttribute('step', '1');
   slider.value = chartTimeRange;
   container.appendChild(slider);

   // label element
   const label = document.createElement('label');
   label.textContent = 'Range Control:';
   container.appendChild(label);

   // span element
   const span = document.createElement('span');
   span.setAttribute('id', "rangeSliderSpan");
   container.appendChild(span);

   span.innerHTML = chartTimeRange;
   slider.addEventListener("input", () =>
   {
      chartTimeRange = slider.value;
      span.innerHTML = chartTimeRange;
   });
    controlsContainer.appendChild(container);
}


function setupIntervalControl()
{
   const container = createDiv();

   // slider input element
   const slider = document.createElement('input');
   slider.setAttribute('id', "intervalSlider");
   slider.setAttribute('type', 'range');
   slider.setAttribute('min', '25');
   slider.setAttribute('max', '2000');
   slider.setAttribute('step', '25');
   slider.value = updateInterval;
   container.appendChild(slider);

   // label element
   const label = document.createElement('label');
   label.textContent = 'Refresh Interval:';
   container.appendChild(label);

   // span element
   const span = document.createElement('span');
   span.setAttribute('id', "intervalSliderSpan");
   container.appendChild(span);


   span.innerHTML = updateInterval;
   slider.addEventListener("input", () =>
   {
      updateInterval = slider.value;
      span.innerHTML = updateInterval;
      changeTimer();
   });
  
   controlsContainer.appendChild(container);
}

function setupLabelValueSpan(lab, id)
{
   const container = createDiv();
   container.style.justifyContent = 'right';

   // label element
   const label = document.createElement('label');
   label.textContent = lab;
   container.appendChild(label);

   // span element
   const ele = document.createElement('span');
   ele.setAttribute('id', id);
   ele.style.width = '40px';

   container.appendChild(ele);
   controlsContainer.appendChild(container);
}


function setupStartStopButton()
{
   const container = createDiv();
   container.style.justifyContent = 'center';
   container.style.border = 'none';

   const stopButton = document.createElement('button');
   stopButton.textContent = 'Stop';
   stopButton.setAttribute('id', 'stopButton');
   container.appendChild(stopButton);
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
   controlsContainer.appendChild(container);
}

function setupControls()
{
   controlsContainer.style.display = 'flex';
   controlsContainer.style.flexDirection = 'column';
   controlsContainer.style.gap = '2px';
   controlsContainer.style.width = '360px';
   //controlsContainer.style.border = '1px solid #FF0000';

   setupIntervalControl();
   setupLabelValueSpan("Actual Interval:",    'actualIntervalSpan');
   setupLabelValueSpan("Fetch Time:",         'fetchTimeSpan');
   setupLabelValueSpan("Fetch Frames:",       'fetchNumFramesSpan');
   setupLabelValueSpan("Fetch Rows:",         'fetchNumRowsSpan');
   
   setupRangeControl();

//   setupLabelValueSpan("Array Process Time:", 'arrayProcessTimeSpan');
   setupLabelValueSpan("Update Chart Time:",  'updateChartTimeSpan');
  
   setupLabelValueSpan("Pile Up:",            'pileUpSpan');
   
   setupLabelValueSpan("Num Rows:",            'numRowsSpan');

   setupStartStopButton();
}



function setupCharts()
{
   var option =
   {
      animation: false,

      // sets the padding around the chart
      grid:
      {
         left: '1%',
         top: 10,
         right: '1%',
         bottom: 16
      },


      tooltip: {},
      //legend: {},
      xAxis:
      {
         type: 'time',
         min: 'dataMin', 
         max: 'dataMax'
      },
      yAxis: { type: 'value' },
  };


   for (i=0; i<numCharts; i++)
   {
      chartArea.style.display = 'flex';
      chartArea.style.flexDirection = 'column';
      chartArea.style.gap = '8px';

      // create div container for chart 
      var name = "chartContainer" + i;
      const container = document.createElement('div');
      container.setAttribute('id', name);
      container.style.height = '240px';
      container.style.width = '100%';
      container.style.border = '1px solid #FFFFFF';
      chartArea.appendChild(container);

      // create chart and set initial options  
      chartArray[i] = echarts.init(container);
      chartArray[i].setOption(option);


      // create observer to detect container size changes
      const resizeObserver = new ResizeObserver(() =>
      {
         chartArray.forEach((chart) => { chart.resize(); } );
      });

      // find chart container
      var name = "chartContainer" + i;
      resizeObserver.observe(document.getElementById(name));


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

var fetchTime;
var arrayProcessTime;
var updateChartTime;

var pileUp = 0;


var data = [];
const chartArray = [];
const chartDataArray = [];
var numCharts = 3;

var currentMaxTimestamp = 0;

var i, j, k;

var updateInterval = 200;
var chartTimeRange = 5;

let fetchRunning = false; 

let intervalId; // Variable to store the refresh timer interval ID

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
   setupTestButtons();
   setupControls();
   setupCharts();
   startTimer();
});




async function insertControl(key, val, mod)
{
   try
   {
      var url = 'insertStatusControl.php';
      url += "?key=" + key; 
      url += "&val=" + val; 
      url += "&mod=" + mod; 
      fetch(url);
//      if (!response.ok) throw new Error(`HTTP error! Status: ${response.status}`); // Check if the response status is in the 200-299 range
   } catch (error) { console.error("Fetch error:", error.message);
   } finally
   {



   }
}







var data2 = [];
async function fetchNumRows()
{
   try
   {
      var url = 'findStatusNumRows.php';
      const response = await fetch(url);
      if (!response.ok) throw new Error(`HTTP error! Status: ${response.status}`); // Check if the response status is in the 200-299 range
      data2 = await response.json(); // Parse the response body as JSON
   } catch (error) { console.error("Fetch error:", error.message);
   } finally
   {
      document.getElementById('numRowsSpan').innerHTML = data2;
   }
}

async function deleteRows10()
{
   try
   {
      var url = 'deleteStatusRows.php';
      const response = await fetch(url);
      if (!response.ok) throw new Error(`HTTP error! Status: ${response.status}`); // Check if the response status is in the 200-299 range
      data2 = await response.json(); // Parse the response body as JSON
   } catch (error) { console.error("Fetch error:", error.message);
   } finally
   {
      document.getElementById('numRowsSpan').innerHTML = data2;
   }
}



async function fetchData()
{

   document.getElementById('pileUpSpan').innerHTML = pileUp;

   if (fetchRunning)
   {
      pileUp++;
      return; // ensures only one instance
   }
   pileUp = 0;



   document.getElementById('actualIntervalSpan').innerHTML = Date.now()-t0;

   // reset all timers
   t3 = t2 = t1 = t0 = Date.now(); 

   if (fetchRunning) return; // ensures only one instance
   fetchRunning = true;

   try
   {
      var url = 'fetchDataStatus.php?start_timestamp=';
      if (currentMaxTimestamp == 0) url += '-1000'; 
      else url += currentMaxTimestamp;
      const response = await fetch(url);
      if (!response.ok) throw new Error(`HTTP error! Status: ${response.status}`); // Check if the response status is in the 200-299 range
      data = await response.json(); // Parse the response body as JSON
   } catch (error) { console.error("Fetch error:", error.message);
   } finally
   {
      t1 = Date.now();
      fetchTime = t1 - t0;
      document.getElementById('fetchTimeSpan').innerHTML = fetchTime;
      fetchRunning = false;
      if (data.length) do_stuff(); // only if data
   }
}


function do_stuff()
{
   document.getElementById('fetchNumRowsSpan').innerHTML = data.length;

   // data to be processed consists of rows from client_status table
   // each row is 1 frame, 1 player and multiple data values
   // data is aleady sorted by timestamp
   // data is read one row at a time and added to temporay rows for each chart

   // chartData array for each chart type looks like this
   // each chart is for one value type of data (cpu, sync, ping, etc)
   // each row is for one timestamp and multiple players (as many as were found) up to 8 max

   // when a new timestamp is read, all temp rows are pushed to chart data and cleared
   
   // array of temp rows
   const row = []; 

   // current timestamp
   var currentTimestamp = -1;

   // frame tally
   var fetchNumFrames = 0;

   // player colors array
   const pc = Array(8).fill(0);


   for (const dat of data)
   {
      // get timestamp, player number and color
      var timestamp = dat.timestamp;
      var p = dat.pl_num; 
      pc[p] = dat.pl_col; 

      // value array  
      var v = [];        
      v[0] = dat.cpu;
      v[1] = dat.sync; 
      v[2] = dat.tkbs; 


      // update current max tiomestamp  
      if (timestamp > currentMaxTimestamp) currentMaxTimestamp = timestamp;

      // new timestamp (or first)
      if (timestamp !== currentTimestamp)
      {
         fetchNumFrames++;
         for (i=0; i<numCharts; i++)
         {
            // if not first row, push old row
            if (currentTimestamp !== -1) chartDataArray[i].push(row[i]);

            // clear row and set all to null 
            row[i] = [];
            for (j=0; j<9; j++) row[i][j] = null;

            // set t
            row[i][0] = timestamp;
         }           
         currentTimestamp = timestamp; // new currentTimestamp
      }         
     // set val column by player num     
     for (i=0; i<numCharts; i++) row[i][p+1] = v[i];
   }

   // push last row
   for (i=0; i<numCharts; i++) chartDataArray[i].push(row[i]);


   // remove elements from start of array to maintain chartTimeRange
   for (i=0; i<numCharts; i++)
   {
      const extra_length = chartDataArray[i].length - chartTimeRange*40;
      // start from 1 so we do not remove the column header row
      if (extra_length > 0) chartDataArray[i].splice(1, extra_length);
   }


   // check for valid player data
   // series are not added for player data that is null
   const vd = Array(8).fill(0);
   for (j=1; j<chartDataArray[0].length; j++)
      for (k=1; k<9; k++)
         if (chartDataArray[0][j][k] !== null) vd[k-1] = 1;

   // don't even bother profiling the array manipulation stuff
   // it is rarely even 1ms, insignificant compared to fetch and add to chart

   t2 = Date.now();
   //arrayProcessTime = t2 - t1;
   //document.getElementById('arrayProcessTimeSpan').innerHTML = arrayProcessTime;



   for (i=0; i<numCharts; i++)
   {
      const series = [];
//      const legend = [];

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
//            legend.push(tt); 
         }
      }   

      chartArray[i].setOption(
      {
         dataset: { source: chartDataArray[i] },
         series: series,  
//         legend: { data: legend },
      }, false );
   }
      
   
   t3 = Date.now();
   updateChartTime = t3 - t2;
   document.getElementById('updateChartTimeSpan').innerHTML = updateChartTime;
  
   document.getElementById('fetchNumFramesSpan').innerHTML = fetchNumFrames;

}
