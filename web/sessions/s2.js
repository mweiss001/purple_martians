


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
var chartArray = [];
var chartDataArray = [];
var numCharts = 3;

var curMaxSsid = 0;


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


   for (var i=0; i<numCharts; i++)
   {
      chartDataArray[i] = [];
      chartDataArray[i].push(['time', 'P0', 'P1', 'P2', 'P3', 'P4', 'P5', 'P6', 'P7']);
   }

   setInterval(fetchData, 100);


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
   //console.log(data);
/*
   for (var i=0; i<numCharts; i++)
   {
      chartDataArray[i] = [];
      chartDataArray[i].push(['time', 'P0', 'P1', 'P2', 'P3', 'P4', 'P5', 'P6', 'P7']);
   }
*/

   var row = [];

   var ct = -1; // current t

   for (const dat of data)
   {
      var t = dat.ss_id;
      var p = dat.pl_num;
      var v = [];

      v[0] = dat.cpu;
      v[1] = dat.sync; 
      //v[2] = dat.ping * 1000; 
      v[2] = dat.tkbs; 


      if (t > curMaxSsid) curMaxSsid = t;


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
   for (var i=0; i<numCharts; i++) chartDataArray[i].push(row[i]);


   // remove elements from start of array if too big
   var length = 400;
   for (var i=0; i<numCharts; i++)
   {
      if (chartDataArray[i].length > length) chartDataArray[i].splice(1, chartDataArray[i].length-length);
   }


   for (var i=0; i<numCharts; i++)
   {
      // search for valid data
      var vd = [0,0,0,0,0,0,0,0]; 
      for (var j=1; j<chartDataArray[i].length; j++)
         for (var k=1; k<9; k++)
            if (chartDataArray[i][j][k] !== null) vd[k-1] = 1;

      // only add series and legend if valid data
      const series = [];
      const legend = [];
      for (var j=0; j<8; j++)
      {
         var tt = "P" + j;
         if (vd[j])
         {
            series.push({ type: 'line', name: `${tt}`, encode: { y: `${tt}` }});
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

    name: 'Series 2',
      type: 'bar',
      itemStyle: {
        color: '#00ff00' // Explicitly set color to green
      },
      data: [12, 18, 13, 22]
    }
  ]
*/










