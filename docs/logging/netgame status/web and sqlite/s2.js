import { playerColors, createDiv, setupLabelSpan, insertControl, setupTopPageLinks, setupSliderControl} from './common.js';
import { setupClientStatusChartInsertControls,      updateClientStatusChartInsertControls}   from './clientStatusChartInsertControls.js';
import { setupClientStatusChartRefreshControls }                                             from './clientStatusChartRefreshControls.js';
import { setupClientStatusChartSelectControls,      updateClientStatusChartSelectControls}   from './clientStatusChartSelectControls.js';
import { setupServerSnapshotControls,               updateServerSnapshotControls}            from './serverSnapshotControls.js';
import { setupServerControls,                       updateServerControls}                    from './serverControls.js';



var baseOption =
{
   animation: false,
   
/*   animation: true,
   animationDurationUpdate: '200ms',
   animationEasingUpdate: 'linear',
*/

    title:
    {
        top: '2px',
        padding: [0, 0]
    },

   // sets the padding around the chart
   grid:
   {
      left: '1%',
      top: 20,
      right: '1%',
      bottom: 16
   },
   tooltip:
   {
      trigger: 'axis', formatter: function (params)
      {
         let dateValue = params[0].value[0]; 
         let formattedDate = echarts.format.formatTime('yyyy-MM-dd hh:mm:ss', dateValue);
//         let res = '<b>' + formattedDate + '</b>' + '<br/>Frame:' + frameMap.get(dateValue);
//         let res = 'Frame:' + frameMap.get(dateValue);
         let res = frameMap.get(dateValue);




         // single column
         let i = 1;  
         params.forEach(function (item)
         {
            res += '<br/>' + item.marker + item.seriesName + ': ' + item.value[i++]; // single row
         });

/*


         // 2 columns
         var rows = ['', '', '', '']; 
         let i = 0;  
         params.forEach(function (item)
         {
            if (i < 4) rows[i] = '<br/>' + item.marker + item.seriesName + ': ' + item.value[i+1];
            else rows[i-4] += ' - ' + item.marker + item.seriesName + ': ' + item.value[i+1];
            i++;
         });
         for (var j=0; j<4; j++) res += rows[j];

*/



         return res;
      },
      axisPointer:
      {
        type: 'cross', // 'cross' or 'line'

        intersect: true, 

        link: {xAxisIndex: 'all'} // Link axes across all linked charts
      }

   },
      
//   legend: {},
   xAxis:
   {
      type: 'time',
      min: 'dataMin', 
      max: 'dataMax'
   },
   yAxis: { type: 'value' },
}


function setupCharts()
{
   const chartArea = document.getElementById('chartArea');
   chartArea.style.display = 'flex';
   chartArea.style.flexDirection = 'column';
   chartArea.style.gap = '8px';

   // remove all old charts and containers (if they exist) 
   for (i=0; i<8; i++)
   {
      // dispose of chart if it already exists
      if (chartArray[i]) chartArray[i].dispose;

      // remove container if it already exists
      var name = "chartContainer" + i;      
      var oldc = document.getElementById(name);
      if (oldc) oldc.remove();
   }      

   // create new containrs and charts
   for (i=0; i<numCharts; i++)
   {
      // create div container
      var name = "chartContainer" + i;      
      const container = document.createElement('div');
      container.setAttribute('id', name);
      
      container.classList.add('csChartContainer');


//      container.style.height = chartHeight + 'px';
      container.style.width = '100%';
      container.style.border = '1px solid #FFFFFF';
      chartArea.appendChild(container);

      // create chart and set initial options  
      chartArray[i] = echarts.init(container, 'dark',
      {
         //renderer: 'svg',
         //useDirtyRect: false,
      });


      chartArray[i].group = 'group1';
      echarts.connect('group1');

      // set base option 
      chartArray[i].setOption(baseOption);

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

      // set chart name
      chartArray[i].setOption( { title: { text: chartName[chartMap[i]]  }, }, false );


   }
}


var t0, t1, t2, t3;
var fetchTime;
var arrayProcessTime;
var updateChartTime;
var pileUp = 0;

const chartArray = [];
const chartDataArray = [];
const chartName = ['cpu', 'sync', 'ping', 'lcor', 'rcor', 'rewind', 'difs', 'tkbs'];

/*
// cpu sync
var numCharts = 2;
const chartMap = [0, 1];

*/


/*
// cpu sync rcor
var numCharts = 3;
const chartMap = [0, 1, 4];
*/


// all charts
var numCharts = 8;
var chartMap = [0, 1, 2, 3, 4, 5, 6, 7];


/*
// default 6 charts  cpu sync lcor rcor difs tkbs
var numCharts = 6;
const chartMap = [0, 1, 3, 4, 6, 7];
*/

/*
// cpu
var numCharts = 1;
const chartMap = [0];
*/


var chartHeight = 150;

var currentMaxTimestamp = 0;

var i, j, k;

var updateInterval = 200;
var chartTimeRange = 5;

let fetchRunning = false; 

let intervalId; // Variable to store the refresh timer interval ID

const frameMap = new Map();





async function fetchDataServerStatus()
{
   var serverStatusData = [];
   try
   {
      const response = await fetch('fetchDataServerStatus.php');
      if (!response.ok) { throw new Error(`HTTP error! Status: ${response.status}`); }
      serverStatusData = await response.json();
   } catch (error) { console.error("Fetch error:", error.message);
   } finally
   {
      updateClientStatusChartInsertControls(serverStatusData);
      updateServerSnapshotControls(serverStatusData);
      updateServerControls(serverStatusData);
   }
}




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

function setChartHeight(val)
{
   chartHeight = val;
   document.documentElement.style.setProperty('--ssChartHeight', `${val}px`);
}


document.addEventListener('DOMContentLoaded', (event) =>
{
   setupTopPageLinks();
   //setupTestButtons();

   // to set variable in css
   setChartHeight(chartHeight);

   setupCharts();
   startTimer();

   const statusControls = document.getElementById('statusControls');
   statusControls.style.display = 'flex';

   setupClientStatusChartRefreshControls(statusControls, updateInterval, chartTimeRange);
   setupClientStatusChartInsertControls(statusControls);

   setupClientStatusChartSelectControls(statusControls, numCharts, chartMap, chartHeight);

   setupServerSnapshotControls(statusControls);
   setupServerControls(statusControls);

   // add listener to container
   const clientStatusChartSelectControlsContainer = document.getElementById('clientStatusChartSelectControlsContainer');
   if (clientStatusChartSelectControlsContainer)
   {
      clientStatusChartSelectControlsContainer.addEventListener("change", (event) =>
      { 
         // get id of target element and compare first 15 char 
         var id = event.target.id;
         const sub = id.slice(0, 15);
         if (sub === 'chartTypeSelect')
         {
            // get chartNum from last digit
            const lastDigitChar = id.at(-1);
            const chartNum = Number(lastDigitChar);
            // get chartType from selectedIndex
            const chartType = event.target.selectedIndex;

            chartMap[chartNum] = chartType;
            setupCharts();
         }
         
         if (sub === 'chartPresetSele')
         {
            const index = event.target.selectedIndex;
            
            console.log("index:", index);
            if (index == 0)
            {
               // default 6 charts  cpu sync lcor rcor difs tkbs
               numCharts = 6;
               chartMap = [0, 1, 3, 4, 6, 7];
            }
            if (index == 1)
            {
               // cpu sync rcor
               numCharts = 3;
               chartMap = [0, 1, 4];
            }
            if (index == 2)
            {
               // all charts
               numCharts = 8;
               chartMap = [0, 1, 2, 3, 4, 5, 6, 7];
            }
            updateClientStatusChartSelectControls(numCharts, chartMap, chartHeight);
            setupCharts();
         }            
      });
   }
    

    
   const slider0 = document.getElementById('chartHeightSlider');
   slider0.addEventListener("input", () =>
   {
      setChartHeight(slider0.value);
   });

   const slider1 = document.getElementById('numChartsSlider');
   slider1.addEventListener("input", () =>
   {
      numCharts = slider1.value;
      updateClientStatusChartSelectControls(numCharts, chartMap, chartHeight);
      setupCharts();
   });

   const slider2 = document.getElementById('intervalSlider');
   slider2.addEventListener("input", () =>
   {
      updateInterval = slider2.value;
      changeTimer();
   });

   const slider3 = document.getElementById('rangeSlider');
   slider3.addEventListener("input", () =>
   {
      chartTimeRange = slider3.value;
   });


   const button = document.getElementById('stopButton');
   button.addEventListener("click", function()
   {
      if (intervalId)
      {
         stopTimer();
         button.textContent = 'Start';
      }
      else
      {
         startTimer();
         button.textContent = 'Stop';
      }
   });
   fetchDataServerStatus();
   setInterval(fetchDataServerStatus, 3000);
   
});




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

   var data = [];

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
      if (data.length) do_stuff(data); // only if data
   }
}








const arraysEqual = (a1, a2) =>
{
  if (a1.length !== a2.length) return false;
  return a1.every((value, index) => value === a2[index]);
};


function chartDataArrayPushOrUpdateRow(i, row)
{
   
   // does row with timestamp exist
   for (j=1; j<chartDataArray[i].length; j++)
      if (chartDataArray[i][j][0] === row[0])
      {
         // row perfectly matches existing row, do nothing
         if (arraysEqual(chartDataArray[i][j], row)) return;
         // row matches time value, but data is different  
         else
         {
            //let f = frameMap.get(row[0]);
            //console.log(f + 'Match: ' + chartDataArray[i][j] + "  -  " + row);

            // any non null columns in new row will overwrite array row
            for (k=1; k<9; k++)
               if (row[k] !== null) chartDataArray[i][j][k] = row[k];

            //console.log(f + 'Fixed: ' + chartDataArray[i][j] + "  -  " + row);
            return;

         }
      }

   // time value of row does not exist in array
   chartDataArray[i].push(row);   
   
}










function do_stuff(data)
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

   // for the first row, it may be partial
   // some player data for that time may aleady exist in the data

   // to handle this, I made a custom function: 'chartDataArrayPushOrUpdateRow(i, row)'
      
   
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

      var f = dat.frame;

      frameMap.set(timestamp, f);

      // value array  
      var v = [];        
      v[0] = dat.cpu;
      v[1] = dat.sync; 
      v[2] = dat.ping; 
      v[3] = dat.lcor; 
      v[4] = dat.rcor; 
      v[5] = dat.rewind; 
      v[6] = dat.difs; 
      v[7] = dat.tkbs; 

      // update current max timestamp  
      if (timestamp > currentMaxTimestamp) currentMaxTimestamp = timestamp;

      // new timestamp (or first)
      if (timestamp !== currentTimestamp)
      {
         fetchNumFrames++;
         for (i=0; i<numCharts; i++)
         {
            // if not first row, push old row
            if (currentTimestamp !== -1) chartDataArrayPushOrUpdateRow(i, row[i]);

            // clear row and set all to null 
            row[i] = [];
            for (j=0; j<9; j++) row[i][j] = null;

            // set t
            row[i][0] = timestamp;
         }           
         currentTimestamp = timestamp; // new currentTimestamp
      }         
      // set val column by player num     
      for (i=0; i<numCharts; i++) row[i][p+1] = v[chartMap[i]];
   }

   // push last row
   for (i=0; i<numCharts; i++) chartDataArrayPushOrUpdateRow(i, row[i]);

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
   arrayProcessTime = t2 - t1;
   document.getElementById('arrayProcessTimeSpan').innerHTML = arrayProcessTime;

   for (i=0; i<numCharts; i++)
   {
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

      // update data, legend and series in chart
      chartArray[i].setOption(
      {
         dataset: { source: chartDataArray[i] },
         series: series,  
         legend: { data: legend, show: false },
      }, false );


      // top chart only, show legend
      chartArray[0].setOption(
      {
         legend:
         {
            show: true,
            top: '0px',
            left: '32',
         },
      }, false );
   }
   
   t3 = Date.now();
   updateChartTime = t3 - t2;
   document.getElementById('updateChartTimeSpan').innerHTML = updateChartTime;
   document.getElementById('fetchNumFramesSpan').innerHTML = fetchNumFrames;
}
